#include "Logger.h"
#include "../Agents/Agent.h"
#include <sstream>
#include <algorithm>

Logger::Logger()
    :
    d_showState(false),
    d_showReasoning(false),
    d_useControls(false),
    d_playerActive(true) { }

Logger::Logger(bool useControls, bool showState, bool showReasoning)
    :
    d_showState(showState),
    d_showReasoning(showReasoning),
    d_useControls(useControls),
    d_playerActive(false)
{
}

Logger::Message::Message(std::string msg, Logger::TYPE type)
    :
    d_message(msg),
    d_type(type)
{

}

// --- Public functions

void Logger::show()
{
    // Show all messages that enabled
    showEnabledMessages();
    handleInput();
    cleanForNextTurn();
}

void Logger::flush(SELECT selection)
{
    if (selection == SELECT::ALL)
        showAllMessages();
    else
        showEnabledMessages();
    cleanForNextTurn();
}

void Logger::log(std::string msg, Logger::TYPE type)
{
    d_messages.push(Message(msg, type));
}

void Logger::logWin(uint id)
{
    std::ostringstream buf;
    buf << "Agent " << id << " wins!";
    d_messages.push(Message(buf.str(), TYPE::ACTION));
}

void Logger::logStartTurn(Agent const &agent)
{
    std::ostringstream buf;
    buf << "It is agent " << agent.id() << "'s turn";
    d_messages.push(Message(buf.str(), TYPE::ACTION));
}

void Logger::logStartRound(std::vector<std::shared_ptr<Agent>> const &agents)
{
    d_messages.push(Message("========================================================================================\n\t\t\t\t\tStart New Round\n========================================================================================\n", TYPE::MISC));
    std::string spdice = "dice";
    for (uint idx = 0; idx < 3; ++idx)
    {
        spdice = "dice";
        if(agents[idx]->numDiceOf() == 1){
            spdice = "die";
        }
        std::ostringstream buf;
        buf << "Agent " << agents[idx]->id() << ": I have thrown:\t\t\t\t" << agents[idx]->numDiceWithEyes(0) << "->1 "
            << agents[idx]->numDiceWithEyes(1) << "->2 " << agents[idx]->numDiceWithEyes(2) << "->3 "
            << agents[idx]->numDiceWithEyes(3) << "->4 " << agents[idx]->numDiceWithEyes(4) << "->5 "
            << agents[idx]->numDiceWithEyes(5) << "->6, " << agents[idx]->numDiceOf() << " "<< spdice << " in total";
        d_messages.push(Message(buf.str(), TYPE::STATE));
    }
}

void Logger::logBid(Bid const &bid, Agent const &agent)
{
    std::ostringstream buf;
    logAgentState(agent);
    logPotentialBids(agent);
    if(not agent.lastBid().dice() == 0){
        buf << "Agent " << agent.id() << ": The evaluation of my potential bid is more positive than the evaluation of the previous bid is negative.\n"
               "\t\t Meaning that it is more likely that my bid is correct than that the previous bid is incorrect.";
        d_messages.push(Message(buf.str(), TYPE::REASONING));
    }
    buf.str("");
    buf.clear();
    buf << "Agent " << agent.id() << ": I found a bid!:\n " << bid;
    d_messages.push(Message(buf.str(), TYPE::ACTION));
}

