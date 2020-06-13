#include "icosphere/fast_icosphere.h"
#include "polyscope/combining_hash_functions.h"
#include "polyscope/messages.h"
#include "polyscope/polyscope.h"

#include "polyscope/surface_mesh.h"

#include <chrono>
#include <iostream>
#include <map>
#include <unordered_set>
#include <utility>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#include "stb_image.h"


void GenerateGeometry(const int num_additional, const bool project) {
  std::vector<std::array<double, 3>> verts;
  std::vector<std::vector<size_t>> faces;
  std::tie(verts, faces) = icosphere::FastIcoSphere(num_additional, project);
  std::vector<glm::vec3> vertexPositionsGLM;
  for (std::array<double, 3> p : verts) {
    vertexPositionsGLM.push_back(glm::vec3{p[0], p[1], p[2]});
  }
  auto psMesh = polyscope::registerSurfaceMesh("split-icosahedron", vertexPositionsGLM, faces);
}

void callback() {
  static int numSplits = 2;
  static int project = 1;
  ImGui::PushItemWidth(100);
  ImGui::InputInt("Num points on original icosahedron edge", &numSplits);
  ImGui::InputInt("Project to sphere (>0) or just supdivide", &project);
  if (ImGui::Button("run subroutine")) {
    auto start = std::chrono::high_resolution_clock::now();
    GenerateGeometry(numSplits, project);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Time of geometry with num splits: " << numSplits << " in ms: " << duration.count() << "ms\n";
  }

  ImGui::PopItemWidth();
}

int main() {
  // Initialize polyscope
  polyscope::init();

  // Add a few gui elements
  polyscope::state::userCallback = callback;

  // Show the gui
  polyscope::show();

  return 0;
}
