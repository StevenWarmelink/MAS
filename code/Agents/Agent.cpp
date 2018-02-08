#include "Agent.h"
#include <algorithm>

std::random_device Agent::s_rd;
std::mt19937 Agent::s_gen = std::mt19937(s_rd());
std::uniform_int_distribution<uint> Agent::s_dist = std::uniform_int_distribution<uint>(0, 5);

Agent::Agent(uint id, Logger &logger)
    :
    d_logger(logger),
    d_id(id),
    d_myThrow(6, 0),
    d_observedDiceMine(6, 0),
    d_observedDiceOther1(6, 0),
    d_observedDiceOther2(6, 0)
{
    rollDice();
}

Agent::~Agent()
{
}

// --- Public functions

uint Agent::id() const
{
    return d_id;
}

bool Agent::isAlive() const
{
    return d_numDiceMine != 0;
}

bool Agent::isValidBid(Bid const &bid) const
{
    if (bid.eyes() < 0 or bid.eyes() > 5)
        return false;
    if (bid.dice() <= 0 or bid.dice() > d_numDiceTotal)
        return false;
    if (bid.eyes() <= d_lastBid.eyes() and bid.dice() <= d_lastBid.dice())
        return false;
    if (bid.eyes() > d_lastBid.eyes() and bid.dice() < d_lastBid.dice())
        return false;

    return true;
}

Bid Agent::takeTurn(bool &call)
{
    // I should not take part when i don't have any dice!
    if (not isAlive())
        return Bid(0, 0, d_id);

    processLastBids();
    std::vector<uint> limits = generateBidLimits();
    generateBidEvaluations(limits);
    return pickBestBid(call);
}

void Agent::observeBid(Bid bid)
{
    if (isAlive())
        d_lastBids.push(bid);
}

void Agent::observeEnd(uint loserId)
{
    if (not isAlive())
        return;

    updateNumberOfDice(loserId);
    rollDice();

    // Reset the state
    d_lastBid = Bid(5, 0, d_id);
    while (not d_lastBids.empty())
        d_lastBids.pop();
    std::fill(d_observedDiceOther1.begin(), d_observedDiceOther1.end(), 0);
    std::fill(d_observedDiceOther2.begin(), d_observedDiceOther2.end(), 0);
    std::fill(d_observedDiceMine.begin(), d_observedDiceMine.end(), 0);
    d_numDiceUnknown = d_numDiceTotal - d_numDiceMine;
}

uint Agent::numDiceWithEyes(uint eyes) const
{
    if (not isAlive())
        return 0;
    return d_myThrow.at(eyes);
}

uint Agent::numDiceUnknown() const
{
    return d_numDiceUnknown;
}

uint Agent::numDiceObservedWithEyes(AGENT agent, uint eyes) const
{
    switch (agent)
    {
        case AGENT::ME :
            return d_observedDiceMine.at(eyes);
        case AGENT::OTHER1 :
            return d_observedDiceOther1.at(eyes);
        case AGENT::OTHER2 :
            return d_observedDiceOther2.at(eyes);
    }
}

uint Agent::numDiceObservedWithEyes(uint eyes) const
{
    return d_observedDiceMine.at(eyes) + d_observedDiceOther1.at(eyes) + d_observedDiceOther2.at(eyes);
}

Bid const &Agent::lastBid() const
{
    return d_lastBid;
}

uint Agent::randomNumber() const
{
    return s_dist(s_gen);
}

uint Agent::numDiceOf(AGENT agent) const
{
    switch (agent)
    {
        case AGENT::ME:
            return d_numDiceMine;
        case AGENT::OTHER1:
            return d_numDiceOther1;
        case AGENT::OTHER2:
            return d_numDiceOther2;
    }
}

uint Agent::numDiceTotal() const
{
    return d_numDiceTotal;
}

std::vector<Bid> const &Agent::potentialBids() const
{
    return d_potentialBids;
}

std::vector<Bid> const &Agent::bestBids() const
{
    return d_bestBids;
}

uint Agent::selectedBidIdx() const
{
    return d_selectedBidIdx;
}

// --- Protected virtual functions

Bid Agent::pickBestBid(bool &call)
{
    d_bestBids.clear();
    std::for_each(d_potentialBids.begin(), d_potentialBids.end(), [this](Bid const &bid)
    {
        if (isValidBid(bid))
        {
            if (d_bestBids.empty())
                d_bestBids.push_back(bid);
            else
            {
                if (bid.evaluation() > d_bestBids.at(0).evaluation())
                {
                    d_bestBids.clear();
                    d_bestBids.push_back(bid);
                }
                else 
                    if (bid.evaluation() == d_bestBids.at(0).evaluation())
                        d_bestBids.push_back(bid);
            }
        }
    });


    if (not d_bestBids.empty() && -d_lastBid.evaluation() <= (d_bestBids.front().evaluation() + 1))
    {
        d_selectedBidIdx = randomNumber() % static_cast<uint>(d_bestBids.size());
        return d_bestBids[d_selectedBidIdx];
    }
    call = true;
    return lastBid();
}