void Logger::logCall(Bid const &calledBid, std::vector<std::shared_ptr<Agent>> const &agents, uint activeAgent, Logger::CALL result)
{
    logAgentState(*agents[activeAgent]);
    logPotentialBids(*agents[activeAgent]);
    std::ostringstream buf;

    buf << "Agent " << agents[activeAgent]->id() << ": The evaluation of my potential bid is less positive than the evaluation of the previous bid is negative.\n"
        "\t\t Meaning that it is more likely that the previous bid is incorrect than that my best bid is correct.";
    d_messages.push(Message(buf.str(), TYPE::REASONING));

    buf.str("");
    buf.clear();

    std::string spdice = "dice";
    std::string speyes = "eyes";
    std::string verb   = "are";
    if(calledBid.dice() == 1){
        spdice = "die";
        verb = "is";
    } 
    if(calledBid.printEyes() == 1){
        speyes = "eye";
    }
    if (!(agents[activeAgent]->id() == 0 && d_playerActive)){
        buf << "Agent " << agents[activeAgent]->id() << ": I did not find a better bid and call agent " <<  agents[activeAgent]->id() << "'s bid of " << calledBid.dice() << " "<<  spdice <<" with " << calledBid.printEyes() << " " << speyes << ".";
        d_messages.push(Message(buf.str(), TYPE::ACTION));

        buf.str("");
        buf.clear();
        buf << "The called bid:\n" << calledBid;
        d_messages.push(Message(buf.str(), TYPE::STATE));
    }

    uint dice = agents[0]->numDiceWithEyes(calledBid.eyes()) +
                agents[1]->numDiceWithEyes(calledBid.eyes()) +
                agents[2]->numDiceWithEyes(calledBid.eyes());
    buf.str("");
    buf.clear();

    spdice = "dice";
    speyes = "eyes";
    verb = "are";
    if(dice == 1){
        spdice = "die";
        verb = "is";
    } 
    if(calledBid.printEyes() == 1){
        speyes = "eye";
    }

    buf << "There " << verb << " " << dice << " " << spdice <<" with " << calledBid.printEyes() << " " << speyes <<".";
    d_messages.push(Message(buf.str(), TYPE::ACTION));

    buf.str("");
    buf.clear();
    if (result == CALL::CORRECT)
        buf << "The call is correct!\n";
    else
        buf << "The call is incorrect!\n";
    d_messages.push(Message(buf.str(), TYPE::ACTION));
}

void Logger::logAgentState(Agent const &agent)
{
    std::string spdice = "dice";
    std::string verb = "are";
    if(agent.numDiceOf() == 1){
        spdice = "die";
        verb = "is";
    } 
    std::ostringstream buf;
    buf << "Agent " << agent.id() << ": I have thrown:\t\t\t\t" << agent.numDiceWithEyes(0) << "->1 "
        << agent.numDiceWithEyes(1) << "->2 " << agent.numDiceWithEyes(2) << "->3 " << agent.numDiceWithEyes(3)
        << "->4 " << agent.numDiceWithEyes(4) << "->5 " << agent.numDiceWithEyes(5) << "->6, " << agent.numDiceOf()
        << " " << spdice << " in total";
    d_messages.push(Message(buf.str(), TYPE::STATE));

    buf.str("");
    buf.clear();
    buf << "Agent " << agent.id() << ": Other agents believe that I have:\t"
        << agent.numDiceObservedWithEyes(Agent::AGENT::ME, 0) << "->1 "
        << agent.numDiceObservedWithEyes(Agent::AGENT::ME, 1) << "->2 "
        << agent.numDiceObservedWithEyes(Agent::AGENT::ME, 2) << "->3 "
        << agent.numDiceObservedWithEyes(Agent::AGENT::ME, 3) << "->4 "
        << agent.numDiceObservedWithEyes(Agent::AGENT::ME, 4) << "->5 "
        << agent.numDiceObservedWithEyes(Agent::AGENT::ME, 5) << "->6";
    d_messages.push(Message(buf.str(), TYPE::STATE));

    buf.str("");
    buf.clear();
    buf << "Agent " << agent.id() << ": Everyone believes that agent " << (agent.id() + 1) % 3 << " has:\t"
        << agent.numDiceObservedWithEyes(Agent::AGENT::OTHER1, 0) << "->1 "
        << agent.numDiceObservedWithEyes(Agent::AGENT::OTHER1, 1) << "->2 "
        << agent.numDiceObservedWithEyes(Agent::AGENT::OTHER1, 2) << "->3 "
        << agent.numDiceObservedWithEyes(Agent::AGENT::OTHER1, 3) << "->4 "
        << agent.numDiceObservedWithEyes(Agent::AGENT::OTHER1, 4) << "->5 "
        << agent.numDiceObservedWithEyes(Agent::AGENT::OTHER1, 5) << "->6";
    d_messages.push(Message(buf.str(), TYPE::STATE));

    buf.str("");
    buf.clear();
    buf << "Agent " << agent.id() << ": Everyone believes that agent " << (agent.id() + 2) % 3 << " has:\t"
        << agent.numDiceObservedWithEyes(Agent::AGENT::OTHER2, 0) << "->1 "
        << agent.numDiceObservedWithEyes(Agent::AGENT::OTHER2, 1) << "->2 "
        << agent.numDiceObservedWithEyes(Agent::AGENT::OTHER2, 2) << "->3 "
        << agent.numDiceObservedWithEyes(Agent::AGENT::OTHER2, 3) << "->4 "
        << agent.numDiceObservedWithEyes(Agent::AGENT::OTHER2, 4) << "->5 "
        << agent.numDiceObservedWithEyes(Agent::AGENT::OTHER2, 5) << "->6";
    d_messages.push(Message(buf.str(), TYPE::STATE));

    std::string dice2 = "dice";
    std::string verb2 = "are";
    ((agent.numDiceTotal() == 1) ? verb = "is", spdice = "die" : verb = "are", spdice = "dice");
    (((agent.numDiceTotal() - agent.numDiceOf()) == 1) ? dice2 = "die", verb2 = "is" : dice2 = "dice", verb2 = "are");
    buf.str("");
    buf.clear();
    buf << "Agent " << agent.id() << ": There " << verb << " " << agent.numDiceTotal() << " " << spdice <<" left in the game. "
    // TODO: DICE SING/PL
        << agent.numDiceTotal() - agent.numDiceOf() << " " << dice2 << " " << verb2 <<" unknown to me.";
    d_messages.push(Message(buf.str(), TYPE::STATE));
}

