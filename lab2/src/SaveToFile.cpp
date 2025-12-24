#include "SaveToFile.h"
#include <fstream>
#include "Universe.h"

void FileView::saveToFile(const Universe& universe, const std::string& filename){
    std::ofstream file(filename);
    if (!file.is_open()){
        throw std::runtime_error("Error: Could not open file for writing.");
    }
    file << "Life 1.06" << std::endl;
    file << "#N " << universe.getName() << std::endl;
    file << "#R " << universe.getRule() << std::endl;
    file << "#F " << universe.getField().getWidth() << "/"
         << universe.getField().getHeight() << std::endl;

    const Field& field = universe.getField();
    for (int i = 0; i < field.getWidth(); ++i){
        for (int j = 0; j < field.getHeight(); ++j){
            if (field.getCell(i, j) == CellState::Alive){
                file << i << " " << j << std::endl;
            }
        }
    }

    file.close();
}
