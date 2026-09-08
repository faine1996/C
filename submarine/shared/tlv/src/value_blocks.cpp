#include "value_blocks.h"

namespace tlv
{
    MeasurementBlock decodeMeasurementBlock(const uint8_t *value)
    {
        MeasurementBlock block;

        block.timestamp = static_cast<uint32_t>(value[0])
                         | (static_cast<uint32_t>(value[1]) << 8)
                         | (static_cast<uint32_t>(value[2]) << 16)
                         | (static_cast<uint32_t>(value[3]) << 24);

        block.temp = static_cast<int16_t>(static_cast<uint16_t>(value[4])
                                         | (static_cast<uint16_t>(value[5]) << 8));

        block.humidity = value[6];

        block.battery = static_cast<uint16_t>(value[7])
                       | (static_cast<uint16_t>(value[8]) << 8);

        block.light = static_cast<uint16_t>(value[9])
                     | (static_cast<uint16_t>(value[10]) << 8);

        block.mode = value[11];

        return block;
    }

    void encodeMeasurementBlock(const MeasurementBlock &block, uint8_t *out)
    {
        out[0] = static_cast<uint8_t>(block.timestamp);
        out[1] = static_cast<uint8_t>(block.timestamp >> 8);
        out[2] = static_cast<uint8_t>(block.timestamp >> 16);
        out[3] = static_cast<uint8_t>(block.timestamp >> 24);

        out[4] = static_cast<uint8_t>(static_cast<uint16_t>(block.temp));
        out[5] = static_cast<uint8_t>(static_cast<uint16_t>(block.temp) >> 8);

        out[6] = block.humidity;

        out[7] = static_cast<uint8_t>(block.battery);
        out[8] = static_cast<uint8_t>(block.battery >> 8);

        out[9]  = static_cast<uint8_t>(block.light);
        out[10] = static_cast<uint8_t>(block.light >> 8);

        out[11] = block.mode;
    }

    EventBlock decodeEventBlock(const uint8_t *value)
    {
        EventBlock block;

        block.eventType = value[0];
        block.detail    = value[1];

        block.timestamp = static_cast<uint32_t>(value[2])
                         | (static_cast<uint32_t>(value[3]) << 8)
                         | (static_cast<uint32_t>(value[4]) << 16)
                         | (static_cast<uint32_t>(value[5]) << 24);

        return block;
    }

    void encodeEventBlock(const EventBlock &block, uint8_t *out)
    {
        out[0] = block.eventType;
        out[1] = block.detail;

        out[2] = static_cast<uint8_t>(block.timestamp);
        out[3] = static_cast<uint8_t>(block.timestamp >> 8);
        out[4] = static_cast<uint8_t>(block.timestamp >> 16);
        out[5] = static_cast<uint8_t>(block.timestamp >> 24);
    }
}