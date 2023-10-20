#include "game/game.hpp"

int main(int argc, char** argv) {
    // auto tbl = toml::parse_file("resources/blockDefinitions/baseGame.toml");

    // for (auto& a : *tbl["blocks"].as_table()) {
    //     std::cout << a.first << '\n';
    //     std::cout << (*a.second.as_table())["faces"]["bottom"] << '\n';
    // }

    Game game;
    game.gameLoop();

    return 0;
}