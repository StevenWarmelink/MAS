//
// Created by jelmer on 15-3-17.
//

#include "Bid.h"

Bid::Bid(uint eyes, uint dice, uint id)
    :
    d_eyes(eyes),
    d_dice(dice),
    d_id(id)
{
}

uint Bid::eyes() const
{
    return d_eyes;
}

uint Bid::printEyes() const
{
    return d_eyes + 1;
}

void Bid::eyes(uint const &eyes)
{
    d_eyes = eyes;
}

uint const &Bid::dice() const
{
    return d_dice;
}

void Bid::dice(uint const &dice)
{
    d_dice = dice;
}

double Bid::evaluation() const
{
    return d_evaluation;
}

void Bid::evaluation(double evaluation)
{
    d_evaluation = evaluation;
}

uint Bid::id() const
{
    return d_id;
}

void Bid::id(uint id)
{
    d_id = id;
}

std::ostream &operator<<(std::ostream &os, Bid const &bid)
{
    os << "Bid:\n"
        "\tEyes:\t" << bid.eyes() + 1 << "\n"
           "\tDice:\t" << bid.dice() << "\n"
           "\tID:\t" << bid.id() << "\n";
    //"\tEvaluation:\t" << bid.evaluation() << "\n";
    return os;
}
