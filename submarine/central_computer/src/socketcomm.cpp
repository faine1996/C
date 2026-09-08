#include "socketcomm.h"
#include <iostream>
#include <cstring>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

SocketComm::SocketComm(uint16_t port)
    : m_listenFd(-1),
      m_clientFd(-1)
{
    struct sockaddr_in addr;
    int                opt = 1;

    m_listenFd = socket(AF_INET, SOCK_STREAM, 0);

    if (m_listenFd < 0)
    {
        cerr << "[SOCKET] Failed to create listening socket\n";
        return;
    }

    setsockopt(m_listenFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&addr, 0, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = htons(port);

    if (0 > bind(m_listenFd, (struct sockaddr *)&addr, sizeof(addr)))
    {
        cerr << "[SOCKET] Failed to bind port " << port << "\n";
        close(m_listenFd);
        m_listenFd = -1;
        return;
    }

    if (0 > listen(m_listenFd, 1))
    {
        cerr << "[SOCKET] Failed to listen on port " << port << "\n";
        close(m_listenFd);
        m_listenFd = -1;
        return;
    }

    struct timeval tv;
    tv.tv_sec  = 0;
    tv.tv_usec = 500000;
    setsockopt(m_listenFd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    cout << "[SOCKET] Listening for Ground Station on port " << port << "\n";

}

SocketComm::~SocketComm()
{
    if (-1 != m_clientFd)
    {
        close(m_clientFd);
    }

    if (-1 != m_listenFd)
    {
        close(m_listenFd);
    }
}

SocketComm::SocketComm(SocketComm &&other)
    : m_listenFd(other.m_listenFd),
      m_clientFd(other.m_clientFd)
{
    other.m_listenFd = -1;
    other.m_clientFd = -1;
}

SocketComm &SocketComm::operator=(SocketComm &&other)
{
    if (this != &other)
    {
        if (-1 != m_clientFd)
        {
            close(m_clientFd);
        }

        if (-1 != m_listenFd)
        {
            close(m_listenFd);
        }

        m_listenFd       = other.m_listenFd;
        m_clientFd       = other.m_clientFd;
        other.m_listenFd = -1;
        other.m_clientFd = -1;
    }

    return *this;
}

bool SocketComm::acceptClient()
{
    int newFd;

    if (-1 == m_listenFd)
    {
        return false;
    }

    if (-1 != m_clientFd)
    {
        close(m_clientFd);
        m_clientFd = -1;
    }

    newFd = accept(m_listenFd, nullptr, nullptr);

    if (0 > newFd)
    {
        if (EAGAIN != errno && EWOULDBLOCK != errno)
        {
            cerr << "[SOCKET] accept() failed\n";
        }
        return false;
    }


    fcntl(newFd, F_SETFL, O_NONBLOCK);

    m_clientFd = newFd;

    cout << "[SOCKET] Ground Station connected\n";

    return true;
}

bool SocketComm::isOpen() const
{
    return -1 != m_listenFd;
}

bool SocketComm::isClientConnected() const
{
    return -1 != m_clientFd;
}

void SocketComm::send(const uint8_t *buf, uint8_t len)
{
    if (-1 == m_clientFd)
    {
        cerr << "[SOCKET] send called with no client connected\n";
        return;
    }

    ssize_t written = ::send(m_clientFd, buf, len, 0);

    if (written < 0)
    {
        cerr << "[SOCKET] write failed\n";
    }
}

int SocketComm::recv(uint8_t *buf, uint8_t max_len)
{
    if (-1 == m_clientFd)
    {
        return -1;
    }

    ssize_t n = ::recv(m_clientFd, buf, max_len, 0);

    if (0 == n)
    {
        /* Peer closed the connection gracefully */
        close(m_clientFd);
        m_clientFd = -1;
        return -1;
    }

    if (0 > n)
    {
        /* EAGAIN means no data available — not an error in non-blocking mode */
        return 0;
    }

    return (int)n;
}