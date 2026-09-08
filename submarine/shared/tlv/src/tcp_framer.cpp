#include "tcp_framer.h"

namespace tlv
{
    TcpFramer::TcpFramer()
    {
        reset();
    }

    void TcpFramer::feedByte(uint8_t byte)
    {
        switch (m_state)
        {
            case RX_WAIT_LEN:
                m_len      = byte;
                m_valueIdx = 0U;
                m_state    = RX_WAIT_TAG;
                break;

            case RX_WAIT_TAG:
                m_tag = byte;

                if (0U == m_len)
                {
                    if (m_handler)
                    {
                        m_handler(m_tag, m_value, m_len);
                    }
                    reset();
                }
                else
                {
                    m_state = RX_READ_VALUE;
                }
                break;

            case RX_READ_VALUE:
                m_value[m_valueIdx] = byte;
                ++m_valueIdx;

                if (m_valueIdx >= m_len)
                {
                    if (m_handler)
                    {
                        m_handler(m_tag, m_value, m_len);
                    }
                    reset();
                }
                break;

            default:
                reset();
                break;
        }
    }

    void TcpFramer::onFrame(FrameHandler handler)
    {
        m_handler = std::move(handler);
    }

    uint16_t TcpFramer::buildFrame(uint8_t tag,
                                   const uint8_t *value,
                                   uint8_t len,
                                   uint8_t *out)
    {
        uint8_t  i;
        uint16_t frameLen;

        out[0] = len;
        out[1] = tag;

        for (i = 0U; i < len; ++i)
        {
            out[2U + i] = value[i];
        }

        frameLen = static_cast<uint16_t>(2U + len);

        return frameLen;
    }

    void TcpFramer::reset()
    {
        m_state    = RX_WAIT_LEN;
        m_tag      = 0U;
        m_len      = 0U;
        m_valueIdx = 0U;
    }
}