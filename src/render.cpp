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

/* 3D Engine global variables */
StandardMesh* a_frame;
GLBI_Texture myTexture;

// Pour le ?ptérodactyle? RAWR
IndexedMesh* sphereMesh = nullptr;
IndexedMesh* cubeMesh = nullptr;
IndexedMesh* cylinderMesh = nullptr;
StandardMesh* coneMesh = nullptr;

bool flag_rotation = false;

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
		case GLFW_KEY_A :
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
		case GLFW_KEY_T : 
			if (is_pressed) flag_rotation = !flag_rotation;
			break;
		default: std::cerr<<"Touche non gérée "<<key<<std::endl;
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

void initBasicScene() {
	auto pixmap = open_file("../assets/terrain_copy.pgm");
	initScene(pixmap);

	sphereMesh = STP3D::basicSphere(1.0f, 20, 20);
    sphereMesh->createVAO();

    cubeMesh = STP3D::basicCube();
    cubeMesh->createVAO();

    cylinderMesh = STP3D::basicCylinder(1.0f, 1.0f);
    cylinderMesh->createVAO();

    coneMesh = STP3D::basicCone(1.0f, 1.0f);
    coneMesh->createVAO();

	// cone = basicCone(1, 1);
	// cone->createVAO();
	// cylinder = STP3D::basicCylinder(1, 1);
	// cylinder->createVAO();
	
	a_frame = createRepere(10.0);
	a_frame->createVAO();

	myEngine.switchToFlatShading();

	glActiveTexture(GL_TEXTURE0);
	//Load de l'image
	int img_width, img_height, img_channels;
	auto image = stbi_load("../assets/herbe.png", &img_width, &img_height, &img_channels, 0);
	if (image != nullptr){
		std::cout << "Image chargée correctement" << std::endl;
	}

	myTexture.createTexture();
	myTexture.attachTexture();
	myTexture.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	myTexture.loadImage(img_width, img_height, img_channels, image);
	myTexture.detachTexture();
	stbi_image_free(image);
}

void renderTree() {
    myEngine.setFlatColor(1.0, 1.0, 1.0);
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addRotation(M_PI_2, {1, 0., 0.});
        myEngine.mvMatrixStack.addHomothety({0.1, 0.3, 0.1});
        myEngine.updateMvMatrix();
        cylinderMesh->draw();
    myEngine.mvMatrixStack.popMatrix();
    
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addRotation(M_PI_2, {1, 0., 0.});
        myEngine.mvMatrixStack.addHomothety({0.5, 1., 0.5});
        myEngine.mvMatrixStack.addTranslation({0., 0.3, 0.});
        myEngine.updateMvMatrix();
        coneMesh->draw();
    myEngine.mvMatrixStack.popMatrix();
}


