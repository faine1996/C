#ifndef UART_FRAMER_H
#define UART_FRAMER_H

#include <cstdint>
#include <functional>

namespace tlv
{
    // Builds and parses the LNC <-> Central Computer wire format:
    // SOF(0xAA) + TAG(1) + LEN(1) + VALUE(LEN) + CHECKSUM(1).
    // CHECKSUM = (TAG + LEN + every VALUE byte) & 0xFF, SOF excluded.
    // A frame with a bad checksum is discarded silently; the receiver
    // returns to hunting for the next SOF.
    class UartFramer
    {
    public:
        // Called once per successfully decoded frame. `value` is only
        // valid for the duration of the call.
        using FrameHandler = std::function<void(uint8_t tag, const uint8_t *value, uint8_t len)>;

        UartFramer();

        // Feeds one received byte into the state machine. Invokes the
        // registered handler once per complete, checksum-valid frame.
        // Call once per byte read off the wire.
        void feedByte(uint8_t byte);

        // Registers the callback invoked on each successfully decoded
        // frame. Replaces any previously registered handler.
        void onFrame(FrameHandler handler);

        // Builds SOF + tag + len + value + checksum into `out`.
        // `out` must point to at least len + 4 bytes.
        // Returns the total frame length written (len + 4).
        static uint16_t buildFrame(uint8_t tag,
                                   const uint8_t *value,
                                   uint8_t len,
                                   uint8_t *out);

        // Resets the state machine to WAIT_SOF, discarding any partially
        // received frame.
        void reset();

    private:
        static constexpr uint8_t SOF = 0xAA;

        typedef enum
        {
            RX_WAIT_SOF   = 0,
            RX_READ_TAG   = 1,
            RX_READ_LEN   = 2,
            RX_READ_VALUE = 3,
            RX_READ_CHK   = 4
        } RxState_t;

        RxState_t    m_state;
        uint8_t      m_tag;
        uint8_t      m_len;
        uint8_t      m_value[255];
        uint8_t      m_valueIdx;
        uint8_t      m_checksumAccum;
        FrameHandler m_handler;
    };
}

#endif // UART_FRAMER_H