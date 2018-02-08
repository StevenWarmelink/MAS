//
// Created by jelmer on 15-3-17.
//

#ifndef MAS_BID_H
#define MAS_BID_H

#include <iostream>

class Bid
{
    uint d_id;
    uint d_eyes;    // 1 to 6
    uint d_dice;
    double d_evaluation = 0;
    friend std::ostream &operator<<(std::ostream &os, Bid const &bid);

public:
    Bid() = default;
    Bid(uint eyes, uint dice, uint id);

    uint id() const;
    void id(uint id);
    uint eyes() const;
    uint printEyes() const;
    void eyes(uint const &eyes);
    uint const &dice() const;
    void dice(uint const &dice);
    double evaluation() const;
    void evaluation(double evaluation);
};


#endif //MAS_BID_H
