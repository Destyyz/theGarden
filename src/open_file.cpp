#include "open_file.hpp"

std::vector<int> trees;
int height;
int width;
int gray;

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

std::vector<Vector3D> compute_bytes(std::string str, int width) {
    
    std::vector<Vector3D> pixmap;
    auto Sh = 0.004;
    auto Sp = 0.05;

    std::vector<unsigned int> elevations;
    for (auto i : str){
        auto val = static_cast<unsigned int>((unsigned char) i);
        elevations.emplace_back(val);
    }
    
    unsigned int min = elevations[0];
    for (auto val : elevations){
        min = val < min ? val : min;
    }
    //std::cout << "elevations : " << elevations.size() << std::endl;

    for (long unsigned int i = 0; i < elevations.size(); i++){
        auto x = i %  width;
        auto y = i / width;

        if (elevations[i] == 0){
            trees.emplace_back(i);
            std::cout << i << " : " << elevations[i] << std::endl;
            //trees.add[i]

            unsigned int avg = 0;
            auto neighbors = get_neighbors(i, width);
            for (auto neighbor : neighbors){
                avg += elevations[neighbor];
            }
            avg /= neighbors.size();
            elevations[i] = avg;
        }
        
        Vector3D tuple = Vector3D{x*Sp, elevations[i]*Sh - min, y*Sp};
        //std::cout << "(x, y, z) : " << std::get<0>(tuple) << " " << std::get<1>(tuple) << " " << std::get<2>(tuple) << std::endl;
        pixmap.emplace_back(tuple);
    }
    return pixmap;
}

std::vector<Vector3D> open_file(std::string file_name) {
    std::ifstream file(file_name);
    std::string str;

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