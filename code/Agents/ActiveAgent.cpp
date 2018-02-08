//
// Created by jelmer on 29-3-17.
//

#include "ActiveAgent.h"

ActiveAgent::ActiveAgent(uint id, Logger &logger)
:
    Agent(id, logger)
{
    std::cout << "Created Active Agent\n";
}

Bid ActiveAgent::takeTurn(bool &call)
{
    if (not isAlive())
        return Bid(0, 0, d_id);

    processLastBids();

    std::cout << "Agent " << id() << ": I have thrown:\t\t\t\t" << numDiceWithEyes(0) << "->1 "
                << numDiceWithEyes(1) << "->2 " << numDiceWithEyes(2) << "->3 "
                << numDiceWithEyes(3) << "->4 " << numDiceWithEyes(4) << "->5 "
                << numDiceWithEyes(5) << "->6, " << numDiceOf() << " dice in total\n";


    Bid bid(0, 0, d_id);
    bool goodBid = false;
    bool goodEyes = false;
    bool goodDice = false;
    bool goodCall = false;

    std::string buf;
    int eyes = -1;
    int dice = -1;

    if (lastBid().dice() != 0)
        while (not goodCall)
        {
            std::cout << "Total dice remaining in the game: " <<  numDiceTotal() << "\n";
            std::cout << "Dice remaining for Agent 0 (PLAYER): " << numDiceTotal() - getNumDiceA1() - getNumDiceA2() << "\n";
            std::cout << "Dice remaining for Agent 1 (Agent) : " << getNumDiceA1() << "\n";
            std::cout << "Dice remaining for Agent 2 (Agent) : " <<  getNumDiceA2() << "\n";
            std::cout << "Call (y|n) ? ";
            std::cin >> buf;
            if (buf.front() == 'y')
            {
                call = true;
                return lastBid();
            }
            if (buf.front() == 'n')
                goodCall = true;
            else
            {
                std::cout << "'" << buf <<"' is not a valid response.\n";
            }
        }

    while (not goodBid)
    {
        while (not goodDice)
        {
            std::cout << "Total dice remaining in the game: " <<  numDiceTotal() << "\n";
            std::cout << "Dice remaining for Agent 0 (PLAYER): " << numDiceTotal() - getNumDiceA1() - getNumDiceA2() << "\n";
            std::cout << "Dice remaining for Agent 1 (Agent) : " << getNumDiceA1() << "\n";
            std::cout << "Dice remaining for Agent 2 (Agent) : " <<  getNumDiceA2() << "\n";
            std::cout << "===================\n";
            std::cout << "| Make a new bet! |\n" ;
            std::cout << "===================\n";
            std::cout << "Dice ? ";
            std::cin >> buf;
            try
            {
                dice = std::stoi(buf);
            } catch (std::exception &ex)
            {
                std::cout << "'" << buf << "' is an invalid response\n";
            }
            if (dice > 0 and dice <= numDiceTotal())
                goodDice = true;
            else
                std::cout << "Enter a number between 1 and " << numDiceTotal() << '\n';
        }


        while (not goodEyes)
        {

            std::cout << "Eyes ? ";
            std::cin >> buf;
            try
            {
                eyes = std::stoi(buf) - 1;
            } catch (std::exception &ex)
            {
                std::cout << "'" << buf << "' is an invalid response\n";
            }
            if (eyes >= 0 && eyes < 6)
                goodEyes = true;
            else
                std::cout << "Enter a number between 1 and 6\n";
        }


        bid.eyes(static_cast<uint>(eyes));
        bid.dice(static_cast<uint>(dice));
        if (isValidBid(bid))
            goodBid = true;
        else
        {
            std::cout << "This bid is not valid, try again\n";
            goodEyes = false;
            goodDice = false;
        }
    }
    return bid;
}
