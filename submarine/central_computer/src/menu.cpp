#include "menu.h"
#include <iostream>
#include <string>

using namespace std;

static const uint16_t GS_LINK_PORT = 5555;

Menu::Menu()
    : m_live_sub(nullptr),
      m_running(true)
{
}

Menu::~Menu()
{
    for (Submarine *sub : m_fleet)
    {
        delete sub;
    }
}

Menu::Menu(Menu &&other)
    : m_fleet(move(other.m_fleet)),
      m_live_sub(other.m_live_sub),
      m_running(other.m_running),
      m_gsLink(move(other.m_gsLink))
{
    other.m_live_sub = nullptr;
    other.m_running  = false;
}

Menu &Menu::operator=(Menu &&other)
{
    if (this != &other)
    {
        for (Submarine *sub : m_fleet)
        {
            delete sub;
        }

        m_fleet          = move(other.m_fleet);
        m_live_sub       = other.m_live_sub;
        m_running        = other.m_running;
        m_gsLink         = move(other.m_gsLink);
        other.m_live_sub = nullptr;
        other.m_running  = false;
    }

    return *this;
}

void Menu::printMenu() const
{
    cout << "\n=== Submarine Fleet Management ===\n";
    cout << "1.  Add submarine\n";
    cout << "2.  Display all submarines\n";
    cout << "3.  Find submarine by serial\n";
    cout << "4.  Assign mission\n";
    cout << "5.  Update mission details\n";
    cout << "6.  End mission\n";
    cout << "7.  Add companion submarine\n";
    cout << "8.  Send message\n";
    cout << "9.  Display messages\n";
    cout << "10. Exit\n";
}

void Menu::run()
{
    string input;
    int    choice;

    while (m_running)
    {
        if (m_live_sub != nullptr)
        {
            m_live_sub->getComputer().processIncoming();
        }

        printMenu();

        cout << "Choice: ";
        getline(cin, input);

        if (input.empty())
        {
            continue;
        }

        choice = stoi(input);

        switch (choice)
        {
            case 1:  addSubmarine();    break;
            case 2:  displayAll();      break;
            case 3:  findBySerial();    break;
            case 4:  assignMission();   break;
            case 5:  updateMission();   break;
            case 6:  endMission();      break;
            case 7:  addCompanion();    break;
            case 8:  sendMessage();     break;
            case 9:  displayMessages(); break;
            case 10: exitSystem();      break;
            default:
                cout << "Invalid choice.\n";
                break;
        }
    }
}

void Menu::addSubmarine()
{
    string serial;
    string name;
    string type_input;
    int    type;

    cout << "Serial number: ";
    getline(cin, serial);

    cout << "Name: ";
    getline(cin, name);

    cout << "Type (1=Research, 2=Combat): ";
    getline(cin, type_input);
    type = type_input.empty() ? 0 : stoi(type_input);

    if (type == 1)
    {
        string topic;
        cout << "Research topic: ";
        getline(cin, topic);

        m_fleet.push_back(new ResearchSubmarine(serial, name, topic));
        cout << "Research submarine added.\n";
    }
    else if (type == 2)
    {
        string commander;
        string crew_input;
        int    crew;

        cout << "Commander: ";
        getline(cin, commander);

        cout << "Crew count: ";
        getline(cin, crew_input);
        crew = crew_input.empty() ? 0 : stoi(crew_input);

        if (m_live_sub == nullptr)
        {
            string port;
            cout << "Serial port (e.g. /dev/ttyACM0): ";
            getline(cin, port);

            CombatSubmarine *sub = new CombatSubmarine(serial, name,
                                                       commander, crew,
                                                       port);
            m_fleet.push_back(sub);

            if (sub->getComputer().isLive())
            {
                m_live_sub = sub;
                m_gsLink   = make_unique<GroundStationLink>(sub->getComputer(), GS_LINK_PORT);
                cout << "Combat submarine added as live unit.\n";
            }
            else
            {
                cout << "Combat submarine added (no live link).\n";
            }
        }
        else
        {
            m_fleet.push_back(new CombatSubmarine(serial, name,
                                                  commander, crew));
            cout << "Combat submarine added.\n";
        }
    }
    else
    {
        cout << "Invalid type.\n";
    }
}

void Menu::displayAll() const
{
    if (m_fleet.empty())
    {
        cout << "Fleet is empty.\n";
        return;
    }

    cout << "\n--- Fleet (" << m_fleet.size() << " submarines) ---\n";

    for (const Submarine *sub : m_fleet)
    {
        sub->displayDetails();
        cout << "\n";
    }
}