void Logger::logProcessBid(Bid const &bid, uint value, Agent const &agent)
{
    std::ostringstream buf;

    std::string speyes = "eyes";
    std::string spdice = "dice";
    std::string verb = "have";
    if(bid.dice() == 1){
        spdice = "die";
        verb = "is";
    } 
    if(bid.printEyes() == 1){
        speyes = "eye";
    }
    buf << "Agent " << agent.id() << ": Agent " << bid.id() << " bid " << bid.dice() << " " << spdice << " with " << bid.printEyes()
        << " " << speyes <<".";
    d_messages.push(Message(buf.str(), TYPE::STATE));

    Agent::AGENT agnt = Agent::AGENT::ME;

    if ((agent.id() + 1) % 3 == bid.id())
        agnt = Agent::AGENT::OTHER1;
    if ((agent.id() + 2) % 3 == bid.id())
        agnt = Agent::AGENT::OTHER2;


    double beliefDice = agent.numDiceObservedWithEyes(bid.eyes());

    double unknownDice = agent.numDiceUnknown();
    if (agnt != Agent::AGENT::ME)
    {
        double observedMe = 0;
        for (uint idx = 0; idx < 6; ++idx)
            observedMe += agent.numDiceObservedWithEyes(Agent::AGENT::ME, idx);
        double observedOther = 0;
        if (agnt == Agent::AGENT::OTHER1)
        {
            for (uint idx = 0; idx < 6; ++idx)
                observedOther += agent.numDiceObservedWithEyes(Agent::AGENT::OTHER2, idx);
            unknownDice = agent.numDiceOf() - observedMe + agent.numDiceOf(Agent::AGENT::OTHER2) - observedOther;
        }
        else
        {
            for (uint idx = 0; idx < 6; ++idx)
                observedOther += agent.numDiceObservedWithEyes(Agent::AGENT::OTHER1, idx);
            unknownDice = agent.numDiceOf() - observedMe + agent.numDiceOf(Agent::AGENT::OTHER1) - observedOther;
        }
    }

    double additionalDice = bid.dice() - std::max(unknownDice, 0.0) / 6.0 - beliefDice;

    buf.str("");
    buf.clear();
    buf << "Agent " << agent.id() << ": Evaluating agent " << bid.id() << "'s bid: " << bid.dice() << " " << spdice << " with " << bid.printEyes()  << " " << speyes << ".";
    d_messages.push(Message(buf.str(), TYPE::REASONING));

    speyes = "eyes";
    spdice = "dice";
    verb = "are";
    if(beliefDice == 1){
        spdice = "die";
        verb = "is";
    } 
    if(bid.printEyes() == 1){
        speyes = "eye";
    }

    buf.str("");
    buf.clear();
    buf << "Agent " << agent.id() << ": I believe that everyone believes there " << verb << " " << beliefDice << " " << spdice << " with "
        << bid.printEyes()  << " " << speyes << " discovered this round.";
    d_messages.push(Message(buf.str(), TYPE::REASONING));

    spdice = "dice";
    verb = "are";
    if(unknownDice == 1){
        spdice = "die";
        verb = "is";
    }

    buf.str("");
    buf.clear();
    buf << "Agent " << agent.id() << ": There " << verb << " " << unknownDice << " " << spdice << " that agent " << bid.id()
        << " does not have a belief about.";
    d_messages.push(Message(buf.str(), TYPE::STATE));

    speyes = "eyes";
    if(bid.printEyes() == 1){
        speyes = "eye";
    }
    buf.str("");
    buf.clear();
    buf << "Agent " << agent.id() << ": There " << verb << " " << unknownDice << " " << spdice <<" that agent " << bid.id()
        << " does not have a belief about, so he expects " << unknownDice / 6 << " dice with " << bid.printEyes()
        << " " << speyes << ".";
    d_messages.push(Message(buf.str(), TYPE::REASONING));

    buf.str("");
    buf.clear();
    buf << "Agent " << agent.id() << ": Bids are based on an agent's own dice, their beliefs about other agents' dice, "
        "and the expected number of dice with 'n' eyes based on the dice that the agent does not have a belief about.\n"
        "\t\t This can be formulated as: bid = own dice + expected dice + belief dice\n"
        "\t\t So, the dice of the agent that made a bid can be estimated with the formula above: own dice = bid - expected dice - belief dice";
    d_messages.push(Message(buf.str(), TYPE::REASONING));


    ((additionalDice == 1) ? spdice = "die" : spdice = "dice");
    buf.str("");
    buf.clear();
    buf << "Agent " << agent.id() << ": I expect agent " << bid.id() << " to have a minimum of " << bid.dice() << " - " << unknownDice / 6
        << " - " << beliefDice << " = " << additionalDice << " additional " << spdice << " with " << bid.printEyes()  << " " << speyes << ".";
    d_messages.push(Message(buf.str(), TYPE::REASONING));

    buf.str("");
    buf.clear();
    buf << "Agent " << agent.id() << ": I have updated my belief about agent " << bid.id() << ": ";

    uint iBelieveDice = std::min(static_cast<uint>(std::max(std::round(additionalDice), 0.0)) + agent.numDiceObservedWithEyes(agnt, bid.eyes()), agent.numDiceOf(agnt));


    ((iBelieveDice == 1) ? spdice = "die" : spdice = "dice");
    ((bid.printEyes() == 1) ? speyes = "eye" : speyes = "eyes");
    buf << "I now believe that he has "
        << iBelieveDice << " " << spdice << " with " << bid.printEyes()  << " " << speyes << "\n";
    d_messages.push(Message(buf.str(), TYPE::STATE));
}

