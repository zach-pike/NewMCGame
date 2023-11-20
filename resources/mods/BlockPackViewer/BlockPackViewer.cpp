#include "BlockPackViewer.hpp"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

BlockPackViewer* create() {
    return new BlockPackViewer();
}

void destroy(BlockPackViewer* mod) {
    delete mod;
}

void BlockPackViewer::setup(Game& game) {
}

void BlockPackViewer::frameUpdate(Game& game) {
    ImGui::Begin("BlockPackViewer");


    ImGui::End();
}

void BlockPackViewer::cleanup(Game& game) {

}

std::string BlockPackViewer::getModName() {
    return "BlockPackViewer";
}

IMod::Version BlockPackViewer::getModVersion() {
    return Version { 1, 0 };
}