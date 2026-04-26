#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "glbasimac/glbi_engine.hpp"
#include "glbasimac/glbi_texture.hpp"
#include "init_terrain.hpp"
#include "open_file.hpp"
#include "tools/shaders.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "tools/stb_image.h"
#include <iostream>

using namespace glbasimac;
using namespace STP3D;

/* Window properties */
static const unsigned int WINDOW_WIDTH = 1200;
static const unsigned int WINDOW_HEIGHT = 800;
static const char WINDOW_TITLE[] = "TD05 Ex01";
static float aspectRatio = 1.0f;

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 30.;

extern float Sh, Sp;
extern int width;
extern std::vector<Vector3D> pixelTrees;

// Pour le ptérodactyle RAWR
IndexedMesh* sphereMesh = nullptr;
IndexedMesh* cubeMesh = nullptr;
IndexedMesh* cylinderMesh = nullptr;
StandardMesh* coneMesh = nullptr;


/* 3D Engine global variables */
StandardMesh* a_frame;
GLBI_Texture myTexture;
GLBI_Engine myEngine;
GLBI_Texture rockTexture;
GLBI_Texture lavaTexture;

bool flag_rotation = false;

// Camera parameters
Vector3D pos_camera = Vector3D(50.0, 0, 50.0); // Position of the camera
float angle_horizontal{90};					 // Angle between x axis and viewpoint
float angle_vertical{-10};						 // Angle between z axis and viewpoint
float speed{1.0};								 // Camera movement speed
float day_speed = 20;

/* Error handling function */
void onError(int error, const char* description) {
	std::cout << "GLFW Error ("<<error<<") : " << description << std::endl;
}

void onWindowResized(GLFWwindow* /*window*/, int width, int height)
{
	aspectRatio = width / (float) height;

	glViewport(0, 0, width, height);
	std::cerr<<"Setting 3D projection"<<std::endl;
	myEngine.set3DProjection(60.0,aspectRatio,Z_NEAR,Z_FAR);
}

void onKey(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
{
	int is_pressed = (action == GLFW_PRESS); 
	switch(key) {
		//case GLFW_KEY_A :
		case GLFW_KEY_ESCAPE :
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;
		case GLFW_KEY_L:
			if (is_pressed) glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			break;
		case GLFW_KEY_P:
			if (is_pressed) glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			break;
        case GLFW_KEY_UP :
            angle_phy += 1.0;
            break;
        case GLFW_KEY_DOWN :
            angle_phy -= 1.0;
            break;
        case GLFW_KEY_I :
            dist_zoom *= 0.95;
            break;
        case GLFW_KEY_K :
            dist_zoom *= 1.05;
            break;
        case GLFW_KEY_LEFT :
            angle_theta += 1.0;
            break;
        case GLFW_KEY_RIGHT :
            angle_theta -= 1.0;
            break;
	}

}

void onMouseButton(GLFWwindow* window, int button, int action, int /*mods*/)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		std::cout<<"Pressed in "<<xpos<<" "<<ypos<<std::endl;

	}
}

void movement(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		angle_vertical += 1.0f * speed;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		angle_vertical -= 1.0f * speed;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		angle_horizontal += 1.0f * speed;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		angle_horizontal -= 1.0f * speed;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		pos_camera[0] += cos(deg2rad(angle_horizontal)) * speed;
		pos_camera[1] += sin(deg2rad(angle_horizontal)) * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		pos_camera[0] -= cos(deg2rad(angle_horizontal)) * speed;
		pos_camera[1] -= sin(deg2rad(angle_horizontal)) * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		pos_camera[0] -= sin(deg2rad(angle_horizontal)) * speed;
		pos_camera[1] += cos(deg2rad(angle_horizontal)) * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		pos_camera[0] += sin(deg2rad(angle_horizontal)) * speed;
		pos_camera[1] -= cos(deg2rad(angle_horizontal)) * speed;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		pos_camera[2] -= 1.0 * speed;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		pos_camera[2] += 1.0 * speed;
}

void initTree(){
	coneMesh = basicCone(1, 1);
	coneMesh->createVAO();
	cylinderMesh = STP3D::basicCylinder(1, 1);
	cylinderMesh->createVAO();
}

void initFrame(){
	a_frame = createRepere(10.0);
	a_frame->createVAO();
}

void initSun(){
	sphereMesh = basicSphere(1);
	sphereMesh->createVAO();
}

void initPtero(){
    cubeMesh = STP3D::basicCube();
    cubeMesh->createVAO();
}

