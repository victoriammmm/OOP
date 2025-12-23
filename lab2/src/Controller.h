#pragma once
#include <string>
#include "SaveToFile.h"
#include "View.h"

class GameController{
public:
    GameController(Universe& universe, ConsoleView& consoleView, FileView& fileView);
    void run(int argc, char* argv[]);

private:
    Universe& universe;
    ConsoleView& consoleView;
    FileView& fileView;
    void processCommand(const std::string& command);
    void tick(int n);
    void dump(const std::string& filename);
    void exitGame();
    void help();
};