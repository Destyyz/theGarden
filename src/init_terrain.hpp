#pragma once
#include "../third_party/glbasimac/glbasimac/glbi_engine.hpp"
#include "../third_party/glbasimac/glbasimac/glbi_set_of_points.hpp"
#include "../third_party/glbasimac/glbasimac/glbi_convex_2D_shape.hpp"
#include "../third_party/glbasimac/tools/basic_mesh.hpp"
#include "../third_party/glbasimac/tools/mesh.hpp"

using namespace glbasimac;
using namespace STP3D;

/* Camera parameters and functions */
static const float Z_NEAR {0.1f};
static const float Z_FAR {500.f};
extern float angle_theta;      // Angle between x axis and viewpoint
extern float angle_phy  ;      // Angle between z axis and viewpoint
extern float dist_zoom  ;      // Distance between origin and viewpoint

extern bool flag_anim_rot_scale;
extern bool flag_anim_rot_arm;

extern std::vector<int> trees;
extern std::vector<Vector3D> pixelTrees;

extern int width;
extern int height;

extern StandardMesh* terrain;

void initTerrain(std::vector<Vector3D> pixmap);

void drawTerrain();

void drawAllTrees();

void volcanoGrid(int res_tour, int res_haut, int nb_lines, int res_int, std::vector<std::vector<float>>& grille_R, std::vector<std::vector<float>>& grille_H);

void buildVolcMesh(int res_tour, int nb_lines, 
                    const std::vector<std::vector<float>>& grille_R, 
                    const std::vector<std::vector<float>>& grille_H, 
                    std::vector<float>& volcanoSet, std::vector<float>& normalSet, 
                    std::vector<float>& colorSet, std::vector<float>& uvSet);

void initVolcano(int res_tour, int res_haut);

void drawVolcano();