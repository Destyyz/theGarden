#pragma once
#include "../third_party/glbasimac/glbasimac/glbi_engine.hpp"
#include "../third_party/glbasimac/glbasimac/glbi_set_of_points.hpp"
#include "../third_party/glbasimac/glbasimac/glbi_convex_2D_shape.hpp"
#include "../third_party/glbasimac/tools/basic_mesh.hpp"

using namespace glbasimac;

/* Camera parameters and functions */
static const float Z_NEAR {0.1f};
static const float Z_FAR {500.f};
extern float angle_theta;      // Angle between x axis and viewpoint
extern float angle_phy  ;      // Angle between z axis and viewpoint
extern float dist_zoom  ;      // Distance between origin and viewpoint

extern bool flag_anim_rot_scale;
extern bool flag_anim_rot_arm;

/* OpenGL Engine */
extern GLBI_Engine myEngine;

extern std::vector<int> trees;
extern int width;

void initTerrain(std::vector<Vector3D> pixmap);

void initScene(std::vector<Vector3D> pixmap);

void drawFrame();

void drawTerrain();