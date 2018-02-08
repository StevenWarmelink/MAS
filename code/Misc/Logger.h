#ifndef MAS_LOGGER_H
#define MAS_LOGGER_H

#include <vector>
#include <string>
#include <queue>
#include <memory>

#include "Bid.h"

class Agent;

class Logger
{
    // Logging state
    bool d_showState;
    bool d_showReasoning;

    // Control state
    bool d_useControls;
    bool d_playerActive;

public:
    enum class TYPE
    {
        ACTION,
        STATE,
        REASONING,
        MISC
    };

    enum class SELECT
    {
        ALL,
        ENABLED
    };

    enum class CALL
    {
        CORRECT,
        INCORRECT
    };

    class Message
    {
        std::string d_message;
        TYPE d_type;

        friend class Logger;
        friend std::ostream &operator<<(std::ostream &os, Message msg);

    public:
        Message(std::string msg, Logger::TYPE type);
    };

    Logger();                                    // Only print actions and stop only at the player's turn!
    Logger(bool useControls, bool showState, bool showReasoning); // Decide yourself what to turn on

    // Show all messages and allow the user to request more.
    void show();
    // Show messages without prompt
    void flush(SELECT selection = SELECT::ALL);

    // Register a message for this turn.
    void log(std::string msg, TYPE type);

    void logWin(uint id);
    void logStartRound(std::vector<std::shared_ptr<Agent>> const &agents);
    void logStartTurn(Agent const &agent);
    void logBid(Bid const &bid, Agent const &Agent);
    void logCall(Bid const &calledBid, std::vector<std::shared_ptr<Agent>> const &agents, uint activeAgent, CALL result);
    void logProcessBid(Bid const &bid, uint value, Agent const &agent);


private:
    std::queue<Message> d_messages;

    void showAllMessages();
    void showEnabledMessages();
    void showEnabledMessage(Message const &msg) const;
    void showMessagesOfType(TYPE type);

    void cleanForNextTurn();

    void logAgentState(Agent const &agent);
    void logPotentialBids(Agent const &agent);

    void handleInput();
    void showPrompt();
};



#endif //MAS_LOGGER_H