void Menu::findBySerial() const
{
    string serial;

    cout << "Serial number: ";
    getline(cin, serial);

    Submarine *sub = findSub(serial);

    if (sub == nullptr)
    {
        cout << "Submarine not found.\n";
        return;
    }

    sub->displayDetails();
}

void Menu::assignMission()
{
    string serial;
    string desc;

    cout << "Serial number: ";
    getline(cin, serial);

    Submarine *sub = findSub(serial);

    if (sub == nullptr)
    {
        cout << "Submarine not found.\n";
        return;
    }

    if (sub->isOnMission())
    {
        cout << "Submarine already on a mission.\n";
        return;
    }

    cout << "Mission description: ";
    getline(cin, desc);

    CombatSubmarine *combat = dynamic_cast<CombatSubmarine *>(sub);

    if (combat != nullptr)
    {
        combat->assignMission(desc);
    }
    else
    {
        sub->assignMission();
    }

    cout << "Mission assigned.\n";
}

void Menu::updateMission()
{
    string serial;

    cout << "Serial number: ";
    getline(cin, serial);

    Submarine *sub = findSub(serial);

    if (sub == nullptr)
    {
        cout << "Submarine not found.\n";
        return;
    }

    sub->updateMission();
}

void Menu::endMission()
{
    string serial;

    cout << "Serial number: ";
    getline(cin, serial);

    Submarine *sub = findSub(serial);

    if (sub == nullptr)
    {
        cout << "Submarine not found.\n";
        return;
    }

    if (!sub->isOnMission())
    {
        cout << "Submarine is not on a mission.\n";
        return;
    }

    CombatSubmarine *combat = dynamic_cast<CombatSubmarine *>(sub);

    if (combat != nullptr)
    {
        combat->endMission();
    }
    else
    {
        sub->endMission();
    }

    cout << "Mission ended.\n";
}

void Menu::addCompanion()
{
    string serial_a;
    string serial_b;

    cout << "Combat submarine serial: ";
    getline(cin, serial_a);

    CombatSubmarine *sub_a = findCombatSub(serial_a);

    if (sub_a == nullptr)
    {
        cout << "Combat submarine not found.\n";
        return;
    }

    cout << "Companion serial: ";
    getline(cin, serial_b);

    CombatSubmarine *sub_b = findCombatSub(serial_b);

    if (sub_b == nullptr)
    {
        cout << "Companion not found.\n";
        return;
    }

    if (sub_a == sub_b)
    {
        cout << "A submarine cannot be its own companion.\n";
        return;
    }

    sub_a->addCompanion(sub_b);
    cout << "Companion added.\n";
}

void Menu::sendMessage()
{
    string serial_from;
    string serial_to;
    string content;
    bool   is_companion;
    size_t i;

    cout << "Sender serial: ";
    getline(cin, serial_from);

    CombatSubmarine *sender = findCombatSub(serial_from);

    if (sender == nullptr)
    {
        cout << "Sender not found.\n";
        return;
    }

    cout << "Recipient serial: ";
    getline(cin, serial_to);

    CombatSubmarine *recipient = findCombatSub(serial_to);

    if (recipient == nullptr)
    {
        cout << "Recipient not found.\n";
        return;
    }

    if (sender == recipient)
    {
        cout << "Cannot send a message to yourself.\n";
        return;
    }

    is_companion = false;

    for (i = 0; i < sender->getCompanions().size(); ++i)
    {
        if (sender->getCompanions()[i] == recipient)
        {
            is_companion = true;
            break;
        }
    }

    if (!is_companion)
    {
        cout << "Recipient is not a companion in the same mission.\n";
        return;
    }

    cout << "Message: ";
    getline(cin, content);

    recipient->receiveMessage(content, sender);
    cout << "Message sent.\n";
}

void Menu::displayMessages() const
{
    string serial;

    cout << "Serial number: ";
    getline(cin, serial);

    CombatSubmarine *sub = findCombatSub(serial);

    if (sub == nullptr)
    {
        cout << "Combat submarine not found.\n";
        return;
    }

    sub->displayMessages();
}

void Menu::exitSystem()
{
    cout << "Exiting system.\n";
    m_running = false;
}

Submarine *Menu::findSub(const string &serial) const
{
    for (Submarine *sub : m_fleet)
    {
        if (sub->getSerial() == serial)
        {
            return sub;
        }
    }

    return nullptr;
}

CombatSubmarine *Menu::findCombatSub(const string &serial) const
{
    Submarine *sub = findSub(serial);

    if (sub == nullptr)
    {
        return nullptr;
    }

    return dynamic_cast<CombatSubmarine *>(sub);
}