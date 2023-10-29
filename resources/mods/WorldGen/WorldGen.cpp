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

    ImGui::Combo("World Generator", &currentSelectedGenerator, opts, IM_ARRAYSIZE(opts));

    ImGui::SliderInt("Chunk size X", &newChunkSx, 1, 50);
    ImGui::SliderInt("Chunk size Y", &newChunkSy, 1, 50);
    ImGui::SliderInt("Chunk size Z", &newChunkSz, 1, 50);

    switch (currentSelectedGenerator) {
        // Perlin Noise generator
        case 0: {
            ImGui::SliderFloat("X stretch", &xStretch, .001, .1);
            ImGui::SliderFloat("Z stretch", &zStretch, .001, .1);

            ImGui::SliderFloat("Y Scale", &yScale, 0.f, (newChunkSy - 1) * 16);

            if (ImGui::Button("Generate!") && !generatorWorker.isWorkerRunning()) {
                generatorWorker.generatePerlinNoiseWorld(game.getWorldRef().getBlockDBRef(), WorldGenWorker::PerlinNoiseWorldGenDetails{
                    .xStretch = xStretch,
                    .zStretch = zStretch,
                    .yScale = yScale,

                    .chunksX = newChunkSx,
                    .chunksY = newChunkSy,
                    .chunksZ = newChunkSz,
                    
                    .seed = (uint32_t)rand()
                });
            }
        } break;
        
        // Cos world generator
        case 1: {
            ImGui::SliderFloat("Y Scale", &yScale, 0, newChunkSy*16-1);
            ImGui::SliderFloat("Horiz Stretch", &horizStretch, .001, 1);

            if (ImGui::Button("Generate!") && !generatorWorker.isWorkerRunning()) {
                generatorWorker.generateSineWaveWorld(game.getWorldRef().getBlockDBRef(), WorldGenWorker::SineWorldGenDetails{
                    .chunksX = newChunkSx,
                    .chunksY = newChunkSy,
                    .chunksZ = newChunkSz,
                    .scale = yScale,
                    .horizStretch = horizStretch
                });
            }
        } break;
    }

    if (generatorWorker.isWorkerRunning()) ImGui::Text("Generating world...");


    // If the generated world is ready, move the chunks into the game world
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