#include "init_ptero.hpp"
#include "GLFW/glfw3.h"
#include <cmath>
#include "open_file.hpp"


extern GLBI_Engine myEngine;
extern IndexedMesh* sphereMesh;
extern IndexedMesh* cubeMesh;
extern StandardMesh* coneMesh;

void initPtero() {
    cubeMesh = STP3D::basicCube();
    cubeMesh->createVAO();
}


void renderPteroBody() {
    // CORPS
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addHomothety({0.7, 1.8, 0.5}); 
        myEngine.setFlatColor(0.25, 0.45, 0.15); 
        myEngine.updateMvMatrix();
        sphereMesh->draw();
    myEngine.mvMatrixStack.popMatrix();

    // CRETE (DOS)
    for (float y_pos = -0.8f; y_pos <= 1.0f; y_pos += 0.4f) {
        myEngine.mvMatrixStack.pushMatrix();
            myEngine.mvMatrixStack.addTranslation({0, y_pos, 0.35f}); 
            myEngine.mvMatrixStack.addRotation(M_PI_2, {1, 0, 0}); 
            myEngine.mvMatrixStack.addHomothety({0.12, 0.35, 0.55}); 
            myEngine.setFlatColor(0.10, 0.30, 0); 
            myEngine.updateMvMatrix();
            coneMesh->draw();
        myEngine.mvMatrixStack.popMatrix();
    }

    // COU
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation({0, 1.2f, 0.15f}); 
        myEngine.mvMatrixStack.addRotation(0.6f, {1, 0, 0}); 
        myEngine.mvMatrixStack.addTranslation({0, 0.35f, -0.2}); 
        myEngine.mvMatrixStack.addHomothety({0.3, 0.9, 0.3}); 
        myEngine.setFlatColor(0.25, 0.45, 0.15);
        myEngine.updateMvMatrix();
        sphereMesh->draw();
    myEngine.mvMatrixStack.popMatrix();
}


void renderPteroHead() {
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation({0, 2.1f, 0.66f}); 
        myEngine.mvMatrixStack.addRotation(-0.3f, {1, 0, 0}); 
        
        // CRANE
        myEngine.mvMatrixStack.pushMatrix();
            myEngine.mvMatrixStack.addHomothety({0.35, 0.75, 0.35}); 
            myEngine.setFlatColor(0.25, 0.45, 0.15); 
            myEngine.updateMvMatrix();
            sphereMesh->draw();
        myEngine.mvMatrixStack.popMatrix();

        // CORNE
        myEngine.mvMatrixStack.pushMatrix();
            myEngine.mvMatrixStack.addTranslation({0, -0.35f, 0}); 
            myEngine.mvMatrixStack.addRotation((M_PI / 2) + 1.2f, {1, 0, 0}); 
            myEngine.mvMatrixStack.addHomothety({0.25, 1.3, 0.25}); 
            myEngine.setFlatColor(0.10, 0.30, 0);
            myEngine.updateMvMatrix();
            coneMesh->draw();
        myEngine.mvMatrixStack.popMatrix();

        // BEC
        myEngine.mvMatrixStack.pushMatrix();
            myEngine.mvMatrixStack.addTranslation({0, 0.75f, 0});
            myEngine.mvMatrixStack.addRotation(M_PI_2, {1, 0, 0}); 
            myEngine.mvMatrixStack.addHomothety({0.15, 0.15, 1.5}); 
            myEngine.setFlatColor(0.8, 0.6, 0.3); 
            myEngine.updateMvMatrix();
            coneMesh->draw();
        myEngine.mvMatrixStack.popMatrix();
        
        // YEUX
        for(float side_eye : {-1.0f, 1.0f}) {
            myEngine.mvMatrixStack.pushMatrix();
                myEngine.mvMatrixStack.addTranslation({0.25f * side_eye, 0.3f, 0.2f});
                myEngine.mvMatrixStack.addHomothety(0.15f); 
                myEngine.setFlatColor(0.18, 0.08, 0.04);
                myEngine.updateMvMatrix();
                sphereMesh->draw();
            myEngine.mvMatrixStack.popMatrix();
        }
    myEngine.mvMatrixStack.popMatrix();
}

void renderPteroLegs() {
    for(float side : {-1.0f, 1.0f}) {
        myEngine.mvMatrixStack.pushMatrix();
            myEngine.mvMatrixStack.addTranslation({0.4f * side, -0.7f, -0.3f}); 
            myEngine.mvMatrixStack.addRotation(-0.15f * side, {0, 0, 1}); 
            myEngine.mvMatrixStack.addRotation(0.8f, {1, 0, 0}); 
            
            myEngine.mvMatrixStack.pushMatrix();
                myEngine.mvMatrixStack.addTranslation({0, -0.7f, 0.2}); 
                myEngine.mvMatrixStack.addHomothety({0.2, 1.4, 0.2}); 
                myEngine.setFlatColor(0.25, 0.45, 0.15); 
                myEngine.updateMvMatrix();
                cubeMesh->draw();
            myEngine.mvMatrixStack.popMatrix();

            myEngine.mvMatrixStack.pushMatrix();
                myEngine.mvMatrixStack.addTranslation({0, -1.4f, 0.2}); 
                myEngine.mvMatrixStack.addHomothety({0.25, 0.4, 0.1}); 
                myEngine.setFlatColor(0.10, 0.30, 0);
                myEngine.updateMvMatrix();
                cubeMesh->draw();
            myEngine.mvMatrixStack.popMatrix();
        myEngine.mvMatrixStack.popMatrix();
    }
}

