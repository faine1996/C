#ifndef VALUE_BLOCKS_H
#define VALUE_BLOCKS_H

#include <cstddef>
#include <cstdint>

namespace tlv
{
    // MEASUREMENT BLOCK (12 bytes) — carried by KEEPALIVE, DATA_REPORT, DATA_ITEM
    struct MeasurementBlock
    {
        uint32_t timestamp; // epoch seconds
        int16_t  temp;      // whole degrees C
        uint8_t  humidity;  // percent
        uint16_t battery;   // raw 12-bit ADC
        uint16_t light;     // raw 12-bit ADC
        uint8_t  mode;      // 0 = Normal, 1 = Warning, 2 = Error
    };

    constexpr size_t MEASUREMENT_BLOCK_LEN = 12;

    // Decodes a 12-byte little-endian MEASUREMENT BLOCK.
    // `value` must point to at least MEASUREMENT_BLOCK_LEN bytes.
    MeasurementBlock decodeMeasurementBlock(const uint8_t *value);

    // Encodes a MEASUREMENT BLOCK into `out`, little-endian, matching
    // decodeMeasurementBlock's layout.
    // `out` must point to at least MEASUREMENT_BLOCK_LEN bytes.
    void encodeMeasurementBlock(const MeasurementBlock &block, uint8_t *out);

    // EVENT BLOCK (6 bytes) — carried by EVENT, EVENT_ITEM
    struct EventBlock
    {
        uint8_t  eventType;
        uint8_t  detail;
        uint32_t timestamp; // epoch seconds
    };

    constexpr size_t EVENT_BLOCK_LEN = 6;

    // Decodes a 6-byte little-endian EVENT BLOCK.
    // `value` must point to at least EVENT_BLOCK_LEN bytes.
    EventBlock decodeEventBlock(const uint8_t *value);

    // Encodes an EVENT BLOCK into `out`, little-endian, matching
    // decodeEventBlock's layout.
    // `out` must point to at least EVENT_BLOCK_LEN bytes.
    void encodeEventBlock(const EventBlock &block, uint8_t *out);
}

#endif // VALUE_BLOCKS_H