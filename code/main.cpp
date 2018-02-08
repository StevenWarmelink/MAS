#include "Simulation/Simulation.h"
#include "Misc/arguments/arguments/arguments.h"

void showHelp(char name[])
{
    std::cout << "This is the Perudo Simulator created for MAS\n"
                  "usage: " << name << " [-p | (-c -s -r -d)]\n"
                  "\t Options:\n"
                  "\t\t -d: \t\t Enables debugging mode (player mode with logging control enabled)\n"
                  "\t\t -p: \t\t Enables player mode, all other options are disabled in player mode.\n"
                  "\t\t -c: \t\t Disables control mode, continues through every turn when disabled.\n"
                  "\t\t -s: \t\t Enables automatic logging of state information.\n"
                  "\t\t -r: \t\t Enables automatic logging of reasoning information.\n";

}

int main(int argc, char *argv[])
{
    Arguments args(argc, argv, "hpcsrd");

    if (args.option('h'))
    {
        showHelp(argv[0]);
        return 0;
    }

    bool activePlayer = args.option('p') > 0;
    bool useControl = args.option('c') == 0;
    bool showState = args.option('s') > 0;
    bool showReasoning = args.option('r') > 0;
    bool debugPlayer = args.option('d') > 0;

    if (activePlayer)
    {
        if (useControl or showState or showReasoning)
            std::cout << "Ignoring all options except active player!";
        Logger logger;
        Simulation sim(logger, Simulation::TYPE::GAME);
        try
        {
            sim.run();
        } catch(std::exception &e)
        {
            std::cout << e.what() << '\n';
        }

        return 0;
    }

    if (debugPlayer)
    {
        Logger logger(useControl, showState, showReasoning);
        Simulation sim(logger, Simulation::TYPE::GAME);
        try
        {
            sim.run();
        } catch(std::exception &e)
        {
            std::cout << e.what() << '\n';
        }

        return 0;
    }

    Logger logger(useControl, showState, showReasoning);
    Simulation sim(logger, Simulation::TYPE::SIMULATION);
    try
    {
        sim.run();
    } catch(std::exception &e)
    {
        std::cout << e.what() << '\n';
    }
}
