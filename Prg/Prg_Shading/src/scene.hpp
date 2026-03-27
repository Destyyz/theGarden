#pragma once

#include <vector>
#include "globject.hpp"

class Scene {
    public:
        Scene() : nb_object(0) {};
        ~Scene();

        void addOneObject(unsigned int typ_obj,float* diffuse,float* trans,
                          float rot,int typ_rot,float scale);

        void createScene();
        void drawScene(float current_time);
        static void makeRot(int trot,float arot);
    private:
        int nb_object;
        std::vector<float> translations;
        std::vector<float> rotation_angles;
        std::vector<int> rotation_axes;
        std::vector<float> scales;
        std::vector<GLObject*> les_objets;
};
