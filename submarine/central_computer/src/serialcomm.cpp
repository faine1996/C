#include "serialcomm.h"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <cstring>

using namespace std;

SerialComm::SerialComm(const string &port)
    : m_fd(-1),
      m_port(port)
{
    /* Open port: read/write, not a controlling terminal, non-blocking */
    m_fd = open(port.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);

    if (m_fd < 0)
    {
        cerr << "[SERIAL] Failed to open " << port << "\n";
        m_fd = -1;
        return;
    }

    struct termios tty;
    memset(&tty, 0, sizeof(tty));

    if (0 != tcgetattr(m_fd, &tty))
    {
        cerr << "[SERIAL] tcgetattr failed on " << port << "\n";
        close(m_fd);
        m_fd = -1;
        return;
    }

    /* Baud rate — must match LNC (115200) */
    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);

    /* 8 data bits */
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;

    /* No parity */
    tty.c_cflag &= ~PARENB;

    /* 1 stop bit */
    tty.c_cflag &= ~CSTOPB;

    /* Disable hardware flow control */
    tty.c_cflag &= ~CRTSCTS;

    /* Enable receiver, ignore modem status lines */
    tty.c_cflag |= (CLOCAL | CREAD);

    /* Raw mode — no line buffering, no echo, no signals */
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    /* No software flow control */
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);

    /* No output processing */
    tty.c_oflag &= ~OPOST;

    /* Non-blocking: return immediately even if no bytes available */
    tty.c_cc[VMIN]  = 0;
    tty.c_cc[VTIME] = 0;

    if (tcsetattr(m_fd, TCSANOW, &tty) != 0)
    {
        cerr << "[SERIAL] tcsetattr failed on " << port << "\n";
        close(m_fd);
        m_fd = -1;
        return;
    }

    cout << "[SERIAL] Opened " << port << "\n";
}

SerialComm::~SerialComm()
{
    if (m_fd != -1)
    {
        close(m_fd);
        m_fd = -1;
    }
}

/* Move constructor — transfer fd, leave source unable to close it */
SerialComm::SerialComm(SerialComm &&other)
    : m_fd(other.m_fd),
      m_port(move(other.m_port))
{
    other.m_fd = -1;
}

/* Move assignment — close what we own, then take the other's fd */
SerialComm &SerialComm::operator=(SerialComm &&other)
{
    if (this != &other)
    {
        if (m_fd != -1)
        {
            close(m_fd);
        }

        m_fd       = other.m_fd;
        m_port     = move(other.m_port);
        other.m_fd = -1;
    }

    return *this;
}

void SerialComm::send(const uint8_t *buf, uint8_t len)
{
    if (m_fd == -1)
    {
        cerr << "[SERIAL] send called on closed port\n";
        return;
    }

    ssize_t written = write(m_fd, buf, len);

    if (written < 0)
    {
        cerr << "[SERIAL] write failed\n";
    }
}

int SerialComm::recv(uint8_t *buf, uint8_t max_len)
{
    if (m_fd == -1)
    {
        return -1;
    }

    ssize_t n = read(m_fd, buf, max_len);

    if (n < 0)
    {
        /* EAGAIN means no data available — not an error in non-blocking mode */
        return 0;
    }

    return (int)n;
}

bool SerialComm::isOpen() const
{
    return m_fd != -1;
}