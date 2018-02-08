#ifndef MAS_SIMULLATION_H
#define MAS_SIMULLATION_H

#include <memory>
#include "../Misc/Bid.h"
#include "../Agents/Agent.h"
#include "../Misc/Logger.h"

class Simulation
{
    Logger &d_logger;
    std::vector<std::shared_ptr<Agent>> d_agents;
    uint d_activeAgent = 0;

    uint d_numDiceTotal = 15;
    uint d_numDeadAgents = 0;

public:
    enum class TYPE
    {
        SIMULATION,
        GAME
    };

    Simulation(Logger &logger, TYPE type = TYPE::SIMULATION);

    void run();

    Agent const &agent(uint idx) const;
    Agent const &activeAgent() const;
    uint numDeadAgents() const;
    uint numDiceTotal() const;

private:
    void takeTurn();
    bool isEndGame() const;
    uint numDiceWithEyesInGame(uint eyes);
    bool setNextActiveAgent(Bid const &bid, bool call);
    void processCalledBid(Bid const &bid);
    void findNextAliveAgent();
    void showBidToEveryone(Bid const &bid);
};


#endif //MAS_SIMULLATION_H
