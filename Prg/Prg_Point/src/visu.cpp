/* *******************************************************/
/* Exemple de programme OpenGL / GLUT           (C) 2010 */
/* Venceslas Biri   Université Paris Est Marne La Vallée */
/* *******************************************************/

#include <iostream>
#include <cmath>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "visu.hpp"
#include "gl_drawing.hpp"
#include "create_object.hpp"

/* Window properties */
static const unsigned int WINDOW_WIDTH = 1000;
static const unsigned int WINDOW_HEIGHT = 1000;
static const char WINDOW_TITLE[] = "Construction d'Objets Canoniques par Triangle";

/* variables globales pour la gestion de la caméra */
float profondeur {10};
float latitude {30.0};
float longitude {45.0};
float ratio {1.0};

float obj_rot {0.0};
bool flag_anim {false};


/* Error handling function */
void onError(int error, const char* description)
{
	std::cerr<<"GLFW Error ("<<error<<"): "<<description<<std::endl;
}

void onWindowResized(GLFWwindow* /* window */, int width, int height)
{
	// adjust the viewport when the window is resized
	std::cout<<"New window size : "<<width<<" "<<height<<std::endl;
	glViewport(0, 0, width, height);
	ratio = (float)width/(float)height;
	resetProjMatrix();
}

void onKey(GLFWwindow* window , int key, int /* scancode */, int action, int /* mods */)
{
	bool is_pressed = (action == GLFW_PRESS); 
	switch(key) {
		case GLFW_KEY_A :
		case GLFW_KEY_ESCAPE :
			if (is_pressed) glfwSetWindowShouldClose(window,GLFW_TRUE);
			break;
		case GLFW_KEY_S :
			if(is_pressed) flag_anim = !flag_anim;
			break;
		case GLFW_KEY_O :
			if (profondeur>0.1+STEP_PROF) profondeur -= STEP_PROF;
			break;
		case GLFW_KEY_L :
			profondeur += STEP_PROF;
			break;
		case GLFW_KEY_UP :
			if(latitude<90.0-STEP_ANGLE) latitude += STEP_ANGLE;
			break;
		case GLFW_KEY_DOWN :
			if (latitude>-90.0+STEP_ANGLE) latitude -= STEP_ANGLE;
			break;
		case GLFW_KEY_LEFT :
			longitude -= STEP_ANGLE;
			break;
		case GLFW_KEY_RIGHT :
			longitude += STEP_ANGLE;
			break;
		default: std::cout<<"Touche non gérée ("<<key<<")"<<std::endl;
	}
}


/*********************************************************/
/* fonction d'initialisation des paramètres de rendu et  */
/* des objets de la scènes.                              */
void init() {
	obj_rot = 0.0;
	flag_anim = false;

	/* INITIALISATION DE LA SCENE */
	createCoordinates();

	/* Preparation OpenGL */
	initGL();
}



int main() {
	/* GLFW initialisation */
	GLFWwindow* window;
	if (!glfwInit()) return -1;

	/* Utilisation OpenGL 4.1 pour Mac */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	/* Callback to a function if an error is rised by GLFW */
	glfwSetErrorCallback(onError);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1000.0, 1000.0, "Visualisation de solide par points", NULL, NULL);
	if (!window)
	{
		// If no context created : exit !
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	gladLoadGL();

	//glfwSetWindowSizeCallback(window,onWindowResized);
	glfwSetKeyCallback(window, onKey);

	onWindowResized(window,WINDOW_WIDTH,WINDOW_HEIGHT);

	init();

    while (!glfwWindowShouldClose(window))
    {
		// If animation is running update scene
		if (flag_anim) {
			obj_rot+=STEP_ANGLE;
		}


		// draw...
		drawFunc();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
    }

}
