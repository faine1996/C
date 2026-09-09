#include "groundstationlink.h"
#include "tlv_tags.h"
#include <iostream>
#include <mutex>
#include <chrono>

using namespace std;

/* How long to wait, with no new data arriving, before giving up on the
 * LNC's response and telling the Ground Station the stream is over. */
static const chrono::seconds RESPONSE_TIMEOUT(5);

GroundStationLink::GroundStationLink(CentralComputer &cc, uint16_t port)
    : m_cc(cc),
      m_socket(port),
      m_stop(false)
{
    m_framer.onFrame([this](uint8_t tag, const uint8_t *value, uint8_t len)
    {
        relayRequest(tag, value, len);
    });

    m_thread = thread(&GroundStationLink::run, this);
}

GroundStationLink::~GroundStationLink()
{
    m_stop = true;

    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

void GroundStationLink::run()
{
    while (!m_stop)
    {
        if (!m_socket.acceptClient())
        {
            continue;
        }

        serveClient();
    }
}

void GroundStationLink::serveClient()
{
    uint8_t buf[64];
    int     n;

    m_framer.reset();

    while (!m_stop && m_socket.isClientConnected())
    {
        n = m_socket.recv(buf, sizeof(buf));

        if (n < 0)
        {
            break;
        }

        for (int i = 0; i < n; ++i)
        {
            m_framer.feedByte(buf[i]);
        }

        this_thread::sleep_for(chrono::milliseconds(10));
    }
}

void GroundStationLink::relayRequest(uint8_t tag, const uint8_t *value, uint8_t len)
{
    uint32_t start;
    uint32_t end;
    uint8_t  frameBuf[257];
    uint16_t frameLen;
    bool     done;
    chrono::steady_clock::time_point lastData;

    if (len < 8U)
    {
        return;
    }

    start = (uint32_t)value[0]
          | ((uint32_t)value[1] << 8)
          | ((uint32_t)value[2] << 16)
          | ((uint32_t)value[3] << 24);

    end = (uint32_t)value[4]
        | ((uint32_t)value[5] << 8)
        | ((uint32_t)value[6] << 16)
        | ((uint32_t)value[7] << 24);

    done = false;

    {
        lock_guard<mutex> lock(m_cc.uartMutex());

        m_cc.setRangeItemHandler([this, &done, &lastData, &frameBuf, &frameLen](uint8_t itemTag, const uint8_t *itemValue, uint8_t itemLen)
        {
            frameLen = tlv::TcpFramer::buildFrame(itemTag, itemValue, itemLen, frameBuf);
            m_socket.send(frameBuf, (uint8_t)frameLen);
            lastData = chrono::steady_clock::now();

            if (0U == itemLen)
            {
                done = true;
            }
        });

        if (tlv::TAG_GET_DATA_RANGE == tag)
        {
            m_cc.sendGetDataRange(start, end);
        }
        else
        {
            m_cc.sendGetEventsRange(start, end);
        }

        lastData = chrono::steady_clock::now();

        while (!done)
        {
            m_cc.processIncoming();

            if (chrono::steady_clock::now() - lastData > RESPONSE_TIMEOUT)
            {
                cerr << "[GS-LINK] Timed out waiting for LNC response\n";

                {
                    uint8_t responseTag = (tlv::TAG_GET_DATA_RANGE == tag)
                                         ? tlv::TAG_DATA_ITEM
                                         : tlv::TAG_EVENT_ITEM;

                    frameLen = tlv::TcpFramer::buildFrame(responseTag, nullptr, 0U, frameBuf);
                    m_socket.send(frameBuf, (uint8_t)frameLen);
                }

                break;
            }

            this_thread::sleep_for(chrono::milliseconds(10));
        }

        m_cc.setRangeItemHandler(nullptr);
    }
}
