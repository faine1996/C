#ifndef MENU_H
#define MENU_H

#include "submarine.h"
#include "combatsubmarine.h"
#include "researchsubmarine.h"
#include "groundstationlink.h"
#include <vector>
#include <string>
#include <memory>


class Menu
{
public:
    /**
     * @brief  Default constructor. Initialises empty fleet, sets
     *         m_live_sub to nullptr and m_running to true.
     */
    Menu();

    /**
     * @brief  Destructor. Deletes every Submarine pointer in m_fleet.
     *         The vector itself cleans up automatically.
     */
    ~Menu();

    /**
     * @brief  Copy constructor — deleted. Owning a fleet of polymorphic
     *         pointers makes copy semantically unclear.
     */
    Menu(const Menu &)            = delete;

    /**
     * @brief  Copy assignment — deleted. Same reason.
     */
    Menu &operator=(const Menu &) = delete;

    /**
     * @brief  Move constructor. Transfers fleet vector and live sub pointer.
     *         Sets other.m_live_sub to nullptr after transfer.
     */
    Menu(Menu &&other);

    /**
     * @brief  Move assignment. Deletes existing fleet, then transfers.
     */
    Menu &operator=(Menu &&other);

    /**
     * @brief  Main loop. Prints menu, reads user choice, dispatches to
     *         private methods. Calls processIncoming() each iteration
     *         if m_live_sub is not nullptr.
     */
    void run();

private:
    private:
    std::vector<Submarine *> m_fleet;
    CombatSubmarine         *m_live_sub;
    bool                     m_running;
    std::unique_ptr<GroundStationLink> m_gsLink;

    /* ---- menu option methods ---- */

    /** @brief Option 1 — add a new submarine to the fleet. */
    void addSubmarine();

    /** @brief Option 2 — display all submarines. */
    void displayAll() const;

    /** @brief Option 3 — find and display one submarine by serial. */
    void findBySerial() const;

    /** @brief Option 4 — assign a mission to a submarine. */
    void assignMission();

    /** @brief Option 5 — update a submarine's mission details. */
    void updateMission();

    /** @brief Option 6 — end a submarine's mission. */
    void endMission();

    /** @brief Option 7 — associate companion combat submarines. */
    void addCompanion();

    /** @brief Option 8 — send a message between combat submarines. */
    void sendMessage();

    /** @brief Option 9 — display messages received by a submarine. */
    void displayMessages() const;

    /** @brief Option 10 — set m_running to false to exit run() loop. */
    void exitSystem();

    /**
     * @brief  Searches m_fleet by serial number.
     * @return Submarine* if found, nullptr if not.
     */
    Submarine *findSub(const std::string &serial) const;

    /**
     * @brief  Searches m_fleet by serial, returns only if CombatSubmarine.
     *         Uses dynamic_cast — returns nullptr if found but wrong type.
     * @return CombatSubmarine* if found and correct type, nullptr otherwise.
     */
    CombatSubmarine *findCombatSub(const std::string &serial) const;

    /**
     * @brief  Prints the main menu options to stdout.
     */
    void printMenu() const;
};

#endif /* MENU_H */