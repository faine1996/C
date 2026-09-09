#ifndef GROUND_STATION_H
#define GROUND_STATION_H

#include "tcpclient.h"
#include "tcp_framer.h"
#include "value_blocks.h"
#include <cstdint>
#include <string>
#include <vector>

class GroundStation
{
public:
    /**
     * @brief  Constructor. Connects to the Central Computer at host:port.
     *         Check isConnected() afterward.
     * @param  host  CC hostname/IP (e.g. "127.0.0.1").
     * @param  port  CC's Ground Station TCP port.
     */
    GroundStation(const std::string &host, uint16_t port);

    /**
     * @brief  Returns true if connected to the Central Computer.
     */
    bool isConnected() const;

    /**
     * @brief  Requests measurement records for [start, end] (Unix epoch
     *         seconds, inclusive) and blocks until the full response is
     *         received or a timeout elapses.
     * @param  start  Range start, epoch seconds.
     * @param  end    Range end, epoch seconds.
     * @return The matching records, in the order received. Empty if none
     *         matched, the request failed, or it timed out.
     */
    std::vector<tlv::MeasurementBlock> requestDataRange(uint32_t start, uint32_t end);

    /**
     * @brief  Requests event records for [start, end] (Unix epoch
     *         seconds, inclusive) and blocks until the full response is
     *         received or a timeout elapses.
     * @param  start  Range start, epoch seconds.
     * @param  end    Range end, epoch seconds.
     * @return The matching records, in the order received. Empty if none
     *         matched, the request failed, or it timed out.
     */
    std::vector<tlv::EventBlock> requestEventsRange(uint32_t start, uint32_t end);

private:
    TcpClient      m_client;
    tlv::TcpFramer m_framer;
};

#endif // GROUND_STATION_H