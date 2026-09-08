#ifndef RESEARCH_SUBMARINE_H
#define RESEARCH_SUBMARINE_H

#include "submarine.h"
#include <string>
#include <vector>

class ResearchSubmarine : public Submarine
{
public:
    /**
     * @brief  Regular constructor. Calls Submarine(serial, name) in
     *         initialiser list, sets the initial research topic.
     * @param  serial  Unique serial number.
     * @param  name    Submarine name.
     * @param  topic   Initial research topic.
     */
    ResearchSubmarine(const std::string &serial,
                      const std::string &name,
                      const std::string &topic);

    /**
     * @brief  Override of Submarine's pure virtual. Prints serial, name,
     *         mission status, topic, and all researcher names.
     */
    void displayDetails() const override;

    /**
     * @brief  Override of Submarine's pure virtual. Prompts the user to
     *         update the research topic.
     */
    void updateMission() override;

    /**
     * @brief  Adds a researcher name to the list.
     * @param  name  Researcher's name — passed by const ref, no copy cost.
     */
    void addResearcher(const std::string &name);

    /**
     * @brief  Returns current research topic by const reference.
     */
    const std::string &getTopic() const;

private:
    std::vector<std::string> m_researchers;
    std::string              m_topic;
};

#endif /* RESEARCH_SUBMARINE_H */