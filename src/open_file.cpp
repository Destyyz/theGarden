#include "open_file.hpp"

std::vector<int> get_neighbors(int index, int width) {
    std::vector<int> neighbors;
    int x = index % width;
    int y = index / width;

    int directions[4][2] = {{0, -1}, {0, 1}, {1, 0}, {-1, 0}};

    for (auto& dir : directions) {
        int nx = x + dir[0];
        int ny = y + dir[1];

        if (nx >= 0 && nx < width && ny >= 0 && ny < width) {
            neighbors.push_back(ny * width + nx);
        }
    }
    return neighbors;
}

std::vector<std::tuple<int, int, int>> compute_bytes(std::string str, int width) {
    std::vector<std::tuple<int, int, int>> tuple_list;

    std::vector<unsigned int> list;
    for (auto i : str){
        auto val = static_cast<unsigned int>((unsigned char) i);
        list.emplace_back(val);
    }

    unsigned int min = list[0];
    for (auto val : list){
        min = val < min ? val : min;
    }

    for (auto i = 0; i < list.size(); i++){
        auto x = i %  width;
        auto y = i / width;
        std::tuple<int, int, int> tuple = std::make_tuple(x, list[i]*10 - min, y);
        tuple_list.emplace_back(tuple);
    }

    for (auto i = 0; i < tuple_list.size(); i++){
        if (std::get<1>(tuple_list[i]) == 0){
            unsigned int avg = 0;
            auto neighbors = get_neighbors(i, width);
            for (auto neighbor : neighbors){
                avg += std::get<1>(tuple_list[neighbor]);
            }
            avg /= 4;
            std::get<1>(tuple_list[i]) = avg;
        }
    }
    return tuple_list;
}

std::vector<std::tuple<int, int, int>> open_file(std::string file_name) {
    std::ifstream file(file_name);
    std::string str;

    int height = 0;
    int width = 0;
    int gray = 0;

    int line = 0;
    while (std::getline(file, str) && line < 3)
    {
        if (line == 0 && str != "P5"){
            std::cout << "Bouuuuuh" << std::endl;
        }
        
        if (str[0] == '#'){
            continue;
        }

        if (line == 1){
            height = std::stof(str);
            width = std::stof(str);
            std::cout << "Height : " << height << std::endl;
            std::cout << "Width : " << width << std::endl;
        }

        if (line == 2){
            gray = std::stof(str);
            std::cout << "Gray scale : [0 - " << gray << "]" << std::endl;
        }
        line++;
    }
    std::getline(file, str);
    return compute_bytes(std::move(str), width);
}

int main(){
    open_file("../assets/terrain_copy.pgm");
    return 0;
}