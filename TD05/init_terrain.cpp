#include "init_terrain.hpp"

/// Camera parameters
float angle_theta {45.0};      // Angle between x axis and viewpoint
float angle_phy {30.0};      // Angle between z axis and viewpoint
float dist_zoom {30.0};      // Distance between origin and viewpoint

float rotate {0.0};
float balance {0.0};
bool flag_anim_rot_scale {false};
bool flag_anim_rot_arm {false};

GLBI_Engine myEngine;
GLBI_Convex_2D_Shape ground{3};
GLBI_Set_Of_Points frame(3);

void initScene() {
	frame.initSet({0,0,0, 10,0,0,
				0,0,0, 0,10,0,
				0,0,0, 0,0,10},
				{1.0,0.0,0.0,
				1.0,0.0,0.0, 
				0.,1.0,0.0,
				0.,1.0,0.0,
				0.,0.0,1.0,
				0.,0.0,1.0});
	frame.changeNature(GL_LINES);
}

void drawFrame() {
	frame.drawSet();
}