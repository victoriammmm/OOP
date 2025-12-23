#pragma once
#include <fstream>
#include <string>
#include "Universe.h"

class ConsoleView{
public:
    void display(const Universe& universe);
    void displayError(const std::string& message);
    void displayHelp();
};