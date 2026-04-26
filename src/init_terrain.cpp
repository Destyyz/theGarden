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
StandardMesh* volcanoMesh = nullptr;
std::vector<Vector3D> pixelTrees;

void initTerrain(std::vector<Vector3D> pixmap) {
    std::vector<float> terrainSet;
    std::vector<float> colorSet;
	std::vector<float> uvSet;
    std::vector<float> normalSet;
    
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

				float current_u = (float) (idx % width);
                int current_v = (float) (idx / width);
                uvSet.push_back(current_u);
                uvSet.push_back(current_v);


                auto haut = (y-1) < 0 ? pixmap[y * width + x].z : pixmap[(y-1) * width + x].z;
                auto bas = (y+1) > width ? pixmap[y * width + x].z : pixmap[(y+1) * width + x].z;
                auto gauche = (x-1) < 0 ? pixmap[y * width + x].z : pixmap[y * width + (x-1)].z;
                auto droite = (x+1) > width ? pixmap[y * width + x].z : pixmap[y * width + (x+1)].z;

                auto normalX = (haut - bas)*Sh / 2*Sp;
                auto normalY = (gauche - droite)*Sh / 2*Sp;
                auto normalZ = 1.f;
                auto normal = Vector3D{normalX, normalY, normalZ};
                normal.normalize();

                normalSet.push_back(normal.x);
                normalSet.push_back(normal.z);
                normalSet.push_back(normal.y);
            }
        }
    }
	unsigned int nb_vertices = terrainSet.size() / 3;
	terrain = new StandardMesh(nb_vertices, GL_TRIANGLES);
	terrain->addOneBuffer(0, 3, terrainSet.data(), "position", true);
    terrain->addOneBuffer(1, 3, colorSet.data(), "color", true);
    terrain->addOneBuffer(2, 2, uvSet.data(), "texcoord", true);
    terrain->addOneBuffer(3, 3, normalSet.data(), "normal", true);
    
	terrain->createVAO();
}

void drawTerrain() {
	if (terrain != nullptr) {
        terrain->draw();
    }
}

void volcanoGrid(int res_tour, int res_haut, int nb_lines, int res_int, std::vector<std::vector<float>>& grille_R, std::vector<std::vector<float>>& grille_H) {
    float haut_max = 8.0f;
    float rayon_base = 5.0f;
    float rayon_sommet = 1.0f; 
    float epaisseur = 0.1f;
    float profondeur = 4.0f;

    for (int y = 0; y < nb_lines; y++) {
        float rayon, hauteur;
        if (y < res_haut) {
            float progression = (float)y / (float)(res_haut - 1);
            rayon = rayon_base * (1.0f - progression) + rayon_sommet * progression;
            hauteur = progression * haut_max;
        } else {
            int y_interieur = y - res_haut; 
            float progression = (float)y_interieur / (float)(res_int - 1);
            rayon = (rayon_sommet - epaisseur) * (1.0f - progression) + (rayon_sommet - epaisseur - 0.4f) * progression;
            hauteur = haut_max - (progression * profondeur);
        }
        
        for (int x = 0; x < res_tour; x++) {
            float angle = ((float)x / res_tour) * 2.0f * M_PI;
            float onde1 = sin(angle * 12.0f) * cos(hauteur * 2.5f) * 0.5f;
            float onde2 = sin(angle * 25.0f + hauteur * 1.5f) * 0.25f;
            float bruit = (onde1 + onde2) * 0.4f;
            
            grille_R[y][x] = rayon + bruit;
            grille_H[y][x] = hauteur;
        }
    }
}