std::ostream &operator<<(std::ostream &os, Logger::Message msg)
{
    switch (msg.d_type)
    {
        case Logger::TYPE::ACTION:
            os << "ACTION:\t";
            break;
        case Logger::TYPE::STATE:
            os << "STATE:\t";
            break;
        case Logger::TYPE::REASONING:
            os << "REASON:\t";
            break;
        case Logger::TYPE::MISC:
            break;
    }

    os << msg.d_message << '\n';
    return os;
}

// --- Private functions

void Logger::showAllMessages()
{
    std::queue<Message> tmp;
    while (true)
    {
        if (d_messages.empty())
            break;

        Message msg = d_messages.front();
        tmp.push(msg);
        d_messages.pop();

        std::cout << msg;
    }

    d_messages = tmp;
}

void Logger::showEnabledMessages()
{
    std::queue<Message> tmp;
    while (true)
    {
        if (d_messages.empty())
            break;

        Message msg = d_messages.front();
        tmp.push(msg);
        d_messages.pop();

        showEnabledMessage(msg);
    }

    d_messages = tmp;
}

void Logger::showEnabledMessage(Message const &msg) const
{
    switch (msg.d_type)
    {
        case TYPE::ACTION:
            break;
        case TYPE::STATE:
            if (not d_showState)
                return;
            break;
        case TYPE::REASONING:
            if (not d_showReasoning)
                return;
            break;
    }

    // Print the message
    std::cout << msg;
}

