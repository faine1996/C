#include "uart_framer.h"

namespace tlv
{
    UartFramer::UartFramer()
    {
        reset();
    }

    void UartFramer::feedByte(uint8_t byte)
    {
        switch (m_state)
        {
            case RX_WAIT_SOF:
                if (SOF == byte)
                {
                    m_state = RX_READ_TAG;
                }
                break;

            case RX_READ_TAG:
                m_tag            = byte;
                m_checksumAccum  = byte;
                m_state          = RX_READ_LEN;
                break;

            case RX_READ_LEN:
                m_len           = byte;
                m_checksumAccum = static_cast<uint8_t>(m_checksumAccum + byte);
                m_valueIdx      = 0U;
                m_state         = (0U == byte) ? RX_READ_CHK : RX_READ_VALUE;
                break;

            case RX_READ_VALUE:
                m_value[m_valueIdx] = byte;
                m_checksumAccum     = static_cast<uint8_t>(m_checksumAccum + byte);
                ++m_valueIdx;

                if (m_valueIdx >= m_len)
                {
                    m_state = RX_READ_CHK;
                }
                break;

            case RX_READ_CHK:
                if (byte == m_checksumAccum)
                {
                    if (m_handler)
                    {
                        m_handler(m_tag, m_value, m_len);
                    }
                }
                reset();
                break;

            default:
                reset();
                break;
        }
    }

    void UartFramer::onFrame(FrameHandler handler)
    {
        m_handler = std::move(handler);
    }

    uint16_t UartFramer::buildFrame(uint8_t tag,
                                    const uint8_t *value,
                                    uint8_t len,
                                    uint8_t *out)
    {
        uint8_t  chk;
        uint8_t  i;
        uint16_t frameLen;

        chk    = static_cast<uint8_t>(tag + len);
        out[0] = SOF;
        out[1] = tag;
        out[2] = len;

        for (i = 0U; i < len; ++i)
        {
            out[3U + i] = value[i];
            chk = static_cast<uint8_t>(chk + value[i]);
        }

        out[3U + len] = chk;
        frameLen      = static_cast<uint16_t>(4U + len);

        return frameLen;
    }

    void UartFramer::reset()
    {
        m_state         = RX_WAIT_SOF;
        m_tag           = 0U;
        m_len           = 0U;
        m_valueIdx      = 0U;
        m_checksumAccum = 0U;
    }
}
