#include "WorldGen.hpp"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

NewWorldGen* create() {
    return new NewWorldGen();
}

void destroy(NewWorldGen* mod) {
    delete mod;
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

    if (ImGui::Button("Generate!")) {
        generatorWorker.startWorldGenerator(game.getWorldRef().getBlockDBRef(), WorldGenWorker::WorldGenDetails{
            .xStretch = xStretch,
            .zStrecth = zStretch,
            .yScale = yScale,

            .chunksX = newChunkSx,
            .chunksY = newChunkSy,
            .chunksZ = newChunkSz,
            
            .seed = (uint32_t)rand()
        });
    }

    if (generatorWorker.isWorldReady()) {
        game.getWorldRef().moveChunks(generatorWorker.getGeneratedWorld());
        generatorWorker.clearWorldReady();
    }

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