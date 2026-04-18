#include "init_terrain.hpp"
#include "open_file.hpp"
#include <algorithm>

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
StandardMesh* terrain = nullptr;


void initTerrain(std::vector<Vector3D> pixmap) {
    std::vector<float> terrainSet;
    std::vector<float> colorSet;
	std::vector<float> uvSet;
    
    for (int y = 0; y < height - 1; y++) {
        for (int x = 0; x < width - 1; x++) {
            int i0 = y * width + x;
            int i1 = (y + 1) * width + x;
            int i2 = y * width + (x + 1);
            int i3 = (y + 1) * width + (x + 1);

            // triangle 1 : i0, i1, i2
            // triangle 2 : i2, i1, i3
            int indices[6] = {i0, i1, i2, i2, i1, i3};

            for (int idx : indices) {
                terrainSet.push_back(pixmap[idx].x);
                terrainSet.push_back(pixmap[idx].z);
                terrainSet.push_back(pixmap[idx].y);

                if (std::find(trees.begin(), trees.end(), idx) != trees.end()) {
                    colorSet.insert(colorSet.end(), {1.0f, 1.0f, 1.0f});
                } else {
                    colorSet.insert(colorSet.end(), {0.0f, 0.0f, 1.0f});
                }

				int current_x = idx % width;
                int current_y = idx / width;
                
               	float u = (float)current_x;
				float v = (float)current_y;
                
                uvSet.push_back(u);
                uvSet.push_back(v);
            }
        }
    }

    // terrain.initSet(terrainSet, colorSet);
    // terrain.changeNature(GL_TRIANGLES); 

	unsigned int nb_vertices = terrainSet.size() / 3;
	terrain = new StandardMesh(nb_vertices, GL_TRIANGLES);

	terrain->addOneBuffer(0, 3, terrainSet.data(), "position", true);
    terrain->addOneBuffer(1, 3, colorSet.data(), "color", true); 
    terrain->addOneBuffer(2, 2, uvSet.data(), "texcoord", true);
	
	terrain->createVAO();
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
	if (terrain != nullptr) {
        terrain->draw();
    }
}