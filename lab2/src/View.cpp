#include "View.h"

void ConsoleView::display(const Universe& universe){
    std::cout << "Universe: " << universe.getName() << std::endl;
    std::cout << "Rule: " << universe.getRule() << std::endl;
    std::cout << "Iteration: " << universe.getIteration() << std::endl;
    const Field& field = universe.getField();

    for (int i = 0; i < field.getWidth(); ++i){
        for (int j = 0; j < field.getHeight(); ++j){
            if (field.getCell(i, j) == CellState::Alive) {
                std::cout << "@ ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    }
}

void ConsoleView::displayHelp(){
    std::cout << "Commands:" << std::endl;
    std::cout << "  tick [n=1] - Calculate n (default 1) iterations and print "
                 "the result."
              << std::endl;
    std::cout << "  dump <filename> - Save the universe to a file."
              << std::endl;
    std::cout << "  exit - Exit the game." << std::endl;
    std::cout << "  help - Print this help message." << std::endl;
}

void ConsoleView::displayError(const std::string& message){
    std::cerr << "Error: " << message << std::endl;
}