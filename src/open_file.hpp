#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include "../third_party/glbasimac/glbasimac/glbi_engine.hpp"
#include "../third_party/glbasimac/glbasimac/glbi_set_of_points.hpp"
#include "../third_party/glbasimac/glbasimac/glbi_convex_2D_shape.hpp"
#include "../third_party/glbasimac/tools/basic_mesh.hpp"


extern std::vector<int> trees;
extern int height, width;
extern float Sh, Sp;

std::vector<int> get_neighbors(int index, int width);

std::vector<Vector3D> compute_bytes(std::string str, int width);

std::vector<Vector3D> open_file(std::string name);

