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

StandardMesh* terrain = nullptr;
std::vector<Vector3D> pixelTrees;

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
                    pixelTrees.emplace_back(Vector3D{pixmap[idx].x, pixmap[idx].z, pixmap[idx].y});
                } else {
                    colorSet.insert(colorSet.end(), {0.0f, 0.0f, 1.0f});
                }

				int current_x = idx % width;
                int current_y = idx / width;
                
               	float u = (float) current_x;
				float v = (float) current_y;
                
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

void drawTerrain() {
	if (terrain != nullptr) {
        terrain->draw();
    }
}