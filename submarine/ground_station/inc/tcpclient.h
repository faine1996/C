#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <cstdint>
#include <string>

class TcpClient
{
public:
    /**
     * @brief  Constructor. Connects to host:port over TCP. Check isOpen()
     *         afterward — a failed connection leaves the object usable
     *         but not live, same as SerialComm's failed-open behaviour.
     * @param  host  Hostname or IP address (e.g. "127.0.0.1").
     * @param  port  TCP port to connect to.
     */
    TcpClient(const std::string &host, uint16_t port);

    /**
     * @brief  Destructor. Closes the socket (safe if not open).
     */
    ~TcpClient();

    /**
     * @brief  Copy constructor — deleted. Cannot duplicate an open socket.
     */
    TcpClient(const TcpClient &) = delete;

    /**
     * @brief  Copy assignment — deleted. Same reason.
     */
    TcpClient &operator=(const TcpClient &) = delete;

    /**
     * @brief  Move constructor. Transfers the fd, leaves source unable
     *         to close it.
     */
    TcpClient(TcpClient &&other);

    /**
     * @brief  Move assignment. Closes existing fd, then transfers other's.
     */
    TcpClient &operator=(TcpClient &&other);

    /**
     * @brief  Returns true if the connection is open.
     */
    bool isOpen() const;

    /**
     * @brief  Sends len bytes to the server.
     */
    void send(const uint8_t *buf, uint8_t len);

    /**
     * @brief  Reads up to max_len bytes from the server.
     * @return Number of bytes read, 0 if none available right now, or -1
     *         if not connected or the connection has closed.
     */
    int recv(uint8_t *buf, uint8_t max_len);

private:
    int m_fd;
};

#endif // TCP_CLIENT_H
