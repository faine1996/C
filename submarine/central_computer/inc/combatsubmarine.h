#ifndef COMBAT_SUBMARINE_H
#define COMBAT_SUBMARINE_H

#include "submarine.h"
#include "centralcomputer.h"
#include "message.h"
#include <string>
#include <vector>

class CombatSubmarine : public Submarine
{
public:
    /**
     * @brief  Regular constructor. Creates a non-live combat submarine.
     *         CentralComputer defaults to nullptr comms.
     * @param  serial     Unique serial number.
     * @param  name       Submarine name.
     * @param  commander  Commander name.
     * @param  crew       Number of crew members.
     */
    CombatSubmarine(const std::string &serial,
                    const std::string &name,
                    const std::string &commander,
                    int crew);

    /**
     * @brief  Overloaded constructor. Creates the live combat submarine.
     *         Passes port to CentralComputer which opens the serial link.
     *         Used only for the first CombatSubmarine added to the fleet.
     * @param  serial     Unique serial number.
     * @param  name       Submarine name.
     * @param  commander  Commander name.
     * @param  crew       Number of crew members.
     * @param  port       Serial port path e.g. "/dev/ttyACM0".
     */
    CombatSubmarine(const std::string &serial,
                    const std::string &name,
                    const std::string &commander,
                    int crew,
                    const std::string &port);

    /**
     * @brief  Override of Submarine's pure virtual. Prints all combat
     *         submarine details including mission, companions, history.
     */
    void displayDetails() const override;

    /**
     * @brief  Override of Submarine's pure virtual. Prompts user to
     *         update mission description or commander name.
     */
    void updateMission() override;

    /**
     * @brief  Assigns a mission. Calls Submarine::assignMission() to set
     *         the flag, stores the description, records in mission history.
     * @param  desc  Mission description string.
     */
    void assignMission(const std::string &desc);

    /**
     * @brief  Ends the current mission. Calls Submarine::endMission() to
     *         clear the flag, clears description and companions.
     *         Mission history is preserved.
     */
    void endMission();

    /**
     * @brief  Adds a non-owning pointer to a companion submarine.
     *         Guards against nullptr and duplicates.
     * @param  sub  Pointer to the companion CombatSubmarine.
     */
    void addCompanion(CombatSubmarine *sub);

    /**
     * @brief  Stores a received message with a non-owning pointer to sender.
     * @param  content  Message text.
     * @param  sender   Pointer to the sending CombatSubmarine.
     */
    void receiveMessage(const std::string &content,
                        const CombatSubmarine *sender);

    /**
     * @brief  Prints all received messages including sender name.
     */
    void displayMessages() const;

    /**
     * @brief  Returns a reference to the CentralComputer member.
     *         Reference not pointer — m_computer always exists.
     *         Used by Menu to call processIncoming() each loop iteration.
     */
    CentralComputer &getComputer();

    /**
     * @brief  Returns commander name by const reference.
     */
    const std::string &getCommander() const;

    /**
     * @brief  Returns crew count by value — int is cheap to copy.
     */
    int getCrew() const;

    /**
     * @brief  Returns current mission description by const reference.
     */
    const std::string &getMissionDesc() const;

    /**
     * @brief  Returns full mission history vector by const reference.
     */
    const std::vector<std::string> &getMissionHistory() const;

    /**
    * @brief  Returns companions vector by const reference.
    *         Used by Menu::sendMessage to verify mission membership.
    */
    const std::vector<CombatSubmarine *> &getCompanions() const;

private:
    CentralComputer                m_computer;
    std::string                    m_mission_desc;
    std::string                    m_commander;
    int                            m_crew;
    std::vector<CombatSubmarine *> m_companions;
    std::vector<Message>           m_messages;
    std::vector<std::string>       m_mission_history;
};

#endif /* COMBAT_SUBMARINE_H */