#include "menu.h"
#include <iostream>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;

Menu::Menu(const string &host, uint16_t port)
    : m_station(host, port),
      m_running(true)
{
}

void Menu::printMenu() const
{
    cout << "\n=== Ground Station ===\n";
    cout << "1. Request data range\n";
    cout << "2. Request events range\n";
    cout << "3. Exit\n";
}

uint32_t Menu::promptForEpoch(const string &prompt) const
{
    string    input;
    struct tm t;

    memset(&t, 0, sizeof(t));

    cout << prompt << " (YYYY-MM-DD HH:MM:SS): ";
    getline(cin, input);

    istringstream iss(input);
    iss >> get_time(&t, "%Y-%m-%d %H:%M:%S");

    if (iss.fail())
    {
        cout << "Could not parse that time.\n";
        return 0U;
    }

    return static_cast<uint32_t>(timegm(&t));
}

void Menu::requestData()
{
    uint32_t start = promptForEpoch("Start time");
    uint32_t end   = promptForEpoch("End time");

    if (!m_station.isConnected())
    {
        cout << "Not connected to the Central Computer.\n";
        return;
    }

    auto records = m_station.requestDataRange(start, end);

    cout << records.size() << " record(s) received:\n";

    for (const auto &r : records)
    {
        time_t     wall   = static_cast<time_t>(r.timestamp);
        struct tm *wallTm = gmtime(&wall);
        char       buf[32];

        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", wallTm);

        cout << "  " << buf
             << "  temp:"  << r.temp << "c"
             << "  hum:"   << static_cast<unsigned>(r.humidity) << "%"
             << "  batt:"  << r.battery
             << "  light:" << r.light
             << "  mode:"  << static_cast<unsigned>(r.mode)
             << "\n";
    }
}

void Menu::requestEvents()
{
    uint32_t start = promptForEpoch("Start time");
    uint32_t end   = promptForEpoch("End time");

    if (!m_station.isConnected())
    {
        cout << "Not connected to the Central Computer.\n";
        return;
    }

    auto records = m_station.requestEventsRange(start, end);

    cout << records.size() << " record(s) received:\n";

    for (const auto &r : records)
    {
        time_t     wall   = static_cast<time_t>(r.timestamp);
        struct tm *wallTm = gmtime(&wall);
        char       buf[32];

        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", wallTm);

        cout << "  " << buf
             << "  type:"   << static_cast<unsigned>(r.eventType)
             << "  detail:" << static_cast<unsigned>(r.detail)
             << "\n";
    }
}

void Menu::exitSystem()
{
    m_running = false;
    cout << "Exiting.\n";
}

void Menu::run()
{
    string input;
    int    choice;

    if (!m_station.isConnected())
    {
        cout << "Warning: not connected to the Central Computer.\n";
    }

    while (m_running)
    {
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
            case 1: requestData();   break;
            case 2: requestEvents(); break;
            case 3: exitSystem();    break;
            default: cout << "Invalid choice.\n"; break;
        }
    }
}