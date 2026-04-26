#include "sun_light.hpp"
#include "tools/shaders.hpp"
#include "open_file.hpp"
#include <vector>
#include <cmath>

extern GLBI_Engine myEngine;
extern IndexedMesh* sphereMesh;
extern int width;

void initSun() {
    sphereMesh = basicSphere(1);
    sphereMesh->createVAO();
}

void renderSun() {
    auto x = (width / 2.0f);
    auto y = (width / 2.0f);
    auto z = width + 50.0f;

    std::vector<float> color = {252/255.0f, 249/255.0f, 112/255.0f};
    myEngine.setFlatColor(color[0], color[1], color[2]);
    
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation(Vector3D(x, y, z));
        myEngine.mvMatrixStack.addHomothety({30, 30, 30});
        myEngine.switchToPhongShading();
        myEngine.setLightPosition(Vector4D(0, 0, 1, 0.0), 0);
        myEngine.setLightIntensity(Vector3D(color[0], color[1], color[2]), 0);
        myEngine.switchToFlatShading();
        myEngine.updateMvMatrix();
        sphereMesh->draw();
    myEngine.mvMatrixStack.popMatrix();
    
    myEngine.setFlatColor(0., 0., 0.);
}

void renderPointLight(Vector3D vector, float angle) {
    std::vector<float> color = {1.0f, 0.0f, 0.0f};
    
    float x_offset = 0.0f;
    float y_offset = -5.0f;
    float z_offset = -4.0f;

    float rotated_x = x_offset * cos(angle) - y_offset * sin(angle);
    float rotated_y = x_offset * sin(angle) + y_offset * cos(angle);
    auto offset = Vector3D{vector.x + rotated_x, vector.y + rotated_y, vector.z + z_offset};

    myEngine.setFlatColor(color[0], color[1], color[2]);
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation(offset);
        myEngine.switchToPhongShading();
        myEngine.setLightPosition(Vector4D(offset, 1.0), 1);
        myEngine.setLightIntensity(Vector3D(1000.0f * color[0], 0.0f, 0.0f), 1);
        myEngine.switchToFlatShading();
        myEngine.updateMvMatrix();
        sphereMesh->draw();
    myEngine.mvMatrixStack.popMatrix();
}