double Agent::evaluatePotentialBid(Bid const &bid) const
{
    double known = numDiceWithEyes(bid.eyes());
    known += numDiceObservedWithEyes(AGENT::OTHER1, bid.eyes());
    known += numDiceObservedWithEyes(AGENT::OTHER2, bid.eyes());

    return std::round(known + numDiceUnknown() / 6) - bid.dice();
}

// How many dice does the other agent need to have to bid what he did knowing what he believes about the throws that he cannot see.
uint Agent::evaluateObservedBid(Bid const &bid) const
{
    double known = numDiceObservedWithEyes(bid.eyes());

    if (bid.id() == d_id)
    {
        double prob = static_cast<double>(numDiceUnknown()) / 6;
        return std::min(static_cast<uint>(std::max(std::round(bid.dice() - prob - known), 0.0)), bid.dice());
    }

    // Do not use your own dice to compute what other's must have

    uint diceOther = 0;
    double observedOther = 0;

    if ((id() + 1) % 3 == bid.id()) // Bid made by other1
    {
        diceOther = numDiceOf(AGENT::OTHER2);
        for (uint idx = 0; idx < 6; ++idx)
            observedOther += numDiceObservedWithEyes(AGENT::OTHER2, idx);
    }
    if ((id() + 2) % 3 == bid.id()) // Bid made by other2
    {
        diceOther = numDiceOf(AGENT::OTHER1);
        for (uint idx = 0; idx < 6; ++idx)
            observedOther += numDiceObservedWithEyes(AGENT::OTHER1, idx);
    }

    double otherKnowMe = 0;
    for (uint idx = 0; idx < 6; ++idx)
        otherKnowMe += numDiceObservedWithEyes(AGENT::ME, idx);

    double prob = static_cast<double>(diceOther - observedOther + numDiceOf() - otherKnowMe) / 6;
    uint expectedDice = std::min(static_cast<uint>(std::max(std::round(bid.dice() - std::max(prob, 0.0) - known), 0.0)), bid.dice());
    return expectedDice;
}

// --- Private functions

void Agent::processLastBids()
{
    Bid bid(0, 0, 0);
    bool processedBids = false;
    while (not d_lastBids.empty())
    {
        processedBids = true;
        bid = d_lastBids.front();
        d_lastBids.pop();

        if (bid.dice() != 0)
        {
            uint value = evaluateObservedBid(bid);
            d_logger.logProcessBid(bid, value, *this);
            if (bid.id() == d_id)
                d_observedDiceMine.at(bid.eyes()) += value;
            else
            {
                if (bid.id() == (1 + d_id) % 3)
                {
                    d_observedDiceOther1.at(bid.eyes()) += value;
                    d_observedDiceOther1[bid.eyes()] = std::min(d_observedDiceOther1.at(bid.eyes()), d_numDiceOther1);
                }
                else
                {
                    d_observedDiceOther2.at(bid.eyes()) += value;
                    d_observedDiceOther2[bid.eyes()] = std::min(d_observedDiceOther2.at(bid.eyes()), d_numDiceOther2);
                }

                if (value > d_numDiceUnknown)
                    d_numDiceUnknown = 0;
                else
                    d_numDiceUnknown -= value;
            }
        }
    }
    if (processedBids)
    {
        d_lastBid = bid;
        d_lastBid.evaluation(evaluatePotentialBid(d_lastBid));
    }
}

void Agent::updateNumberOfDice(uint loserId)
{
    --d_numDiceTotal;
    if (loserId == d_id)
        --d_numDiceMine;
    if (loserId == (d_id + 1) % 3)
        --d_numDiceOther1;
    if (loserId == (d_id + 2) % 3)
        --d_numDiceOther2;
}

void Agent::rollDice()
{
    std::fill(d_myThrow.begin(), d_myThrow.end(), 0);
    for (uint idx = 0; idx < d_numDiceMine; ++idx)
        ++d_myThrow[randomNumber()];
}

std::vector<uint> Agent::generateBidLimits() const
{
    std::vector<uint> res(6, d_lastBid.dice());
    for (uint idx = 0; idx <= d_lastBid.eyes(); ++idx)
        ++res[idx];

    return res;
}

void Agent::generateBidEvaluations(std::vector<uint> const &limits)
{
    d_potentialBids = std::vector<Bid>(6, Bid(0, 0, d_id));

    for (uint idx = 0; idx < 6; ++idx)
    {
        d_potentialBids[idx].eyes(idx);
        d_potentialBids[idx].dice(limits[idx]);
        d_potentialBids[idx].evaluation(evaluatePotentialBid(d_potentialBids[idx]));
    }
}

uint Agent::getNumDiceA1(){
    return d_numDiceOther1;
}


uint Agent::getNumDiceA2(){
    return d_numDiceOther2;
}