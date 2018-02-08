#include "Simulation.h"
#include "../Agents/ActiveAgent.h"
#include <assert.h>

Simulation::Simulation(Logger &logger, TYPE type)
    :
    d_logger(logger)
{
    d_logger.log("Created Agents", Logger::TYPE::ACTION);
    // The following is correct, ignore editor warnings!

    if (type == TYPE::SIMULATION)
        d_agents.emplace_back(new Agent(0, d_logger));
    if (type == TYPE::GAME)
        d_agents.emplace_back(new ActiveAgent(0, d_logger));
    d_agents.emplace_back(new Agent(1, d_logger));
    d_agents.emplace_back(new Agent(2, d_logger));

    assert(d_agents.size() == 3);
}

// --- Public functions

void Simulation::run()
{
    d_logger.log("START simulation", Logger::TYPE::ACTION);
    d_logger.flush();

    while (true)
    {
        if (isEndGame())
            break;

        takeTurn();
    }

    d_logger.logWin(d_agents[d_activeAgent]->id());
    d_logger.log("END simulation", Logger::TYPE::ACTION);
    d_logger.flush();
}


Agent const &Simulation::agent(uint idx) const
{
    return *d_agents.at(idx);
}

Agent const &Simulation::activeAgent() const
{
    return *d_agents.at(d_activeAgent);
}

uint Simulation::numDeadAgents() const
{
    return d_numDeadAgents;
}

uint Simulation::numDiceTotal() const
{
    return d_numDiceTotal;
}

// --- Private functions

void Simulation::takeTurn()
{
    d_logger.logStartRound(d_agents);

    while (true)
    {
        d_logger.logStartTurn(*d_agents[d_activeAgent]);
        bool call = false;
        Bid bid = d_agents[d_activeAgent]->takeTurn(call);
        if (setNextActiveAgent(bid, call))
        {
            d_logger.show();
            break;
        }
        else
            showBidToEveryone(bid);

        d_logger.show();
    }
}

bool Simulation::isEndGame() const
{
    return d_numDeadAgents == 2;
}

uint Simulation::numDiceWithEyesInGame(uint eyes)
{
    uint res = 0;
    for (uint idx = 0; idx < 3; ++idx)
        if (d_agents[idx]->isAlive())
            res += d_agents[idx]->numDiceWithEyes(eyes);
    return res;
}

bool Simulation::setNextActiveAgent(Bid const &bid, bool call)
{
    if (call)
        processCalledBid(bid);
    else
    {
        d_logger.logBid(bid, *d_agents[d_activeAgent]);
        findNextAliveAgent();
    }

    return call;
}

void Simulation::processCalledBid(Bid const &bid)
{
    uint dice = numDiceWithEyesInGame(bid.eyes());
    if (dice < bid.dice())
    {
        d_logger.logCall(bid, d_agents, d_activeAgent, Logger::CALL::CORRECT);
        d_activeAgent = bid.id();
    }
    else
        d_logger.logCall(bid, d_agents, d_activeAgent, Logger::CALL::INCORRECT);

    --d_numDiceTotal;

    for (uint idx = 0; idx < 3; ++idx)
        d_agents[idx]->observeEnd(d_activeAgent);

    // This might end the game for an agent so check if the active agent is still available
    if (not d_agents[d_activeAgent]->isAlive())
    {
        ++d_numDeadAgents;
        findNextAliveAgent();
    }
}

void Simulation::findNextAliveAgent()
{
    while (true)
    {
        d_activeAgent = (d_activeAgent + 1) % 3;
        if (d_agents[d_activeAgent]->isAlive())
            break;
    }
}

void Simulation::showBidToEveryone(Bid const &bid)
{
    for (uint idx = 0; idx < 3; ++idx)
        d_agents[idx]->observeBid(bid);
}