void Logger::showMessagesOfType(TYPE type)
{
    std::queue<Message> tmp;
    while (true)
    {
        if (d_messages.empty())
            break;

        Message msg = d_messages.front();
        tmp.push(msg);
        d_messages.pop();

        if (msg.d_type == type)
            std::cout << msg;
    }

    d_messages = tmp;
}

void Logger::handleInput()
{
    if (not d_useControls)
        return;

    showPrompt();
    while (true)
    {
        char key;
        std::cin >> key;

        switch (key)
        {
            case 'q':
                throw std::invalid_argument("You decided to quit the program");
            case 'n':
                return;
            case 'r':
                if (not d_showReasoning and not d_playerActive)
                    showMessagesOfType(TYPE::REASONING);
                break;
            case 's':
                if (not d_showState and not d_playerActive)
                    showMessagesOfType(TYPE::STATE);
                break;
            default :
                std::cout << "The key '" << key << "' is not bound to an action.\n";
        }

        showPrompt();
    }
}

void Logger::showPrompt()
{
    std::cout << "(press 'n' to continue)\n";
    std::cout << "(press 'q' to quit)\n";
    if (not d_playerActive)
    {
        if (not d_showState)
            std::cout << "(press 's' to show the state)\n";
        if (not d_showReasoning)
            std::cout << "(press 'r' to show the reasoning)\n";
    }
    std::cout << "?:\t";
}

void Logger::cleanForNextTurn()
{
    while (true)
    {
        if (d_messages.empty())
            break;
        d_messages.pop();
    }
}

