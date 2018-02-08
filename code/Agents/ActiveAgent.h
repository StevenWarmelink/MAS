#ifndef MAS_ACTIVEAGENT_H
#define MAS_ACTIVEAGENT_H

#include "Agent.h"


class ActiveAgent final : public Agent
{
    uint d_id;

public:
    ActiveAgent(uint id, Logger &logger);

    virtual Bid takeTurn(bool &call) override;
};


#endif //MAS_ACTIVEAGENT_H
