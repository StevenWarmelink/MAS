#ifndef MAS_AGENT_H
#define MAS_AGENT_H

#include "../Misc/Bid.h"
#include "../Misc/Logger.h"
#include <queue>
#include <vector>
#include <random>

class Agent
{
    Logger &d_logger;
    uint d_id;

    // Hardcoded for 3 agents!!
    uint d_numDiceTotal = 15;
    uint d_numDiceUnknown = 10;
    uint d_numDiceMine = 5;
    uint d_numDiceOther1 = 5;
    uint d_numDiceOther2 = 5;

    // for each number of eyes the number of dice that occur
    std::vector<uint> d_myThrow;            // The exact throw, only known to this agent
    std::vector<uint> d_observedDiceMine;   // What everyone thinks that I have based on my bids
    std::vector<uint> d_observedDiceOther1; // What everyone thinks that the other agent has based on his bids
    std::vector<uint> d_observedDiceOther2; // What everyone thinks that the other agent has based on his bids

    // Keep track of the last bids and process at the start of the turn
    std::queue<Bid> d_lastBids;
    Bid d_lastBid = Bid(5, 0, d_id);    // The bid that has te be called or increased.

    std::vector<Bid> d_potentialBids;
    std::vector<Bid> d_bestBids;
    uint d_selectedBidIdx;

    // Stuff for random generation
    static std::random_device s_rd;
    static std::mt19937 s_gen;
    static std::uniform_int_distribution<uint> s_dist;

public:
    enum class AGENT
    {
        ME, OTHER1, OTHER2
    };

    Agent(uint id, Logger &logger);
    virtual ~Agent();   // Might be needed child classes with other strategies

    // Getters
    uint id() const;
    bool isAlive() const;

    uint numDiceWithEyes(uint eyes) const;
    uint numDiceUnknown() const;
    uint numDiceOf(AGENT agent = AGENT::ME) const;
    uint numDiceObservedWithEyes(uint eyes) const;
    uint numDiceObservedWithEyes(AGENT agent, uint eyes) const;
    uint numDiceTotal() const;

    std::vector<Bid> const &potentialBids() const;
    std::vector<Bid> const &bestBids() const;
    uint selectedBidIdx() const;
    Bid const &lastBid() const;
    bool isValidBid(Bid const &bid) const; // Validate if a bid is possible in the current state of the game

    virtual Bid takeTurn(bool &call);
    void observeBid(Bid bid);
    void observeEnd(uint loserId);

    uint getNumDiceA1();
    uint getNumDiceA2();

protected:
    uint randomNumber() const;

    virtual double evaluatePotentialBid(Bid const &bid) const;
    virtual uint evaluateObservedBid(Bid const &bid) const;
    virtual Bid pickBestBid(bool &call);
    void processLastBids();

private:
    void updateNumberOfDice(uint loserId);
    void rollDice();

    std::vector<uint> generateBidLimits() const;
    void generateBidEvaluations(std::vector<uint> const &limits);
};


#endif //MAS_AGENT_H
