#ifndef SUBMARINE_H
#define SUBMARINE_H

#include <string>
#include <ostream>

class Submarine
{
public:
    /**
     * @brief  Regular constructor. Every submarine has a serial number
     *         and a name. Starts not on a mission.
     * @param  serial  Unique serial number string.
     * @param  name    Submarine name string.
     */
    Submarine(const std::string &serial, const std::string &name);

    /**
     * @brief  Virtual destructor. Required because we delete derived
     *         objects through a Submarine* pointer. Without this, only
     *         the base destructor would run — derived members would leak.
     */
    virtual ~Submarine() = default;

    /**
     * @brief  Pure virtual. Every subclass must print its own details.
     *         Making it pure (= 0) means Submarine cannot be instantiated
     *         directly — only ResearchSubmarine and CombatSubmarine can.
     */
    virtual void displayDetails() const = 0;

    /**
     * @brief  Pure virtual. Every subclass handles mission updates
     *         differently so there is no sensible base implementation.
     */
    virtual void updateMission() = 0;

    /**
     * @brief  Returns serial number by const reference — no copy, read-only.
     */
    const std::string &getSerial() const;

    /**
     * @brief  Returns name by const reference — no copy, read-only.
     */
    const std::string &getName() const;

    /**
     * @brief  Returns true if submarine is currently assigned a mission.
     */
    bool isOnMission() const;

    /**
     * @brief  Marks submarine as on a mission.
     */
    void assignMission();

    /**
     * @brief  Marks submarine as available. Clears mission flag only —
     *         subclasses handle clearing their own mission data.
     */
    void endMission();

    /**
     * @brief  Stream insertion operator. Prints serial, name, mission status.
     */
    friend std::ostream &operator<<(std::ostream &os, const Submarine &s);

private:
    std::string m_serial;
    std::string m_name;
    bool        m_on_mission;
};

#endif /* SUBMARINE_H */