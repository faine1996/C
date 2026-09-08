#ifndef TCP_FRAMER_H
#define TCP_FRAMER_H

#include <cstdint>
#include <functional>

namespace tlv
{
    // Builds and parses the Central Computer <-> Ground Station wire
    // format: LEN(1) + TAG(1) + VALUE(LEN). No SOF, no checksum — TCP
    // already guarantees ordered, uncorrupted delivery, so the extra
    // resync/integrity fields the UART link needs would be redundant here.
    class TcpFramer
    {
    public:
        // Called once per successfully decoded frame. `value` is only
        // valid for the duration of the call.
        using FrameHandler = std::function<void(uint8_t tag, const uint8_t *value, uint8_t len)>;

        TcpFramer();

        // Feeds one received byte into the state machine. Invokes the
        // registered handler once per complete frame.
        // Call once per byte read off the socket.
        void feedByte(uint8_t byte);

        // Registers the callback invoked on each successfully decoded
        // frame. Replaces any previously registered handler.
        void onFrame(FrameHandler handler);

        // Builds LEN + TAG + VALUE into `out`.
        // `out` must point to at least len + 2 bytes.
        // Returns the total frame length written (len + 2).
        static uint16_t buildFrame(uint8_t tag,
                                   const uint8_t *value,
                                   uint8_t len,
                                   uint8_t *out);

        // Resets the state machine to WAIT_LEN, discarding any partially
        // received frame.
        void reset();

    private:
        typedef enum
        {
            RX_WAIT_LEN   = 0,
            RX_WAIT_TAG   = 1,
            RX_READ_VALUE = 2
        } RxState_t;

        RxState_t    m_state;
        uint8_t      m_tag;
        uint8_t      m_len;
        uint8_t      m_value[255];
        uint8_t      m_valueIdx;
        FrameHandler m_handler;
    };
}

#endif // TCP_FRAMER_H