void initBasicScene() {
	auto pixmap = open_file("../assets/terrain_copy.pgm");

	initTerrain(pixmap);

	initTree();
	initFrame();
	initSun();
    initPtero();
	// volcan :
	initVolcano(150, 75); 
	
	glActiveTexture(GL_TEXTURE0);
	//Load de l'image
	int img_width, img_height, img_channels;
	auto image = stbi_load("../assets/herbe.png", &img_width, &img_height, &img_channels, 0);
	if (image != nullptr){
		std::cout << "Image chargée correctement" << std::endl;
	}

	myEngine.switchToPhongShading();
	myEngine.addALight(Vector4D{1., 1., 1., 0.}, Vector3D{1., 1., 1.});
    myEngine.addALight(Vector4D{1., 1., 1., 1.}, Vector3D{1., 1., 1.});
	myEngine.switchToFlatShading();

	myTexture.createTexture();
	myTexture.attachTexture();
	myTexture.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	myTexture.loadImage(img_width, img_height, img_channels, image);
	myTexture.detachTexture();
	stbi_image_free(image);

	// volcan :
	auto image_rock = stbi_load("../assets/volcano_rock.jpg", &img_width, &img_height, &img_channels, 0);
    if (image_rock != nullptr){
        std::cout << "Texture roche chargée correctement" << std::endl;
        rockTexture.createTexture();
        rockTexture.attachTexture();
        rockTexture.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        rockTexture.setParameters(GL_TEXTURE_WRAP_S, GL_REPEAT);
        rockTexture.setParameters(GL_TEXTURE_WRAP_T, GL_REPEAT);
        rockTexture.loadImage(img_width, img_height, img_channels, image_rock);
        rockTexture.detachTexture();
        stbi_image_free(image_rock);
    }

	// lave volcan :
	auto image_lava = stbi_load("../assets/lava.png", &img_width, &img_height, &img_channels, 0);
    if (image_lava != nullptr){
        std::cout << "Texture lave chargee correctement" << std::endl;
        lavaTexture.createTexture();
        lavaTexture.attachTexture();
        lavaTexture.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        lavaTexture.setParameters(GL_TEXTURE_WRAP_S, GL_REPEAT);
        lavaTexture.setParameters(GL_TEXTURE_WRAP_T, GL_REPEAT);
        lavaTexture.loadImage(img_width, img_height, img_channels, image_lava);
        lavaTexture.detachTexture();
        stbi_image_free(image_lava);
    }
}

void renderSun(){
	// auto angle = glfwGetTime() * (day_speed/100);
	// auto x = cos(angle)*(width+50) + (width/2);
	// auto y = (width/2);
	// auto z = sin(angle)*(width+50);

	auto x = (width/2);
	auto y = (width/2);
	auto z = width+50;

	std::vector<float> color = {(252/255.), (249/255.), (112/255.)};
	myEngine.setFlatColor(color[0], color[1], color[2]);
	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addTranslation(Vector3D(x, y, z));
		myEngine.mvMatrixStack.addHomothety({30, 30, 30});
		myEngine.switchToPhongShading();
		myEngine.setLightPosition(Vector4D(0, 0, 1, 0.0), 0);

		auto intensity = 2;
		myEngine.setLightIntensity(Vector3D(intensity*color[0], intensity*color[1], intensity*color[2]), 0);
		myEngine.switchToFlatShading();

		myEngine.updateMvMatrix();
		sphereMesh->draw();
	myEngine.mvMatrixStack.popMatrix();
	myEngine.setFlatColor(0., 0., 0.);
}

void renderPointLight(Vector3D vector){
	myEngine.setFlatColor(1, 1, 1);
	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addTranslation({vector.x, vector.y+1.2f, vector.z+2.4f});
		myEngine.mvMatrixStack.addHomothety({0.2, 0.2, 0.2});
		myEngine.switchToPhongShading();
		myEngine.setLightPosition(Vector4D(Vector3D{vector.x, vector.y+1.2f, vector.z+2.4f}, 1.0), 1);
		auto intensity = 10;
		myEngine.setLightIntensity(Vector3D(intensity*1, intensity*1, intensity*1), 1);
		myEngine.switchToFlatShading();

		myEngine.updateMvMatrix();
		sphereMesh->draw();
	myEngine.mvMatrixStack.popMatrix();
	myEngine.setFlatColor(0., 0., 0.);
}