void renderPtero() {
    float time = glfwGetTime();
    float wingAngle = sin(time * 5.0f) * 0.6f; 

    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation({0, 0, 5.0f}); 
        
        ////// CORPS
        myEngine.mvMatrixStack.pushMatrix();
            myEngine.mvMatrixStack.addHomothety({0.7, 1.8, 0.5}); 
            myEngine.setFlatColor(0.25, 0.45, 0.15); 
            myEngine.updateMvMatrix();
            sphereMesh->draw();
        myEngine.mvMatrixStack.popMatrix();

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
                    myEngine.mvMatrixStack.addHomothety(0.1f); 
                    myEngine.setFlatColor(1.0, 0.0, 0.0);
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
                
                myEngine.mvMatrixStack.pushMatrix();
                    myEngine.mvMatrixStack.addTranslation({0.8f * side, 0, 0}); 
                    myEngine.mvMatrixStack.addHomothety({1.6, 0.9, 0.1}); 
                    myEngine.setFlatColor(0.25, 0.45, 0.15); 
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
                        myEngine.mvMatrixStack.addHomothety({0.05, 0.15, 0.5}); 
                        myEngine.mvMatrixStack.addTranslation({0, 0, -0.3f});
                        myEngine.setFlatColor(0.10, 0.30, 0);
                        myEngine.updateMvMatrix();
                        coneMesh->draw();
                    myEngine.mvMatrixStack.popMatrix();
                }

                myEngine.mvMatrixStack.pushMatrix();
                    myEngine.mvMatrixStack.addRotation(-0.2f * side, {0, 0, 1}); 
                    myEngine.mvMatrixStack.addRotation(M_PI_2 * side, {0, 1, 0}); 
                    myEngine.mvMatrixStack.addTranslation({0, -0.5, 0});
                    myEngine.mvMatrixStack.addHomothety({0.1, 0.8, 1.6}); 
                    myEngine.setFlatColor(0.25, 0.45, 0.15);
                    myEngine.updateMvMatrix();
                    coneMesh->draw();
                myEngine.mvMatrixStack.popMatrix();

                myEngine.mvMatrixStack.pushMatrix();
                    myEngine.mvMatrixStack.addRotation(-0.5f * side, {0, 0, 1}); 
                    myEngine.mvMatrixStack.addTranslation({1.0f * side, 0, 0}); 
                    myEngine.mvMatrixStack.addRotation(M_PI_2 * side, {0, 1, 0}); 
                    myEngine.mvMatrixStack.addHomothety({0.1, 1.0, 2.0}); 
                    myEngine.setFlatColor(0.25, 0.45, 0.15);
                    myEngine.updateMvMatrix();
                    coneMesh->draw();
                myEngine.mvMatrixStack.popMatrix();

            myEngine.mvMatrixStack.popMatrix();
        }
    myEngine.mvMatrixStack.popMatrix();
}


void renderBasicScene() {
	a_frame->draw();

	myEngine.switchToFlatShading();
    renderPtero();

	//renderTree();
	
	/*
	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addHomothety(5.);
		myEngine.mvMatrixStack.addTranslation({(width/2)*-Sp, -6.4, 0});
		myEngine.updateMvMatrix();
		myEngine.activateTexturing(true);
		myTexture.attachTexture();
		drawTerrain();
		myTexture.detachTexture();
		myEngine.activateTexturing(false);
	myEngine.mvMatrixStack.popMatrix();
	*/
	
	
	/*
	//boule qui bouge pas
	myEngine.switchToPhongShading();
	myEngine.mvMatrixStack.pushMatrix();
		myEngine.activateTexturing(true);
		myTexture.attachTexture();
		myEngine.setFlatColor(0, 1, 1);
		myEngine.updateMvMatrix();
		boule->draw();
		myTexture.detachTexture();
		myEngine.activateTexturing(false);
	myEngine.mvMatrixStack.popMatrix();

	myEngine.switchToFlatShading();

	//boule qui bouge
	myEngine.setFlatColor(1.0,1.0,0.0);
	myEngine.mvMatrixStack.pushMatrix();
	if (flag_rotation){
		auto angle = glfwGetTime();
		myEngine.mvMatrixStack.addTranslation(Vector3D(5*cos(angle), 5*sin(angle), 15.));
		myEngine.switchToPhongShading();
		myEngine.setLightPosition(Vector4D(5*cos(angle), 5*sin(angle), 15., 1.0));
		myEngine.switchToFlatShading();

	} else {
		myEngine.mvMatrixStack.addTranslation(Vector3D(5, 0,15));
	}
	myEngine.mvMatrixStack.addHomothety(0.05);
	myEngine.updateMvMatrix();
	boule->draw();
	myEngine.mvMatrixStack.popMatrix();
	*/
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

		/* Render begins here */
		glClearColor(0.f,0.0f,0.2f,0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		
		/* Fix camera position */
		myEngine.mvMatrixStack.loadIdentity();

		Vector3D pos_camera = Vector3D(dist_zoom*cos(deg2rad(angle_theta))*cos(deg2rad(angle_phy)),
										dist_zoom*sin(deg2rad(angle_theta))*cos(deg2rad(angle_phy)),
										dist_zoom*sin(deg2rad(angle_phy)));
		Vector3D viewed_point = Vector3D(0.0,0.0,0.0);
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
