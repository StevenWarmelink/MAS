Multi Agent Systems Project
---------------------------
This repository contains a simulation of the game Perudo. Statistics and epistemic logic are used.

**TODO**
- Bug fixes:
  - Het bieden gaat te lang door
  - Agents geloven dat er een onmogelijk aantal stenen bij andere agents zijn
  - Agents bieden onmogelijke boden
- Interface:
  - Stop bij elke turn
  - Geef de optie tot meer informatie
  - Maak duidelijk hoe kennis wordt gebruikt met alleen de command line info
- Report:
  - Uitleg van het spel voor een leek
  - Duidelijk maken welke regels wij gebruiken
- Kennis en Logica:
  - Niet alleen gebruiken voor beschrijven van het systeem
  - Iets toevoegen waar het actief voor wordt gebruikt


**Rules:**

Start with 5 dice per player, we use 3 players. Players have to sequentially bid on the
number of dice that they believe have a certain number of eyes. Bids have to either increase in 
number of dice or the number of eyes. Or a player can call a bid when he believes that it is not true.
When a bid is called and is found to be correct the caller loses a die. When the bid was incorrect
the bidder loses a die.

The complete game also uses jokers and special rules when a player has only one die left.
We chose not to take these ruels into account for the sake of simplicity.

**System:**

First some notation:
- `a(m)(d) = x` means agent `m` has `x` dice with `d` eyes.
- `b(m)(d) = x` means that agent `m` bids `x` dice with `d` eyes.
- `c(d) = x` means that for `d` eyes a bid has to be larger or equal to `x` dice.
- `p(d)` is the expected number of dice with `d` eyes in the unknown dice.
- `B(m)` is a prefix which means that agent `m` believes what is prefixed.
- `E` is a prefix which means that every agent believes what is prefixed.
- `C` is a prefix which means that what is prefixed is commonly believed.

Agents keep track of the following information:
- The total number of dice left in the game.
- The number of dice of all agents individually.
- What everyone believes that they have thrown based on their bids.  
  `E a(m)(d) = x` for `1 <= d <= 6`.
- What everyone believes that the other agents have individually based on their bids.  
  `E a(m)(d) = x` for `1 <= d <= 6` and all other agents `m`.
- The total number of dice in the round that have not been accounted for.

**Bids** are made by computing the minimum number of dice required for each number of eyes.
- `c(d2) = b(1)(d1)` if `d2 > d1`
- `c(d2) = b(1)(d1) + 1` otherwise

for `1 <= d2 <= 6` where `b(1)(d1)` is the last made bid. Then for each number of eyes the minimum possible bid is evaluated.

- `a(m)(d) + E a(n1)(d) + E a(n2)(d) + numUnknownDice / 6 - c(d)`
for `1 <= d <= 6` where `m` is the active agent. `n1` and `n2` are the other agents.

When the bids with the highest evaluation have a positive evaluation a random one is chosen.
Otherwise the last bid is called and the turn ends.

**All agents** update their believes based on all bids that are made. Suppose agent 1 has the turn and agents
2 and 3 have made bids. Then agent 1 has two bids to process, and has to estimate how many dice the other agents have
in order to bid what they did. In this process only the beliefs that everyone hold are used.
`b(2)(d)` is processed by updating

- `E a(2)(d) += (numUnknownDice + numMyDice - E a(1)(d)) / 6 - E a(1)(d) + E a(2)(d) + E a(3)(d)`   
   where the update cannot be negative or lower than the number of dice that were bid. The value after the update
   can also not be higher that the total number of dice that agent 2 has.

This update rule can be summarised as follows: How many dice does agent 2 have more than I currently believe in order to
bid what he did knowing what he believes that everyone has. All agents will get the same results because they do
not use the information of their throw.

**Example round of play**

The first agent takes a turn, in which it makes a betting statement based on its own dice roll and
the basic estimates about the unknown dice. A bet is divided into eyes (1-6) and the number of dice bet.

This number of dice must be higher than the previous bet if the bet contains the same or a lower number
eyes. The number of dice of a higher bet can be the same as the previous bid's if the number of eyes is
higher. The agent makes the bet which it believes has the highest utility, based on how many dice of
each type are currently in the game. This statement is then received by the second agent, who evaluates
it, based on its own dice roll and its estimate (which is assumed to be the same estimate as the other
agent for now). If it deduces it can make a higher bet which it believes is plausible, it will select its own bet.

It does this using the same method as the first agent, but also includes the knowledge made by the betting
statement of agent one (as described in the previous section). If a player does not see any possible bet it can
make, it will ”call” the previous player's bid, which will resolve that round in favor of one of the two agents
as described above. In this model the agents use basic deductions in the form of A(1)Bets(4)(1)→A(1)Belief s(4)(1).
If A(1) bets 4 ones, then Agent 1 beliefs there are 4 ones.  Then based on the number of dice still in play,
the agent could reason that (Estimate(2)(1)∧A(1)Beliefs(4)(1))→A(1)(2)(1) (If an estimate of unknown dice would
lead to 2 ones and A(1) beliefs that there are 4 ones, A(1) must have 2 ones themselves. This way the bet has
conveyed information for agent two and it can use this information.

**Further Improvements:**

We can see that with every turn an agent makes an announcement. Since all agents know the way the other
agents reason, they can use this knowledge to extrapolate the die roll the announcing agents has made.
However we could model the following aditions:

Instead of calling when an agent has no more options to bet, it could instead also call out bets which
it does not believe are plausible. For example agent 2 could ”call” a bet of 4 fives from agents 1, when
it knows it has no fives and has deduced that agent 1 believes there to be two fives in the (for agent 1)
unknown number of dice. Since the agent 3 has only 5 dice it is unlikely he will have 2 fives's, therefore
agent 2 will call out the bet.

Agents could also be set to make bets they see as implausible (bluffing). This would give other agents a
false belief of the type of eyes the agent has rolled. To deal with this agents would also store beliefs
about the trustworthiness of other agents. If they detect lying (which they could by analyzing called bets),
they could set a new parameter of which agents they trust. $A_1 trusts A_2$. This would make the game more
dynamic and more realistic.