void Logger::logPotentialBids(Agent const &agent)
{
    std::ostringstream buf;
    buf << "Agent " << agent.id() << ": Bids are evaluated by estimating how many dice there are with the specified number of eyes in the game.\n"
        "\t\t Value = Own dice + Expected unknown dice + belief dice - dice that are bid\n"
        "\t\t A positive value means that the agent expects that there are more dice in the game than are bid, meaning "
        "that the last bid is correct.\n\t\t A negative value means that the agent expects that the bid is incorrect.";
    d_messages.push(Message(buf.str(), TYPE::REASONING));

    double expectedDice = agent.numDiceUnknown() / 6;
    double ownDice = agent.numDiceWithEyes(agent.lastBid().eyes());
    double observedDice = agent.numDiceObservedWithEyes(agent.lastBid().eyes()) -
                          agent.numDiceObservedWithEyes(Agent::AGENT::ME, agent.lastBid().eyes());

    if(not agent.lastBid().dice() == 0){
        std::string spdice[4];
        std::string speyes;
        std::string verb[2];

        ((agent.lastBid().dice() == 1) ? spdice[0] = "die" : spdice[0] = "dice");
        ((agent.lastBid().printEyes() == 1) ? speyes= "eye" : speyes = "eyes");
        ((ownDice == 1) ? spdice[1] = "die" : spdice[1] = "dice");
        ((agent.numDiceUnknown() == 1) ? spdice[2] = "die", verb[0] = "is" : spdice[2] = "dice", verb[0] = "are");
        ((expectedDice == 1) ? verb[1] = "is" : verb[1] = "are");
        ((observedDice == 1) ? spdice[3] = "die" : spdice[3] = "dice");


        buf.str("");
        buf.clear();
        buf << "Agent " << agent.id() << ": Agent " << agent.lastBid().id() << " made the last bid: "
            << agent.lastBid().dice() << " " << spdice[0] << " with " << agent.lastBid().printEyes()  << " " << speyes << ".\n";
        buf << "\t\t I have " << ownDice << " " << spdice[1] << " with " << agent.lastBid().printEyes()  << " " << speyes << ".\n";
        buf << "\t\t There " << verb[0] <<  " " << agent.numDiceUnknown() << " unknown " << spdice[2] << ", so "
            << expectedDice << " of the unknown dice " << verb[1] << " expected to have " << agent.lastBid().printEyes()  << " " << speyes << ".\n";
        buf << "\t\t I believe that the other agents have "
            << observedDice << " " << spdice[3] << " with " << agent.lastBid().printEyes()  << " " << speyes << ", so the last made bid evaluates to "
            << ownDice << " + " << expectedDice << " + " << observedDice << " - " << agent.lastBid().dice()
            << " = "  << ownDice + observedDice + expectedDice - agent.lastBid().dice() << ".";
        d_messages.push(Message(buf.str(), TYPE::REASONING));
    

        std::vector<Bid> const &bids = agent.bestBids();
        std::for_each(bids.begin(), bids.end(), [&](Bid const &potentialBid)
        {
            buf.str("");
            buf.clear();

            ((potentialBid.dice() == 1) ? spdice[0] = "die" : spdice[0] = "dice");
            ((potentialBid.eyes() == 1) ? speyes = "eye" : speyes = "eyes");


            buf << "Agent " << agent.id() << ": I have to bid at least " << potentialBid.dice() << " dice with " << potentialBid.printEyes()  << " eyes.";
            d_messages.push(Message(buf.str(), TYPE::REASONING));

            expectedDice = agent.numDiceUnknown() / 6;
            ownDice = agent.numDiceWithEyes(potentialBid.eyes());
            observedDice = agent.numDiceObservedWithEyes(potentialBid.eyes()) -
                           agent.numDiceObservedWithEyes(Agent::AGENT::ME, potentialBid.eyes());

            ((ownDice == 1) ? spdice[0] = "die" : spdice[0] = "dice");
            ((potentialBid.printEyes() == 1) ? speyes = "eye" : speyes = "eyes");
            ((agent.numDiceUnknown() == 1) ? spdice[1] = "die", verb[0] = "is" : spdice[1] = "dice", verb[0] = "are");
            ((expectedDice == 1) ? spdice[2] = "die" : spdice[2] = "dice");
            ((observedDice == 1) ? spdice[3] = "die" : spdice[3] = "dice");

            buf.str("");
            buf.clear();
            buf << "Agent " << agent.id() << ": I have " << ownDice << " " << spdice[0] <<" with " << potentialBid.printEyes()  << " " << speyes << ".\n";
            buf << "\t\t There " << verb[0] << " " << agent.numDiceUnknown() << " unknown " << spdice[1] << ", so "
                << expectedDice << " dice are expected to have " << potentialBid.printEyes()  << " " << speyes << ".\n";
            buf << "\t\t I believe that the other agents have "
                << observedDice << " " << spdice[3] << " with " << potentialBid.printEyes()  << " eyes, so the bid evaluates to "
                << ownDice << " + " << expectedDice << " + " <<  observedDice << " - " << potentialBid.dice()
                << " = "  << std::round(ownDice + observedDice + expectedDice) - potentialBid.dice() << ".";
            d_messages.push(Message(buf.str(), TYPE::REASONING));
        });
    }
    else{
        buf.str("");
        buf.clear();
        buf << "Agent " << agent.id() << " is allowed to make the first bid.";
        d_messages.push(Message(buf.str(), TYPE::STATE));
    }
}