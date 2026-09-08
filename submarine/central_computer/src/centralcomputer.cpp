#include "centralcomputer.h"
#include <iostream>
#include <cstring>
#include <ctime>

using namespace std;

/* TLV constants — match LNC comm.h exactly */
static const uint8_t COMM_SOF          = 0xAAU;
static const uint8_t TAG_SET_TIME      = 0x21U;
static const uint8_t TAG_SET_CONFIG    = 0x20U;
static const uint8_t TAG_KEEPALIVE     = 0x10U;
static const uint8_t TAG_EVENT         = 0x11U;
static const uint8_t TAG_TIME_SYNC_REQ = 0x13U;
static const uint8_t TAG_GET_TIME      = 0x22U;
static const uint8_t TAG_TIME_REPORT   = 0x80U;

static const char *MODE_NAMES[]  = { "Normal", "Warning", "Error" };
static const char *EVENT_NAMES[] = {
    "unknown",
    "mode_change",
    "object_detected",
    "object_cleared",
    "config_changed",
    "startup"
};

/* -----------------------------------------------------------------------
 * Constructors / destructor / Big Five
 * --------------------------------------------------------------------- */

/* Default constructor — non-live CC */
CentralComputer::CentralComputer()
    : m_comms(nullptr),
      m_set_time_wall(0),
      m_time_synced(false)
{
    rxReset();
}

/* Overloaded constructor — live CC with serial port */
CentralComputer::CentralComputer(const string &port)
    : m_comms(new SerialComm(port)),
      m_set_time_wall(0),
      m_time_synced(false)
{
    rxReset();

    if (!m_comms->isOpen())
    {
        cerr << "[CC] Serial port failed to open — running without LNC link\n";
        delete m_comms;
        m_comms = nullptr;
    }
    else
    {
        cout << "[CC] Live link established on " << port << "\n";
    }
}

CentralComputer::~CentralComputer()
{
    delete m_comms;
}

/* Move constructor */
CentralComputer::CentralComputer(CentralComputer &&other)
    : m_comms(other.m_comms),
      m_set_time_wall(other.m_set_time_wall),
      m_time_synced(other.m_time_synced),
      m_rx(other.m_rx)
{
    other.m_comms = nullptr;
}

/* Move assignment */
CentralComputer &CentralComputer::operator=(CentralComputer &&other)
{
    if (this != &other)
    {
        delete m_comms;
        m_comms          = other.m_comms;
        m_set_time_wall  = other.m_set_time_wall;
        m_time_synced    = other.m_time_synced;
        m_rx             = other.m_rx;
        other.m_comms    = nullptr;
    }

    return *this;
}

/* -----------------------------------------------------------------------
 * Public methods
 * --------------------------------------------------------------------- */

bool CentralComputer::isLive() const
{
    return m_comms != nullptr;
}

void CentralComputer::sendSetTime(const struct tm &t)
{
    uint8_t value[7];

    /* year: struct tm stores years since 1900, DS1307 wants 2-digit (26=2026)
     * So subtract 100 to convert 126 -> 26 */
    value[0] = (uint8_t)(t.tm_year - 100);
    value[1] = (uint8_t)(t.tm_mon + 1);    /* tm_mon is 0-based */
    value[2] = (uint8_t)t.tm_mday;
    value[3] = (uint8_t)(t.tm_wday + 1);   /* tm_wday: 0=Sun, DS1307: 1=Sun */
    value[4] = (uint8_t)t.tm_hour;
    value[5] = (uint8_t)t.tm_min;
    value[6] = (uint8_t)t.tm_sec;

    sendFrame(TAG_SET_TIME, value, 7U);

    /* Record wall-clock baseline for timestamp reconstruction */
    m_set_time_wall = time(nullptr);
    m_time_synced   = true;

    cout << "[CC-TX] SET_TIME sent\n";
}

void CentralComputer::sendSetConfig(uint8_t param_id,
                                    const uint8_t *value,
                                    uint8_t len)
{
    uint8_t buf[256];

    buf[0] = param_id;
    memcpy(buf + 1, value, len);
    sendFrame(TAG_SET_CONFIG, buf, (uint8_t)(len + 1U));

    cout << "[CC-TX] SET_CONFIG param_id=0x"
         << hex << (unsigned)param_id << dec << "\n";
}

void CentralComputer::processIncoming()
{
    uint8_t buf[64];
    int     n;

    if (m_comms == nullptr)
    {
        return;
    }

    n = m_comms->recv(buf, sizeof(buf));

    for (int i = 0; i < n; ++i)
    {
        rxFeedByte(buf[i]);
    }
}

/* -----------------------------------------------------------------------
 * Private — frame builder
 * --------------------------------------------------------------------- */

void CentralComputer::sendFrame(uint8_t tag,
                                const uint8_t *value,
                                uint8_t len)
{
    uint8_t  buf[259];
    uint8_t  chk;
    uint8_t  i;
    uint16_t frame_len;

    if (m_comms == nullptr)
    {
        return;
    }

    chk    = (uint8_t)(tag + len);
    buf[0] = COMM_SOF;
    buf[1] = tag;
    buf[2] = len;

    for (i = 0U; i < len; ++i)
    {
        buf[3U + i] = value[i];
        chk = (uint8_t)(chk + value[i]);
    }

    buf[3U + len] = chk;
    frame_len     = (uint16_t)(4U + len);

    m_comms->send(buf, (uint8_t)frame_len);
}

/* -----------------------------------------------------------------------
 * Private — RX state machine
 * --------------------------------------------------------------------- */

void CentralComputer::rxReset()
{
    m_rx.state          = RX_WAIT_SOF;
    m_rx.tag            = 0U;
    m_rx.len            = 0U;
    m_rx.value_idx      = 0U;
    m_rx.checksum_accum = 0U;
}