void renderTree(){
	myEngine.setFlatColor(101/255.,67/255.,33/255.);

	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addRotation(M_PI_2, {1, 0., 0.});
		myEngine.updateMvMatrix();

		myEngine.mvMatrixStack.pushMatrix();
			myEngine.mvMatrixStack.addHomothety({0.1, 0.8, 0.1});
			myEngine.updateMvMatrix();
			cylinderMesh->draw();
		myEngine.mvMatrixStack.popMatrix();

		myEngine.setFlatColor(55/255.,100/255.,2/255.);
		myEngine.mvMatrixStack.pushMatrix();
			myEngine.mvMatrixStack.addHomothety({0.5, 1.2, 0.5});
			myEngine.mvMatrixStack.addTranslation({0., 0.4, 0.});
			myEngine.updateMvMatrix();
			coneMesh->draw();
		myEngine.mvMatrixStack.popMatrix();
	myEngine.mvMatrixStack.popMatrix();
	myEngine.setFlatColor(1., 1., 1.);
}

void renderPtero() {
    float time = glfwGetTime();
    float wingAngle = sin(time * 5.0f) * 0.6f; 

    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addHomothety({2.5, 2.5, 2.5}); 
        
        ////// CORPS
        myEngine.mvMatrixStack.pushMatrix();
            myEngine.mvMatrixStack.addHomothety({0.7, 1.8, 0.5}); 
            myEngine.setFlatColor(0.25, 0.45, 0.15); 
            myEngine.updateMvMatrix();
            sphereMesh->draw();
        myEngine.mvMatrixStack.popMatrix();

        ////// CRETE (DOS)
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

        ////// COU
        myEngine.mvMatrixStack.pushMatrix();
            myEngine.mvMatrixStack.addTranslation({0, 1.2f, 0.15f}); 
            myEngine.mvMatrixStack.addRotation(0.6f, {1, 0, 0}); 
            myEngine.mvMatrixStack.addTranslation({0, 0.35f, -0.2}); 
            myEngine.mvMatrixStack.addHomothety({0.3, 0.9, 0.3}); 
            myEngine.setFlatColor(0.25, 0.45, 0.15);
            myEngine.updateMvMatrix();
            sphereMesh->draw();
        myEngine.mvMatrixStack.popMatrix();

        ////// TETE
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


        ////// PATTES ARRIERES
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

        ////// AILES
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

                // MEMBRANE INTERNE
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
                    myEngine.setFlatColor(0.10, 0.30, 0); // Vert sombre
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

                // FIN AILE (Membrane externe)
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


    myEngine.mvMatrixStack.popMatrix();
}

