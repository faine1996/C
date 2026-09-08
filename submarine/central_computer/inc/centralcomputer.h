#ifndef CENTRAL_COMPUTER_H
#define CENTRAL_COMPUTER_H

#include "serialcomm.h"
#include "uart_framer.h"
#include <string>
#include <cstdint>
#include <ctime>
#include <mutex>
#include <functional>

class CentralComputer
{
public:
    /**
     * @brief  Default constructor. Sets m_comms = nullptr.
     *         Used for all non-live CombatSubmarines.
     */
    CentralComputer();

    /**
     * @brief  Overloaded constructor. Creates a live SerialComm on the
     *         given port. Used only for the first CombatSubmarine added.
     *         explicit prevents accidental string-to-CC implicit conversion.
     * @param  port  Device path e.g. "/dev/ttyACM0".
     */
    explicit CentralComputer(const std::string &port);

    /**
     * @brief  Destructor. Deletes m_comms (safe if nullptr).
     */
    ~CentralComputer();

    /**
     * @brief  Copy constructor — deleted. Cannot duplicate a live serial port.
     */
    CentralComputer(const CentralComputer &)            = delete;

    /**
     * @brief  Copy assignment — deleted. Same reason.
     */
    CentralComputer &operator=(const CentralComputer &) = delete;

    /**
     * @brief  Move constructor. Transfers m_comms pointer ownership.
     *         Sets other.m_comms = nullptr so other's destructor skips delete.
     */
    CentralComputer(CentralComputer &&other);

    /**
     * @brief  Move assignment. Deletes existing m_comms, then transfers
     *         other.m_comms, sets other.m_comms = nullptr.
     */
    CentralComputer &operator=(CentralComputer &&other);

    /**
     * @brief  Returns true if this CC has a live serial connection.
     */
    bool isLive() const;

    /**
     * @brief  Sends a SET_TIME frame to the LNC. Packs the 7 broken-down
     *         time fields from t into the TAG_SET_TIME (0x21) value bytes.
     * @param  t  Broken-down time (year offset from 1900, so subtract 100
     *            to get 2-digit year matching DS1307 format).
     */
    void sendSetTime(const struct tm &t);

    /**
     * @brief  Sends a SET_CONFIG frame to the LNC.
     * @param  param_id  Config parameter ID byte.
     * @param  value     Pointer to parameter value bytes.
     * @param  len       Number of value bytes.
     */
    void sendSetConfig(uint8_t param_id,
                       const uint8_t *value,
                       uint8_t len);
    
    /**
     * @brief  Sends a GET_DATA_RANGE frame to the LNC, requesting
     *         measurement records whose epoch timestamp falls within
     *         [start, end].
     * @param  start  Range start, inclusive, epoch seconds.
     * @param  end    Range end, inclusive, epoch seconds.
     */
    void sendGetDataRange(uint32_t start, uint32_t end);

    /**
     * @brief  Sends a GET_EVENTS_RANGE frame to the LNC, requesting event
     *         records whose epoch timestamp falls within [start, end].
     * @param  start  Range start, inclusive, epoch seconds.
     * @param  end    Range end, inclusive, epoch seconds.
     */
    void sendGetEventsRange(uint32_t start, uint32_t end);


    /**
     * @brief  Polls the serial port for incoming bytes and feeds them into
     *         the TLV RX state machine. Prints decoded frames to stdout.
     *         Reconstructs wall-clock time from stored set-time baseline
     *         to display human-readable timestamps.
     *         Call once per menu loop iteration.
     */
    void processIncoming();

        /**
     * @brief  Returns the mutex guarding the LNC serial link, shared with
     *         any concurrent Ground Station relay activity. Callers must
     *         hold this lock for the duration of any sendX()/
     *         processIncoming() call.
     */
    std::mutex &uartMutex();

    /**
     * @brief  Callback signature for one relayed DATA_ITEM/EVENT_ITEM frame.
     */
    using RangeItemHandler = std::function<void(uint8_t tag, const uint8_t *value, uint8_t len)>;

    /**
     * @brief  Registers a callback invoked, instead of the normal print,
     *         for each DATA_ITEM/EVENT_ITEM frame received while a Ground
     *         Station range request is in flight. Pass an empty
     *         std::function to return to normal print behaviour.
     */
    void setRangeItemHandler(RangeItemHandler handler);


private:
    SerialComm *m_comms;

    /* Baseline for wall-clock reconstruction:
     * m_set_time_wall  = system time when SET_TIME was sent to LNC
     * m_set_time_tick  = LNC uptime seconds at that moment (from response) */
    time_t m_set_time_wall;
    bool   m_time_synced;

        /* TLV byte framing (SOF+checksum) — decodes incoming bytes into
     * frames and invokes rxDispatch via the registered handler. */
    tlv::UartFramer m_framer;


    /**
     * @brief  Builds and sends one TLV frame: SOF + tag + len + value + chk.
     */
    void sendFrame(uint8_t tag, const uint8_t *value, uint8_t len);

    /**
     * @brief  Called when a complete valid frame has been received.
     *         Decodes and prints the frame contents.
     */
    void rxDispatch(uint8_t tag, const uint8_t *value, uint8_t len);

    std::mutex       m_uartMutex;
    RangeItemHandler m_rangeItemHandler;

};

#endif /* CENTRAL_COMPUTER_H */