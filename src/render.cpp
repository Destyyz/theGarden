#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "glbasimac/glbi_engine.hpp"
#include "glbasimac/glbi_texture.hpp"
#include "init_terrain.hpp"
#include "init_volcano.hpp"
#include "init_ptero.hpp"
#include "init_tree.hpp"
#include "open_file.hpp"
#include "sun_light.hpp"
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
std::vector<int> tabCones;

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
		pos_camera[2] -= 1.0 * speed;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		pos_camera[2] += 1.0 * speed;
	
}

void initFrame(){
	a_frame = createRepere(10.0);
	a_frame->createVAO();
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
	float z = 85.0f;
	float angle = angleRotation - M_PI/2;

	renderPointLight(Vector3D{x, y, z}, angle);

    shadePtero(Vector3D{x, y, z}, angle);

	// volcan :
    myEngine.switchToPhongShading();
    myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addTranslation({width * 0.78f, width * 0.35f, -3.0f});
        myEngine.mvMatrixStack.addHomothety({9.5f, 9.5f, 9.5f});

        myEngine.activateTexturing(true);
        myEngine.updateMvMatrix();
        rockTexture.attachTexture();
        drawVolcano();
        rockTexture.detachTexture();

        myEngine.mvMatrixStack.pushMatrix();
            myEngine.mvMatrixStack.addTranslation({0.0f, 0.0f, 7.0f});
            myEngine.mvMatrixStack.addHomothety({1.3f, 1.3f, 0.05f});
            myEngine.updateMvMatrix();
            lavaTexture.attachTexture();
            sphereMesh->draw();
            lavaTexture.detachTexture();
        myEngine.mvMatrixStack.popMatrix();

        myEngine.activateTexturing(false);
    myEngine.mvMatrixStack.popMatrix();
    myEngine.switchToFlatShading();


	myEngine.switchToPhongShading();
	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addHomothety(10.);
		myEngine.mvMatrixStack.addTranslation(Vector3D{0., 0., -Sh*100});
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

