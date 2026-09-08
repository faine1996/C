#ifndef GROUND_STATION_LINK_H
#define GROUND_STATION_LINK_H

#include "centralcomputer.h"
#include "socketcomm.h"
#include "tcp_framer.h"
#include <cstdint>
#include <thread>
#include <atomic>

class GroundStationLink
{
public:
    /**
     * @brief  Constructor. Opens a TCP listening socket on the given port
     *         and starts a background thread that accepts Ground Station
     *         connections and relays GET_DATA_RANGE/GET_EVENTS_RANGE
     *         requests to the given CentralComputer's LNC link.
     * @param  cc    The live CentralComputer to relay requests through.
     * @param  port  TCP port to listen on.
     */
    GroundStationLink(CentralComputer &cc, uint16_t port);

    /**
     * @brief  Destructor. Signals the background thread to stop and joins
     *         it before returning.
     */
    ~GroundStationLink();

    /**
     * @brief  Copy constructor — deleted. Owns a thread and a socket.
     */
    GroundStationLink(const GroundStationLink &) = delete;

    /**
     * @brief  Copy assignment — deleted. Same reason.
     */
    GroundStationLink &operator=(const GroundStationLink &) = delete;

private:
    /**
     * @brief  Background thread entry point. Runs until m_stop is set.
     */
    void run();

    /**
     * @brief  Handles one already-connected Ground Station: reads request
     *         frames and relays each one until the client disconnects.
     */
    void serveClient();

    /**
     * @brief  Relays a single GET_DATA_RANGE/GET_EVENTS_RANGE request:
     *         forwards it to the LNC, then relays the streamed response
     *         back to the Ground Station until end-of-stream or timeout.
     * @param  tag    Request tag (TAG_GET_DATA_RANGE or TAG_GET_EVENTS_RANGE).
     * @param  value  Request value bytes (8-byte start/end range).
     * @param  len    Value length.
     */
    void relayRequest(uint8_t tag, const uint8_t *value, uint8_t len);

    CentralComputer  &m_cc;
    SocketComm        m_socket;
    tlv::TcpFramer    m_framer;
    std::thread       m_thread;
    std::atomic<bool> m_stop;
};

#endif // GROUND_STATION_LINK_H
