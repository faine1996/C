#ifndef TLV_TAGS_H
#define TLV_TAGS_H

#include <cstdint>

namespace tlv
{
    // LNC -> Central Computer (telemetry & events)
    constexpr uint8_t TAG_KEEPALIVE        = 0x10;
    constexpr uint8_t TAG_EVENT            = 0x11;
    constexpr uint8_t TAG_DATA_REPORT      = 0x12;
    constexpr uint8_t TAG_TIME_SYNC_REQ    = 0x13;

    // Central Computer -> LNC (commands)
    constexpr uint8_t TAG_SET_CONFIG       = 0x20;
    constexpr uint8_t TAG_SET_TIME         = 0x21;
    constexpr uint8_t TAG_GET_TIME         = 0x22;
    constexpr uint8_t TAG_GET_DATA_RANGE   = 0x23;
    constexpr uint8_t TAG_GET_EVENTS_RANGE = 0x24;

    // LNC -> Central Computer (responses)
    constexpr uint8_t TAG_TIME_REPORT      = 0x80;
    constexpr uint8_t TAG_DATA_ITEM        = 0x81; // LEN 0 = end of stream
    constexpr uint8_t TAG_EVENT_ITEM       = 0x82; // LEN 0 = end of stream

    // event_type values carried inside an EVENT/EVENT_ITEM's EVENT BLOCK
    constexpr uint8_t EVENT_MODE_CHANGE     = 0x01;
    constexpr uint8_t EVENT_OBJECT_DETECTED = 0x02;
    constexpr uint8_t EVENT_OBJECT_CLEARED  = 0x03;
    constexpr uint8_t EVENT_CONFIG_CHANGED  = 0x04;
    constexpr uint8_t EVENT_STARTUP         = 0x05;

    // system mode values carried inside a MEASUREMENT BLOCK
    constexpr uint8_t MODE_NORMAL  = 0;
    constexpr uint8_t MODE_WARNING = 1;
    constexpr uint8_t MODE_ERROR   = 2;
}

#endif // TLV_TAGS_H