void CentralComputer::rxFeedByte(uint8_t byte)
{
    switch (m_rx.state)
    {
        case RX_WAIT_SOF:
            if (COMM_SOF == byte)
            {
                m_rx.state = RX_READ_TAG;
            }
            break;

        case RX_READ_TAG:
            m_rx.tag            = byte;
            m_rx.checksum_accum = byte;
            m_rx.state          = RX_READ_LEN;
            break;

        case RX_READ_LEN:
            m_rx.len            = byte;
            m_rx.checksum_accum = (uint8_t)(m_rx.checksum_accum + byte);
            m_rx.value_idx      = 0U;
            m_rx.state = (0U == byte) ? RX_READ_CHK : RX_READ_VALUE;
            break;

        case RX_READ_VALUE:
            m_rx.value[m_rx.value_idx] = byte;
            m_rx.checksum_accum = (uint8_t)(m_rx.checksum_accum + byte);
            ++m_rx.value_idx;

            if (m_rx.value_idx >= m_rx.len)
            {
                m_rx.state = RX_READ_CHK;
            }
            break;

        case RX_READ_CHK:
            if (byte == m_rx.checksum_accum)
            {
                rxDispatch(m_rx.tag, m_rx.value, m_rx.len);
            }
            rxReset();
            break;

        default:
            rxReset();
            break;
    }
}

void CentralComputer::rxDispatch(uint8_t tag,
                                 const uint8_t *value,
                                 uint8_t len)
{
    uint32_t ts_lnc;
    int16_t  temp;
    uint8_t  hum;
    uint16_t batt;
    uint16_t light;
    uint8_t  mode;
    uint8_t  ev_type;
    uint8_t  detail;
    time_t   wall;
    struct tm *wall_tm;
    char     time_buf[16];

    switch (tag)
    {
        case TAG_KEEPALIVE:
            if (len < 12U)
            {
                break;
            }

            /* Parse measurement block — little-endian, matches LNC write */
            ts_lnc = (uint32_t)value[0]
                   | ((uint32_t)value[1] << 8)
                   | ((uint32_t)value[2] << 16)
                   | ((uint32_t)value[3] << 24);

            temp  = (int16_t)((uint16_t)value[4] | ((uint16_t)value[5] << 8));
            hum   = value[6];
            batt  = (uint16_t)value[7]  | ((uint16_t)value[8]  << 8);
            light = (uint16_t)value[9]  | ((uint16_t)value[10] << 8);
            mode  = value[11];

            /* Reconstruct wall clock if time was synced */
            if (m_time_synced)
            {
                wall    = m_set_time_wall + (time_t)ts_lnc;
                wall_tm = localtime(&wall);
                strftime(time_buf, sizeof(time_buf), "%H:%M:%S", wall_tm);
            }
            else
            {
                snprintf(time_buf, sizeof(time_buf), "%lus", (unsigned long)ts_lnc);
            }

            cout << "[CC-RX] KEEPALIVE"
                 << " time:"  << time_buf
                 << " temp:"  << (int)temp  << "c"
                 << " hum:"   << (unsigned)hum << "%"
                 << " batt:"  << (unsigned)batt
                 << " light:" << (unsigned)light
                 << " mode:"  << (mode < 3 ? MODE_NAMES[mode] : "?")
                 << "\n";
            break;

        case TAG_EVENT:
            if (len < 6U)
            {
                break;
            }

            ev_type = value[0];
            detail  = value[1];

            ts_lnc = (uint32_t)value[2]
                   | ((uint32_t)value[3] << 8)
                   | ((uint32_t)value[4] << 16)
                   | ((uint32_t)value[5] << 24);

            if (m_time_synced)
            {
                wall    = m_set_time_wall + (time_t)ts_lnc;
                wall_tm = localtime(&wall);
                strftime(time_buf, sizeof(time_buf), "%H:%M:%S", wall_tm);
            }
            else
            {
                snprintf(time_buf, sizeof(time_buf), "%lus", (unsigned long)ts_lnc);
            }

            cout << "[CC-RX] EVENT"
                 << " time:" << time_buf
                 << " type:" << (ev_type < 6 ? EVENT_NAMES[ev_type] : "?");

            /* Print detail field meaning depends on event type */
            if (1U == ev_type)
            {
                cout << " new_mode:" << (detail < 3 ? MODE_NAMES[detail] : "?");
            }
            else if (2U == ev_type)
            {
                cout << " object:" << (detail == 0 ? "detected" : "cleared");
            }
            else if (5U == ev_type)
            {
                cout << " wd_reset:" << (detail ? "yes" : "no");
            }

            cout << "\n";
            break;

        case TAG_TIME_SYNC_REQ:
            /* LNC is requesting time — send current system time */
            cout << "[CC-RX] TIME_SYNC_REQ received — sending SET_TIME\n";
            {
                time_t   now = time(nullptr);
                struct tm *t = localtime(&now);
                sendSetTime(*t);
            }
            break;

        case TAG_GET_TIME:
            /* LNC is asking for current time as epoch seconds */
            {
                uint8_t  resp[4];
                uint32_t now_s = (uint32_t)time(nullptr);
                resp[0] = (uint8_t)(now_s & 0xFFU);
                resp[1] = (uint8_t)((now_s >> 8)  & 0xFFU);
                resp[2] = (uint8_t)((now_s >> 16) & 0xFFU);
                resp[3] = (uint8_t)((now_s >> 24) & 0xFFU);
                sendFrame(TAG_TIME_REPORT, resp, 4U);
            }
            break;

        default:
            cout << "[CC-RX] Unknown tag 0x"
                 << hex << (unsigned)tag << dec << " — ignored\n";
            break;
    }
}