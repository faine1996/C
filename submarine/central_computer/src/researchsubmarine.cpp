#include "researchsubmarine.h"
#include <iostream>

using namespace std;

ResearchSubmarine::ResearchSubmarine(const string &serial,
                                     const string &name,
                                     const string &topic)
    : Submarine(serial, name),
      m_topic(topic)
{
}

void ResearchSubmarine::displayDetails() const
{
    cout << *this << "\n";
    cout << "  Type   : Research\n";
    cout << "  Topic  : " << m_topic << "\n";
    cout << "  Researchers (" << m_researchers.size() << "):\n";

    for (const string &r : m_researchers)
    {
        cout << "    - " << r << "\n";
    }
}

void ResearchSubmarine::updateMission()
{
    string new_topic;

    cout << "Current topic: " << m_topic << "\n";
    cout << "Enter new research topic: ";
    getline(cin, new_topic);

    if (!new_topic.empty())
    {
        m_topic = new_topic;
        cout << "Topic updated.\n";
    }
    else
    {
        cout << "No change made.\n";
    }
}

void ResearchSubmarine::addResearcher(const string &name)
{
    m_researchers.push_back(name);
}

const string &ResearchSubmarine::getTopic() const
{
    return m_topic;
}