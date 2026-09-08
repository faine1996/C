#ifndef SERIAL_COMM_H
#define SERIAL_COMM_H

#include <string>
#include <cstdint>

class SerialComm
{
public:
    /**
     * @brief  Regular constructor. Opens the serial port at the given path
     *         and configures it via termios: 9600 baud, 8N1, raw mode,
     *         non-blocking. Sets m_fd = -1 on failure.
     * @param  port  Device path e.g. "/dev/ttyACM0".
     */
    explicit SerialComm(const std::string &port);

    /**
     * @brief  Destructor. Closes m_fd if it is open (not -1).
     */
    ~SerialComm();

    /**
     * @brief  Copy constructor — deleted. A file descriptor cannot be
     *         duplicated safely. Two objects closing the same fd would crash.
     */
    SerialComm(const SerialComm &)            = delete;

    /**
     * @brief  Copy assignment — deleted. Same reason as copy constructor.
     */
    SerialComm &operator=(const SerialComm &) = delete;

    /**
     * @brief  Move constructor. Transfers m_fd from other to this.
     *         Sets other.m_fd = -1 so other's destructor does not close it.
     */
    SerialComm(SerialComm &&other);

    /**
     * @brief  Move assignment. Closes existing m_fd first, then transfers
     *         other.m_fd, sets other.m_fd = -1.
     */
    SerialComm &operator=(SerialComm &&other);

    /**
     * @brief  Sends len bytes from buf over the serial port.
     * @param  buf  Pointer to byte array to send.
     * @param  len  Number of bytes to send.
     */
    void send(const uint8_t *buf, uint8_t len);

    /**
     * @brief  Non-blocking read. Returns number of bytes read (0 if none).
     * @param  buf      Buffer to read into.
     * @param  max_len  Maximum bytes to read.
     * @return int — bytes read, 0 if nothing available, -1 on error.
     */
    int recv(uint8_t *buf, uint8_t max_len);

    /**
     * @brief  Returns true if the port is open (m_fd != -1).
     */
    bool isOpen() const;

private:
    int         m_fd;
    std::string m_port;
};

#endif /* SERIAL_COMM_H */