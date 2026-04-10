#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

std::vector<int> get_neighbors(int index, int width);

std::vector<std::tuple<int, int, int>> compute_bytes(std::string str, int width);

std::vector<std::tuple<int, int, int>> open_file();