void renderPteroWings(float wingAngle) {
    for(float side : {-1.0f, 1.0f}) {
        myEngine.mvMatrixStack.pushMatrix();
            myEngine.mvMatrixStack.addTranslation({0.4f * side, 0, 0}); 
            myEngine.mvMatrixStack.addRotation(wingAngle * side, {0, 1, 0}); 
            
            // OS PRINCIPAL
            myEngine.mvMatrixStack.pushMatrix();
                myEngine.mvMatrixStack.addTranslation({1.5f * side, 0.45f, 0}); 
                myEngine.mvMatrixStack.addHomothety({2.8, 0.15, 0.15}); 
                myEngine.setFlatColor(0.10, 0.30, 0); 
                myEngine.updateMvMatrix();
                cubeMesh->draw();
            myEngine.mvMatrixStack.popMatrix();

            // MEMBRANE PRINCIPALE
            myEngine.mvMatrixStack.pushMatrix();
                myEngine.mvMatrixStack.addTranslation({0.8f * side, 0, 0}); 
                myEngine.mvMatrixStack.addHomothety({1.6, 0.9, 0.1}); 
                myEngine.setFlatColor(0.45, 0.65, 0.35); 
                myEngine.updateMvMatrix();
                cubeMesh->draw();
            myEngine.mvMatrixStack.popMatrix();

            myEngine.mvMatrixStack.addTranslation({1.6f * side, 0, 0}); 

            // GRIFFES DES AILES
            for(int i : {0, 1, 2}) {
                myEngine.mvMatrixStack.pushMatrix();
                    myEngine.mvMatrixStack.addTranslation({1.3f * side, 0.4f, 0}); 
                    myEngine.mvMatrixStack.addRotation(0.35f * i * side, {0, 0, 1}); 
                    myEngine.mvMatrixStack.addRotation(M_PI_2, {1, 0, 0}); 
                    myEngine.mvMatrixStack.addHomothety({0.05, 0.15, 0.6}); 
                    myEngine.mvMatrixStack.addTranslation({0, 0, -0.3f});
                    myEngine.setFlatColor(0.10, 0.30, 0);
                    myEngine.updateMvMatrix();
                    coneMesh->draw();
                myEngine.mvMatrixStack.popMatrix();
            }

            // OS SECONDAIRE
            myEngine.mvMatrixStack.pushMatrix();
                myEngine.mvMatrixStack.addRotation(-0.9f * side, {0, 0, 1}); 
                myEngine.mvMatrixStack.addTranslation({1.5f * side, 1.25f, 0}); 
                myEngine.mvMatrixStack.addHomothety({2.8, 0.12, 0.12}); 
                myEngine.setFlatColor(0.10, 0.30, 0); 
                myEngine.updateMvMatrix();
                cubeMesh->draw();
            myEngine.mvMatrixStack.popMatrix();

            // JOINTURE
            myEngine.mvMatrixStack.pushMatrix();
                myEngine.mvMatrixStack.addRotation(-0.2f * side, {0, 0, 1}); 
                myEngine.mvMatrixStack.addRotation(M_PI_2 * side, {0, 1, 0}); 
                myEngine.mvMatrixStack.addTranslation({0, -0.5, 0});
                myEngine.mvMatrixStack.addHomothety({0.1, 0.8, 1.6}); 
                myEngine.setFlatColor(0.45, 0.65, 0.35);
                myEngine.updateMvMatrix();
                coneMesh->draw();
            myEngine.mvMatrixStack.popMatrix();

            // MEMBRANE SECONDAIRE
            myEngine.mvMatrixStack.pushMatrix();
                myEngine.mvMatrixStack.addRotation(-0.5f * side, {0, 0, 1}); 
                myEngine.mvMatrixStack.addTranslation({1.0f * side, 0, 0}); 
                myEngine.mvMatrixStack.addRotation(M_PI_2 * side, {0, 1, 0}); 
                myEngine.mvMatrixStack.addHomothety({0.1, 1.0, 2.0}); 
                myEngine.setFlatColor(0.45, 0.65, 0.35);
                myEngine.updateMvMatrix();
                coneMesh->draw();
            myEngine.mvMatrixStack.popMatrix();

        myEngine.mvMatrixStack.popMatrix();
    }
}


void renderPtero() {
    float time = glfwGetTime();
    float wingAngle = sin(time * 5.0f) * 0.6f; 

    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addHomothety({2.5, 2.5, 2.5}); 
        renderPteroBody();
        renderPteroHead();
        renderPteroLegs();
        renderPteroWings(wingAngle);
    myEngine.mvMatrixStack.popMatrix();
}

void shadePtero(Vector3D vector, float angle) {
    myEngine.switchToPhongShading();
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation(vector);
        myEngine.mvMatrixStack.addRotation(angle, Vector3D{0, 0, 1});
        myEngine.updateMvMatrix();
        renderPtero();
    myEngine.mvMatrixStack.popMatrix();
    myEngine.switchToFlatShading();
}