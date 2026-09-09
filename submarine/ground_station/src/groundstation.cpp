#include "groundstation.h"
#include "tlv_tags.h"
#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

static const chrono::seconds RESPONSE_TIMEOUT(5);

GroundStation::GroundStation(const string &host, uint16_t port)
    : m_client(host, port)
{
}

bool GroundStation::isConnected() const
{
    return m_client.isOpen();
}

vector<tlv::MeasurementBlock> GroundStation::requestDataRange(uint32_t start, uint32_t end)
{
    vector<tlv::MeasurementBlock> records;
    uint8_t  value[8];
    uint8_t  frameBuf[257];
    uint16_t frameLen;
    bool     done;
    chrono::steady_clock::time_point lastData;

    if (!m_client.isOpen())
    {
        return records;
    }

    value[0] = static_cast<uint8_t>(start);
    value[1] = static_cast<uint8_t>(start >> 8);
    value[2] = static_cast<uint8_t>(start >> 16);
    value[3] = static_cast<uint8_t>(start >> 24);
    value[4] = static_cast<uint8_t>(end);
    value[5] = static_cast<uint8_t>(end >> 8);
    value[6] = static_cast<uint8_t>(end >> 16);
    value[7] = static_cast<uint8_t>(end >> 24);

    done = false;

    m_framer.reset();
    m_framer.onFrame([&](uint8_t tag, const uint8_t *itemValue, uint8_t itemLen)
    {
        if (tlv::TAG_DATA_ITEM != tag)
        {
            return;
        }

        if (0U == itemLen)
        {
            done = true;
            return;
        }

        records.push_back(tlv::decodeMeasurementBlock(itemValue));
    });

    frameLen = tlv::TcpFramer::buildFrame(tlv::TAG_GET_DATA_RANGE, value, 8U, frameBuf);
    m_client.send(frameBuf, static_cast<uint8_t>(frameLen));

    lastData = chrono::steady_clock::now();

    while (!done)
    {
        uint8_t buf[64];
        int     n = m_client.recv(buf, sizeof(buf));

        if (n < 0)
        {
            break;
        }

        if (n > 0)
        {
            for (int i = 0; i < n; ++i)
            {
                m_framer.feedByte(buf[i]);
            }

            lastData = chrono::steady_clock::now();
        }

        if (chrono::steady_clock::now() - lastData > RESPONSE_TIMEOUT)
        {
            cerr << "[GS] Timed out waiting for response\n";
            break;
        }

        this_thread::sleep_for(chrono::milliseconds(10));
    }

    return records;
}

vector<tlv::EventBlock> GroundStation::requestEventsRange(uint32_t start, uint32_t end)
{
    vector<tlv::EventBlock> records;
    uint8_t  value[8];
    uint8_t  frameBuf[257];
    uint16_t frameLen;
    bool     done;
    chrono::steady_clock::time_point lastData;

    if (!m_client.isOpen())
    {
        return records;
    }

    value[0] = static_cast<uint8_t>(start);
    value[1] = static_cast<uint8_t>(start >> 8);
    value[2] = static_cast<uint8_t>(start >> 16);
    value[3] = static_cast<uint8_t>(start >> 24);
    value[4] = static_cast<uint8_t>(end);
    value[5] = static_cast<uint8_t>(end >> 8);
    value[6] = static_cast<uint8_t>(end >> 16);
    value[7] = static_cast<uint8_t>(end >> 24);

    done = false;

    m_framer.reset();
    m_framer.onFrame([&](uint8_t tag, const uint8_t *itemValue, uint8_t itemLen)
    {
        if (tlv::TAG_EVENT_ITEM != tag)
        {
            return;
        }

        if (0U == itemLen)
        {
            done = true;
            return;
        }

        records.push_back(tlv::decodeEventBlock(itemValue));
    });

    frameLen = tlv::TcpFramer::buildFrame(tlv::TAG_GET_EVENTS_RANGE, value, 8U, frameBuf);
    m_client.send(frameBuf, static_cast<uint8_t>(frameLen));

    lastData = chrono::steady_clock::now();

    while (!done)
    {
        uint8_t buf[64];
        int     n = m_client.recv(buf, sizeof(buf));

        if (n < 0)
        {
            break;
        }

        if (n > 0)
        {
            for (int i = 0; i < n; ++i)
            {
                m_framer.feedByte(buf[i]);
            }

            lastData = chrono::steady_clock::now();
        }

        if (chrono::steady_clock::now() - lastData > RESPONSE_TIMEOUT)
        {
            cerr << "[GS] Timed out waiting for response\n";
            break;
        }

        this_thread::sleep_for(chrono::milliseconds(10));
    }

    return records;
}
