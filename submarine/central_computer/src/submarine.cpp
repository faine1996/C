#include "submarine.h"
#include <ostream>

using namespace std;

Submarine::Submarine(const string &serial, const string &name)
    : m_serial(serial),
      m_name(name),
      m_on_mission(false)
{
}

const string &Submarine::getSerial() const
{
    return m_serial;
}

const string &Submarine::getName() const
{
    return m_name;
}

bool Submarine::isOnMission() const
{
    return m_on_mission;
}

void Submarine::assignMission()
{
    m_on_mission = true;
}

void Submarine::endMission()
{
    m_on_mission = false;
}

ostream &operator<<(ostream &os, const Submarine &s)
{
    os << "[" << s.m_serial << "] "
       << s.m_name
       << " — " << (s.m_on_mission ? "on mission" : "available");
    return os;
}