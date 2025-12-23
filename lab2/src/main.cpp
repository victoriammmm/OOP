#include "Controller.h"
#include "Universe.h"
#include "View.h"
#include <string>

int main(int argc, char* argv[]){
    Universe universe(6, 6, "B3/S23");
    ConsoleView consoleView;
    FileView fileView;
    GameController controller(universe, consoleView, fileView);
    controller.run(argc, argv);
    std::string str = "-i= -o=";
    return 0;
}