void buildVolcMesh(int res_tour, int nb_lines, 
                    const std::vector<std::vector<float>>& grille_R, 
                    const std::vector<std::vector<float>>& grille_H, 
                    std::vector<float>& volcanoSet, std::vector<float>& normalSet, 
                    std::vector<float>& colorSet, std::vector<float>& uvSet) {
    
    auto getPos = [&](int x, int y) {
        float angle = ((float)x / res_tour) * 2.0f * M_PI;
        float r = grille_R[y][x];
        float h = grille_H[y][x];
        return std::vector<float>{ (float)(r * cos(angle)), (float)(r * sin(angle)), h };
    };

    auto calcNormal = [](const std::vector<float>& pA, const std::vector<float>& pB, const std::vector<float>& pC) {
        float u[3] = {pB[0]-pA[0], pB[1]-pA[1], pB[2]-pA[2]};
        float v[3] = {pC[0]-pA[0], pC[1]-pA[1], pC[2]-pA[2]};
        float nx = u[1]*v[2] - u[2]*v[1];
        float ny = u[2]*v[0] - u[0]*v[2];
        float nz = u[0]*v[1] - u[1]*v[0];
        float len = sqrt(nx*nx + ny*ny + nz*nz);
        if(len == 0.0f) len = 1.0f;
        return std::vector<float>{nx/len, ny/len, nz/len};
    };

    auto pushVertex = [&](const std::vector<float>& p, const std::vector<float>& n, float u, float v) {
        volcanoSet.insert(volcanoSet.end(), p.begin(), p.end());
        normalSet.insert(normalSet.end(), n.begin(), n.end());
        colorSet.insert(colorSet.end(), {1.0f, 1.0f, 1.0f}); 
        uvSet.push_back(u);
        uvSet.push_back(v);
    };

    for (int y = 0; y < nb_lines - 1; y++) {
        for (int x = 0; x < res_tour; x++) {
            int next_x = (x + 1) % res_tour;

            float u0 = (float)x / res_tour * 5.0f;
            float u1 = (float)(x + 1) / res_tour * 5.0f;
            float v0 = (float)y / (nb_lines - 1) * 5.0f;
            float v1 = (float)(y + 1) / (nb_lines - 1) * 5.0f;

            std::vector<float> p0 = getPos(x, y);
            std::vector<float> p1 = getPos(x, y + 1);
            std::vector<float> p2 = getPos(next_x, y);
            std::vector<float> p3 = getPos(next_x, y + 1);

            std::vector<float> n1 = calcNormal(p0, p2, p1);
            pushVertex(p0, n1, u0, v0);
            pushVertex(p2, n1, u1, v0);
            pushVertex(p1, n1, u0, v1);

            std::vector<float> n2 = calcNormal(p2, p3, p1);
            pushVertex(p2, n2, u1, v0);
            pushVertex(p3, n2, u1, v1);
            pushVertex(p1, n2, u0, v1);
        }
    }
}

void initVolcano(int res_tour, int res_haut) {
    int res_int = 15;
    int nb_lines = res_haut + res_int;

    std::vector<std::vector<float>> grille_R(nb_lines, std::vector<float>(res_tour));
    std::vector<std::vector<float>> grille_H(nb_lines, std::vector<float>(res_tour));
    std::vector<float> volcanoSet, normalSet, colorSet, uvSet;

    volcanoGrid(res_tour, res_haut, nb_lines, res_int, grille_R, grille_H);
    buildVolcMesh(res_tour, nb_lines, grille_R, grille_H, volcanoSet, normalSet, colorSet, uvSet);

    unsigned int nb_vertices = volcanoSet.size() / 3;
    volcanoMesh = new StandardMesh(nb_vertices, GL_TRIANGLES);

    volcanoMesh->addOneBuffer(0, 3, volcanoSet.data(), "position", true);
    volcanoMesh->addOneBuffer(1, 3, colorSet.data(), "color", true);
    volcanoMesh->addOneBuffer(2, 2, uvSet.data(), "texcoord", true);
    volcanoMesh->addOneBuffer(3, 3, normalSet.data(), "normal", true);

    volcanoMesh->createVAO();
}


void drawVolcano() {
    if (volcanoMesh != nullptr) {
        volcanoMesh->draw();
    }
}