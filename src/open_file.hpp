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
extern int height;
extern int width;

std::vector<int> get_neighbors(int index, int width);

/**
 *  Extrait les octets de données du fichier PGM 
 * et les ajoute dans un tableau sous la forme (x, élévation, y)
 *
 * @param str les données contenues dans le fichier PGM
 * @param width la largeur de l'image PGM
 * @return Un tableau contenant un tuple(x, elevation, y) pour tout point du fichier PGM
 */
std::vector<Vector3D> compute_bytes(std::string str, int width);

/**
 * Ouvre et lit le fichier PGM, dont le nom est donné en paramètre
 * Extrait de ce fichier la hauteur, largeur, échelle de gris du terrain et ses données.
 *
 * @param file_name le nom du fichier PGM
 * @return tableau contenant un tuple(x, elevation, y) pour tout point du fichier PGM
 */
std::vector<Vector3D> open_file(std::string name);

