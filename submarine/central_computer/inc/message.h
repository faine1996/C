#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <ostream>

class CombatSubmarine;

class Message
{
public:
    /**
     * @brief  Constructor. Stores message content and a non-owning pointer
     *         to the sender. The sender is not deleted by this class.
     * @param  content  The message text.
     * @param  sender   Pointer to the CombatSubmarine that sent this message.
     */
    Message(const std::string &content, const CombatSubmarine *sender);

    /**
     * @brief  Returns the message content string by const reference.
     * @return const std::string & — read-only reference, no copy made.
     */
    const std::string &getContent() const;

    /**
     * @brief  Returns a pointer to the sending submarine.
     *         Pointer because the sender could theoretically be nullptr.
     * @return const CombatSubmarine * — non-owning, do not delete.
     */
    const CombatSubmarine *getSender() const;

    /**
     * @brief  Stream insertion operator. Prints content and sender name.
     */
    friend std::ostream &operator<<(std::ostream &os, const Message &m);

private:
    std::string            m_content;
    const CombatSubmarine *m_sender;
};

#endif /* MESSAGE_H */