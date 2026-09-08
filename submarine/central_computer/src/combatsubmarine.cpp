#include "combatsubmarine.h"
#include "centralcomputer.h"
#include <iostream>

using namespace std;

CombatSubmarine::CombatSubmarine(const string &serial,
                                 const string &name,
                                 const string &commander,
                                 int crew)
    : Submarine(serial, name),
      m_commander(commander),
      m_crew(crew),
      m_mission_desc(""),
      m_computer()
{
}

CombatSubmarine::CombatSubmarine(const string &serial,
                                 const string &name,
                                 const string &commander,
                                 int crew,
                                 const string &port)
    : Submarine(serial, name),
      m_commander(commander),
      m_crew(crew),
      m_mission_desc(""),
      m_computer(port)
{
}

void CombatSubmarine::displayDetails() const
{
    cout << *this << "\n";
    cout << "  Type      : Combat\n";
    cout << "  Commander : " << m_commander << "\n";
    cout << "  Crew      : " << m_crew << "\n";
    cout << "  Mission   : "
         << (m_mission_desc.empty() ? "none" : m_mission_desc) << "\n";
    cout << "  Live link : " << (m_computer.isLive() ? "yes" : "no") << "\n";

    cout << "  Companions (" << m_companions.size() << "):\n";
    for (const CombatSubmarine *c : m_companions)
    {
        cout << "    - " << c->getName()
             << " [" << c->getSerial() << "]\n";
    }

    cout << "  Mission history (" << m_mission_history.size() << "):\n";
    for (const string &h : m_mission_history)
    {
        cout << "    - " << h << "\n";
    }
}

void CombatSubmarine::updateMission()
{
    string input;
    int    choice;

    if (!isOnMission())
    {
        cout << "This submarine is not currently on a mission.\n";
        return;
    }

    cout << "1. Update mission description\n";
    cout << "2. Update commander\n";
    cout << "Choice: ";
    getline(cin, input);
    choice = input.empty() ? 0 : stoi(input);

    switch (choice)
    {
        case 1:
            cout << "Current description: " << m_mission_desc << "\n";
            cout << "New description: ";
            getline(cin, m_mission_desc);
            cout << "Mission description updated.\n";
            break;

        case 2:
            cout << "Current commander: " << m_commander << "\n";
            cout << "New commander: ";
            getline(cin, m_commander);
            cout << "Commander updated.\n";
            break;

        default:
            cout << "Invalid choice.\n";
            break;
    }
}

void CombatSubmarine::assignMission(const string &desc)
{
    Submarine::assignMission();
    m_mission_desc = desc;
    m_mission_history.push_back(desc);
}

void CombatSubmarine::endMission()
{
    Submarine::endMission();
    m_mission_desc = "";
    m_companions.clear();
}

void CombatSubmarine::addCompanion(CombatSubmarine *sub)
{
    if (sub == nullptr)
    {
        return;
    }

    for (const CombatSubmarine *c : m_companions)
    {
        if (c == sub)
        {
            cout << "Already a companion.\n";
            return;
        }
    }

    m_companions.push_back(sub);
}

void CombatSubmarine::receiveMessage(const string &content,
                                     const CombatSubmarine *sender)
{
    m_messages.push_back(Message(content, sender));
}

void CombatSubmarine::displayMessages() const
{
    if (m_messages.empty())
    {
        cout << "No messages received.\n";
        return;
    }

    cout << "Messages (" << m_messages.size() << "):\n";

    for (const Message &msg : m_messages)
    {
        cout << "  " << msg << "\n";
    }
}

CentralComputer &CombatSubmarine::getComputer()
{
    return m_computer;
}

const string &CombatSubmarine::getCommander() const
{
    return m_commander;
}

int CombatSubmarine::getCrew() const
{
    return m_crew;
}

const string &CombatSubmarine::getMissionDesc() const
{
    return m_mission_desc;
}

const vector<string> &CombatSubmarine::getMissionHistory() const
{
    return m_mission_history;
}

const vector<CombatSubmarine *> &CombatSubmarine::getCompanions() const
{
    return m_companions;
}