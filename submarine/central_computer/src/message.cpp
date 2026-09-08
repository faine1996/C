#include "message.h"
#include "combatsubmarine.h"
#include <ostream>

using namespace std;

Message::Message(const string &content, const CombatSubmarine *sender)
    : m_content(content),
      m_sender(sender)
{
}

const string &Message::getContent() const
{
    return m_content;
}

const CombatSubmarine *Message::getSender() const
{
    return m_sender;
}

ostream &operator<<(ostream &os, const Message &m)
{
    os << "Message: \"" << m.m_content << "\"";

    if (m.m_sender != nullptr)
    {
        os << " (from: " << m.m_sender->getName() << ")";
    }
    else
    {
        os << " (sender unknown)";
    }

    return os;
}