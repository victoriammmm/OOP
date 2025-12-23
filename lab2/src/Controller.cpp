#include "Controller.h"
#include <sstream>
#include "Universe.h"
#include "View.h"
#include "LoadFromFile.h"

GameController::GameController(Universe& universe, ConsoleView& consoleView, FileView& fileView)
    : universe(universe), consoleView(consoleView), fileView(fileView){
}

void GameController::run(int argc, char* argv[]){
    try{
        if (argc == 4){
            std::string inputFile = argv[1];
            int ticks = std::stoi(argv[2]);
            std::string outputFile = argv[3];

            UniverseLoader::loadFromFile(universe, inputFile);

            tick(ticks);
            fileView.saveToFile(universe, outputFile);
            std::cout << "Universe saved to " << outputFile << std::endl;
            return;
        }

        if (argc == 2){
            UniverseLoader::loadFromFile(universe, argv[1]);
        }
        else{
            universe.randomInit();
        }

        consoleView.display(universe);

        std::string command;
        while (true){
            std::cout << "> ";
            std::getline(std::cin, command);
            processCommand(command);
        }
    }
    catch (const std::exception& e){
        consoleView.displayError(e.what());
    }
}
void GameController::processCommand(const std::string& command)
{
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;

    if (cmd == "exit")
    {
        exitGame();
    }
    else if (cmd == "tick" || cmd == "t")
    {
        int n = 1;
        iss >> n;
        tick(n);
    }
    else if (cmd == "dump")
    {
        std::string filename;
        iss >> filename;
        dump(filename);
    }
    else if (cmd == "help")
    {
        help();
    }
    else
    {
        consoleView.displayError(
            "Unknown command. Type 'help' for a list of commands.");
    }
}

void GameController::tick(int n)
{
    for (int i = 0; i < n; ++i)
    {
        universe.tick();
    }
    consoleView.display(universe);
}

void GameController::dump(const std::string& filename)
{
    try
    {
        fileView.saveToFile(universe, filename);
        std::cout << "Universe saved to " << filename << std::endl;
    }
    catch (const std::exception& e)
    {
        consoleView.displayError(e.what());
    }
}

void GameController::exitGame()
{
    std::cout << "Exiting the game." << std::endl;
    exit(0);
}

void GameController::help()
{
    consoleView.displayHelp();
}