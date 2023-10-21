#include "WorldGen.hpp"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include "PerlinNoise.hpp"

IMod* create() {
    return new NewWorldGen();
}

void destroy(IMod* mod) {
    delete mod;
}

void NewWorldGen::worldGen(World& world) {
    world.generateEmptyMap(newChunkSx, newChunkSy, newChunkSz);

    const auto& blockDB = world.getBlockDBRef();

    int dirtBlockID = blockDB.getIdByName("dirt");
    int grassBlockID = blockDB.getIdByName("grass");

    srand(time(nullptr));
    const siv::PerlinNoise::seed_type seed = rand();
	const siv::PerlinNoise perlin{ seed };

    for (int z=0; z<newChunkSz*16; z++) {
        for (int x=0; x<newChunkSx*16; x++) {
            const double yMax = perlin.octave2D_01((x * xStretch), (z * zStretch), 2) * yScale;

            for(int y=0; y < yMax; y++) world.setBlock(glm::vec3(x, y, z), Block(dirtBlockID));
            world.setBlock(glm::vec3(x, yMax, z), Block(grassBlockID), true);
        }
    }
}

void NewWorldGen::setup(Game& game) {
}

void NewWorldGen::frameUpdate(Game& game) {
    ImGui::Begin("BaseWorldGen", &windowOpen);
    ImGui::SliderInt("Chunk size X", &newChunkSx, 1, 20);
    ImGui::SliderInt("Chunk size Y", &newChunkSy, 1, 20);
    ImGui::SliderInt("Chunk size Z", &newChunkSz, 1, 20);

    ImGui::SliderFloat("X stretch", &xStretch, .001, .1);
    ImGui::SliderFloat("Z stretch", &zStretch, .001, .1);

    ImGui::SliderFloat("Y Scale", &yScale, 0.f, (newChunkSy - 1) * 16);

    if (ImGui::Button("Generate!")) worldGen(game.getWorldRef());

    ImGui::End();
}

void NewWorldGen::cleanup(Game& game) {

}

std::string NewWorldGen::getModName() {
    return "NewWorldGen";
}

IMod::Version NewWorldGen::getModVersion() {
    return Version { 1, 0 };
}