#include "tcpclient.h"
#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

TcpClient::TcpClient(const string &host, uint16_t port)
    : m_fd(-1)
{
    struct sockaddr_in addr;

    m_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (-1 == m_fd)
    {
        cerr << "[TCP] Failed to create socket\n";
        return;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(port);

    if (0 >= inet_pton(AF_INET, host.c_str(), &addr.sin_addr))
    {
        cerr << "[TCP] Invalid host address: " << host << "\n";
        close(m_fd);
        m_fd = -1;
        return;
    }

    if (0 > connect(m_fd, (struct sockaddr *)&addr, sizeof(addr)))
    {
        cerr << "[TCP] Failed to connect to " << host << ":" << port << "\n";
        close(m_fd);
        m_fd = -1;
        return;
    }

    fcntl(m_fd, F_SETFL, O_NONBLOCK);

    cout << "[TCP] Connected to " << host << ":" << port << "\n";
}

TcpClient::~TcpClient()
{
    if (-1 != m_fd)
    {
        close(m_fd);
    }
}

TcpClient::TcpClient(TcpClient &&other)
    : m_fd(other.m_fd)
{
    other.m_fd = -1;
}

TcpClient &TcpClient::operator=(TcpClient &&other)
{
    if (this != &other)
    {
        if (-1 != m_fd)
        {
            close(m_fd);
        }

        m_fd       = other.m_fd;
        other.m_fd = -1;
    }

    return *this;
}

bool TcpClient::isOpen() const
{
    return -1 != m_fd;
}

void TcpClient::send(const uint8_t *buf, uint8_t len)
{
    if (-1 == m_fd)
    {
        cerr << "[TCP] send called on closed connection\n";
        return;
    }

    ssize_t written = ::send(m_fd, buf, len, 0);

    if (written < 0)
    {
        cerr << "[TCP] write failed\n";
    }
}

int TcpClient::recv(uint8_t *buf, uint8_t max_len)
{
    if (-1 == m_fd)
    {
        return -1;
    }

    ssize_t n = ::recv(m_fd, buf, max_len, 0);

    if (0 == n)
    {
        close(m_fd);
        m_fd = -1;
        return -1;
    }

    if (0 > n)
    {
        return 0;
    }

    return (int)n;
}
