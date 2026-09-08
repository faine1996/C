#ifndef SOCKET_COMM_H
#define SOCKET_COMM_H

#include <cstdint>

class SocketComm
{
public:
    /**
     * @brief  Constructor. Opens a TCP listening socket bound to the given
     *         port on all interfaces and starts listening for one client.
     *         Does not block — call acceptClient() to wait for a connection.
     * @param  port  TCP port number to listen on.
     */
    explicit SocketComm(uint16_t port);

    /**
     * @brief  Destructor. Closes both the listening socket and the
     *         accepted client connection (safe if either is not open).
     */
    ~SocketComm();

    /**
     * @brief  Copy constructor — deleted. Cannot duplicate an open socket.
     */
    SocketComm(const SocketComm &) = delete;

    /**
     * @brief  Copy assignment — deleted. Same reason.
     */
    SocketComm &operator=(const SocketComm &) = delete;

    /**
     * @brief  Move constructor. Transfers both fds, leaves source unable
     *         to close them.
     */
    SocketComm(SocketComm &&other);

    /**
     * @brief  Move assignment. Closes existing fds, then transfers other's.
     */
    SocketComm &operator=(SocketComm &&other);

    /**
     * @brief  Blocks until one client connects to the listening socket.
     *         Closes any previously accepted client first.
     * @return true if a client connected successfully, false on error.
     */
    bool acceptClient();

    /**
     * @brief  Returns true if the listening socket is open.
     */
    bool isOpen() const;

    /**
     * @brief  Returns true if a client is currently connected.
     */
    bool isClientConnected() const;

    /**
     * @brief  Sends len bytes to the connected client.
     */
    void send(const uint8_t *buf, uint8_t len);

    /**
     * @brief  Reads up to max_len bytes from the connected client.
     * @return Number of bytes read, 0 if none available right now, or -1
     *         if no client is connected or the connection has closed.
     */
    int recv(uint8_t *buf, uint8_t max_len);

private:
    int m_listenFd;
    int m_clientFd;
};

#endif // SOCKET_COMM_H
