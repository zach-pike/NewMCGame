#include "GameServer/GameServer.hpp"

#include <iostream>
#include <string>

int main(int argc, char** argv) {
    GameServer gs;

    gs.startWorkers();

    std::string line;
    while(std::getline(std::cin, line) && gs.workersRunning()) {
        if (line.size() > 0) {
            if (line[0] == 'c') gs.stopWorkers();
        }
    }
}