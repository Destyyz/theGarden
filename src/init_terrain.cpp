#include "init_terrain.hpp"
#include "open_file.hpp"

/// Camera parameters
float angle_theta {45.0};      // Angle between x axis and viewpoint
float angle_phy {30.0};      // Angle between z axis and viewpoint
float dist_zoom {30.0};      // Distance between origin and viewpoint

float rotate {0.0};
float balance {0.0};
bool flag_anim_rot_scale {false};
bool flag_anim_rot_arm {false};

GLBI_Engine myEngine;
GLBI_Set_Of_Points frame(3);
GLBI_Set_Of_Points terrain(3);


void initTerrain(std::vector<Vector3D> pixmap){
	std::vector<float> terrainSet;
	std::vector<float> colorSet;
	for (auto index = 0; index < pixmap.size()/4; index++){
		int x = index % width;
		int y = index / width;

		int directions[4][2] = {{1, 0}, {0, 0}, {1, 1}, {1, 0}};

		for (auto& dir : directions) {
			int nline = x + dir[0];
			int ncolumn = y + dir[1];

			if (ncolumn >= 0 && ncolumn < width && nline >= 0 && nline < width) {
				terrainSet.emplace_back(pixmap[nline * width + ncolumn].x);
				terrainSet.emplace_back(pixmap[nline * width + ncolumn].z);
				terrainSet.emplace_back(pixmap[nline * width + ncolumn].y);
				if (index == trees[0]){
					colorSet.emplace_back(1);
					colorSet.emplace_back(1);
					colorSet.emplace_back(1);
					trees.erase(trees.begin());
				} else {
					colorSet.emplace_back(0);
					colorSet.emplace_back(0);
					colorSet.emplace_back(1);
				}
			}
		}
	}
	terrain.initSet(terrainSet, colorSet);
	terrain.changeNature(GL_TRIANGLE_STRIP);
}

void initScene(std::vector<Vector3D> pixmap) {
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

	/*
    std::vector<float> terrainSet;
	std::vector<float> colorSet;
	int index = 0;
    for (auto& vec : pixmap){
		terrainSet.emplace_back(vec.x);
		terrainSet.emplace_back(vec.z);
		terrainSet.emplace_back(vec.y);

		if (index == trees[0]){
			colorSet.emplace_back(1);
			colorSet.emplace_back(1);
			colorSet.emplace_back(1);
			trees.erase(trees.begin());
		} else {
			colorSet.emplace_back(1);
			colorSet.emplace_back(0);
			colorSet.emplace_back(0);
		}

		//std::cout << "(x, y, z) : " << vec.x << " " << vec.y << " " << vec.z << std::endl;
		index++;
    }

    terrain.initSet(terrainSet, colorSet);
	terrain.changeNature(GL_TRIANGLES);
	*/
	initTerrain(pixmap);
}

void drawFrame() {
	frame.drawSet();
}

void drawTerrain() {
	terrain.drawSet();
}