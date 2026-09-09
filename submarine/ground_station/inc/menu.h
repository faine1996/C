#ifndef MENU_H
#define MENU_H

#include "groundstation.h"
#include <string>

class Menu
{
public:
    /**
     * @brief  Constructor. Connects to the Central Computer at host:port.
     * @param  host  CC hostname/IP.
     * @param  port  CC's Ground Station TCP port.
     */
    Menu(const std::string &host, uint16_t port);

    /**
     * @brief  Main loop. Prints menu, reads user choice, dispatches to
     *         private methods, until the user chooses to exit.
     */
    void run();

private:
    GroundStation m_station;
    bool          m_running;

    /** @brief Option 1 — request and display a measurement data range. */
    void requestData();

    /** @brief Option 2 — request and display an event range. */
    void requestEvents();

    /** @brief Option 3 — set m_running to false to exit run() loop. */
    void exitSystem();

    /**
     * @brief  Prompts for a human-readable "YYYY-MM-DD HH:MM:SS" time and
     *         converts it to Unix epoch seconds via timegm (UTC).
     * @param  prompt  Text to display before reading input.
     * @return Epoch seconds, or 0 if the input could not be parsed.
     */
    uint32_t promptForEpoch(const std::string &prompt) const;

    /**
     * @brief  Prints the main menu options to stdout.
     */
    void printMenu() const;
};

#endif // MENU_H