void renderBasicScene() {
	
	a_frame->draw();
	renderSun();

	float speed = day_speed; 
	float L = 100.0f;
	float R = 50.0f;
	
	float periCercle = M_PI * R;
	float totalPeri = (2 * L) + (2 * periCercle);

	float dist = fmod(glfwGetTime() * speed, totalPeri);
	float x_rel, y_rel, angleRotation;

	if (dist < L) {
		x_rel = (L / 2.0f) - dist;
		y_rel = R;
		angleRotation = M_PI;
	} else if (dist < L + periCercle) {
		float d_arc = dist - L;
		float theta = (M_PI / 2.0f) + (d_arc / R); 
		x_rel = -L / 2.0f + cos(theta) * R;
		y_rel = sin(theta) * R;
		angleRotation = theta + (M_PI / 2.0f);
	} else if (dist < 2 * L + periCercle) {
		float d_droite = dist - (L + periCercle);
		x_rel = -L / 2.0f + d_droite;
		y_rel = -R;
		angleRotation = 0.0f;
	} else {
		float d_arc = dist - (2 * L + periCercle);
		float theta = (3.0f * M_PI / 2.0f) + (d_arc / R);
		x_rel = L / 2.0f + cos(theta) * R;
		y_rel = sin(theta) * R;
		angleRotation = theta + (M_PI / 2.0f);
	}

	float x = x_rel + (width / 2.0f);
	float y = y_rel + (width / 2.0f);
	float z = 25.0f;

	renderPointLight(Vector3D{x, y, z});

    myEngine.switchToPhongShading();
	myEngine.mvMatrixStack.pushMatrix();
		myEngine.setShininess(2.0);
		myEngine.mvMatrixStack.addTranslation(Vector3D{x, y, z});
		myEngine.mvMatrixStack.addRotation(angleRotation - M_PI_2, Vector3D{0, 0, 1});
		myEngine.setSpecularColor(STP3D::Vector3D(0.1f, 0.1f, 0.1f)); 
		myEngine.updateMvMatrix();
		renderPtero();
	myEngine.mvMatrixStack.popMatrix();

	// // volcan :
    // myEngine.switchToPhongShading();
    // myEngine.setFlatColor(1.0, 1.0, 1.0);
    // myEngine.mvMatrixStack.pushMatrix();
    //     myEngine.mvMatrixStack.addTranslation({-15.0f, -15.0f, 0.0f}); 
        
    //     myEngine.activateTexturing(true);
    //     myEngine.updateMvMatrix();
    //     rockTexture.attachTexture();
    //     drawVolcano();
    //     rockTexture.detachTexture();

    //     myEngine.mvMatrixStack.pushMatrix();
    //         myEngine.mvMatrixStack.addTranslation({0.0f, 0.0f, 7.0f}); 
    //         myEngine.mvMatrixStack.addHomothety({1.0f, 1.0f, 0.05f}); 
    //         myEngine.updateMvMatrix();
    //         lavaTexture.attachTexture();
    //         sphereMesh->draw();
    //         lavaTexture.detachTexture();
    //     myEngine.mvMatrixStack.popMatrix();

    //     myEngine.activateTexturing(false);
    // myEngine.mvMatrixStack.popMatrix();
    // myEngine.switchToFlatShading();


	myEngine.switchToPhongShading();
	myEngine.mvMatrixStack.pushMatrix();
	myEngine.mvMatrixStack.addHomothety(10.);
	myEngine.mvMatrixStack.addTranslation(Vector3D{0., 0., -Sh*100});
	myEngine.setShininess(5.0);
	myEngine.setSpecularColor(STP3D::Vector3D(0/255, 0/255, 112/255));
	myEngine.updateMvMatrix();
		for (auto tree : pixelTrees){
			myEngine.mvMatrixStack.pushMatrix();
				myEngine.mvMatrixStack.addTranslation(tree);
				myEngine.mvMatrixStack.addTranslation(Vector3D{0., 0, -0.1});
				myEngine.updateMvMatrix();
				renderTree();
			myEngine.mvMatrixStack.popMatrix();
		}

			myEngine.mvMatrixStack.pushMatrix();
				myEngine.updateMvMatrix();
				myEngine.activateTexturing(true);
				myTexture.attachTexture();
				drawTerrain();
				myTexture.detachTexture();
				myEngine.activateTexturing(false);
			myEngine.mvMatrixStack.popMatrix();
	myEngine.mvMatrixStack.popMatrix();
	myEngine.switchToFlatShading();
}

int main(int /*argc*/, char** /*argv*/)
{
	/* GLFW initialisation */
	GLFWwindow* window;
	if (!glfwInit()) return -1;

	
	/* Callback to a function if an error is rised by GLFW */
	glfwSetErrorCallback(onError);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
	if (!window)
	{
		// If no context created : exit !
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	std::cout<<"Loading GL extension"<<std::endl;
	// Intialize glad (loads the OpenGL functions)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		return -1;
	}

	glfwSetWindowSizeCallback(window,onWindowResized);
	glfwSetKeyCallback(window, onKey);
	glfwSetMouseButtonCallback(window, onMouseButton);

	std::cout<<"Engine init"<<std::endl;
	myEngine.mode2D = false; // Set engine to 3D mode
	myEngine.initGL();
	onWindowResized(window,WINDOW_WIDTH,WINDOW_HEIGHT);
	CHECK_GL;

	initBasicScene();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Get time (in second) at loop beginning */
		double startTime = glfwGetTime();
		movement(window);


		/* Render begins here */
		glClearColor(0.f,0.0f,0.2f,0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		
		/* Fix camera position */
		myEngine.mvMatrixStack.loadIdentity();

		Vector3D viewed_point = Vector3D(pos_camera[0] + cos(deg2rad(angle_horizontal)) * cos(deg2rad(angle_vertical)),
										 pos_camera[1] + sin(deg2rad(angle_horizontal)) * cos(deg2rad(angle_vertical)),
										 pos_camera[2] + sin(deg2rad(angle_vertical)));

		Vector3D up_vector = Vector3D(0.0,0.0,1.0); // DO NOT TOUCH IT
		Matrix4D viewMatrix = Matrix4D::lookAt(pos_camera,viewed_point,up_vector);
		myEngine.setViewMatrix(viewMatrix);
		myEngine.updateMvMatrix();

		renderBasicScene();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		/* Elapsed time computation from loop begining */
		double elapsedTime = glfwGetTime() - startTime;
		/* If to few time is spend vs our wanted FPS, we wait */
		if(elapsedTime < FRAMERATE_IN_SECONDS)
		{
			glfwWaitEventsTimeout(FRAMERATE_IN_SECONDS-elapsedTime);
		}
	}

	glfwTerminate();
	return 0;
}
