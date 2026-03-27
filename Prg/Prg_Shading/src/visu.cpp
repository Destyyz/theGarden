/* *******************************************************/
/* Exemple de programme OpenGL / GLFW           (C) 2022 */
/* Venceslas Biri   Université Paris Est Marne La Vallée */
/* *******************************************************/
#include <cstdlib>
#include <cmath>
#include <iostream>

#include "visu.hpp"
#include "globject.hpp"

/* Window properties */
static const unsigned int WINDOW_WIDTH = 1000;
static const unsigned int WINDOW_HEIGHT = 1000;
static const char WINDOW_TITLE[] = "Programme for Simple Shading";

/* global variables for camera specifications */
float profondeur;
float latitude;
float longitude;

/* global variables for scene handling */
float global_time {0.0f};
float global_light {0.0f};
bool use_shader {true};
bool animate_world { false};
bool animate_light {false};
GLuint my_shader {0};
Scene* my_scene = NULL;

/* Error handling function */
void onError(int error, const char* description)
{
	std::cerr<<"GLFW Error ("<<error<<"): "<<description<<std::endl;
}

void onWindowResized(GLFWwindow* /* window */, int width, int height)
{
	GLfloat  h = (GLfloat) width / (GLfloat) height ;
	
	/* dimension de l'écran GL */
	glViewport(0, 0, (GLint)width, (GLint)height);
	/* construction de la matrice de projection */
	glMatrixMode(GL_PROJECTION);
	float mat[16];
	computePerspectiveMatrix(mat,60.0,h,0.01,10.0);// Angle de vue, rapport largeur/hauteur, near, far
	glLoadMatrixf(mat);

	/* Retour a la pile de matrice Modelview
			et effacement de celle-ci */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/*********************************************************/
/* IHM : Key Event Handling                              */
void onKey(GLFWwindow* window, int key, int /* scancode */, int action, int /* mods */)
{
	int is_pressed = (action == GLFW_PRESS); 
	switch(key) {
		case GLFW_KEY_A :
		case GLFW_KEY_ESCAPE :
			if (is_pressed) glfwSetWindowShouldClose(window,GLFW_TRUE);
			break;
		case GLFW_KEY_F :
			if (is_pressed) glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			break;
		case GLFW_KEY_P :
			if (is_pressed) glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			break;
		case GLFW_KEY_Q :
			if (is_pressed) animate_world = !animate_world;
			break;
		case GLFW_KEY_L :
			if (is_pressed) animate_light = !animate_light;
			break;
		case GLFW_KEY_S :
			if (is_pressed) use_shader = !use_shader;
			break;
		case GLFW_KEY_Y :
			if (profondeur>0.1+STEP_PROF) profondeur -= STEP_PROF;
			break;
		case GLFW_KEY_H :
			profondeur += STEP_PROF;
			break;
		case GLFW_KEY_UP :
			if (latitude>STEP_ANGLE) latitude -= STEP_ANGLE;
			break;
		case GLFW_KEY_DOWN :
			if(latitude<MY_PI-STEP_ANGLE) latitude += STEP_ANGLE;
			break;
		case GLFW_KEY_LEFT :
			longitude -= STEP_ANGLE;
			break;
		case GLFW_KEY_RIGHT :
			longitude += STEP_ANGLE;
			break;
		default: 
			std::cout<<"Touche non gérée "<<std::endl;
	}
}


/*********************************************************/
/* fonction de dessin de la scène à l'écran              */
static void drawFunc(void) { 
	/* reinitialisation des buffers : couleur et ZBuffer */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* modification de la matrice de la scène */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/* Debut du dessin de la scène */
	glPushMatrix();
	
	/* placement de la caméra */
	float mat_v[16];
	float pos_cam[3] = {profondeur*sinf(longitude)*sinf(latitude),
						profondeur*cosf(latitude),
						profondeur*cosf(longitude)*sinf(latitude)};
	float pt_viewed[3] = {0.f,0.f,0.f};
	float up_vec[3] = {0.0,1.0,0.0};
	computeLookAtMatrix(mat_v,pos_cam,pt_viewed,up_vec);
	glMultMatrixf(mat_v);

	glUseProgram(0);

	glColor3f(1.0,0.0,0.0);
	GLObject::glDrawRepere(2.0);

	// LIGHT POSITION
	float theta_light = -MY_PI/4.0+global_light*MY_PI/50.0;
	float position[4] = {10.0f*cosf(theta_light)/sqrtf(2.f),5.0,10.0f*sinf(-theta_light)/sqrtf(2.f),1.0};
	//float position[4] = {10.0f,0.0,0.0f,1.0};
	glPushMatrix();
		glTranslatef(position[0],position[1],position[2]);
		GLObject::glDrawLight(0.2);
	glPopMatrix();

	if (use_shader) {
		glUseProgram(my_shader);
	}

	GLfloat view[16]; 
	glGetFloatv(GL_MODELVIEW_MATRIX, view); 
	glUniformMatrix4fv(glGetUniformLocation(my_shader,"view_matrix"),1,GL_FALSE,view);

	float overall_intensity = 75.0;
	float black[4] = {0.0,0.0,0.0,1.0};
	float spec_intensity[4] = {overall_intensity,0.0,0.0,0.0};
	float intensite[4] = {overall_intensity,overall_intensity,overall_intensity,0.0};
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
	glLightfv(GL_LIGHT0,GL_POSITION,position);
	glLightfv(GL_LIGHT0,GL_AMBIENT,black);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,intensite);
	glLightfv(GL_LIGHT0,GL_SPECULAR,spec_intensity);
	glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,1.0);
	glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,0.0);
	glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,MY_PI);

	glUniform3fv(glGetUniformLocation(my_shader,"light_world_position"),1,position);

	/* Debut du dessin */
	my_scene->drawScene(global_time);
	/* Fin du dessin */
	glPopMatrix();
	glDisable(GL_LIGHTING);

	/* fin de la définition de la scène */
	glFinish();
}

/*********************************************************/
/* fonction d'initialisation des paramètres de rendu et  */
/* des objets de la scènes.                              */
void init() {
	profondeur = 3.0;
	latitude = MY_PI/3.0;
	longitude = -MY_PI/4.0;

	global_time = 0.0;
	global_light = 0.0;

	/* INITIALISATION DES PARAMETRES GL */
	/* couleur du fond (gris sombre) */
	glClearColor(0.05,0.05,0.05,0.0);
	/* activation du ZBuffer */
	glEnable( GL_DEPTH_TEST);
	glEnable( GL_NORMALIZE);

	/* lissage des couleurs sur les facettes */
	glShadeModel(GL_SMOOTH);

	/* INITIALISATION DE LA SCENE */
	my_scene = new Scene();
	my_scene->createScene();
}

int main(int argc, char** argv) {

	if (argc != 1) {
		std::cerr << "Wrong number of argument. Usage is simply : "<<argv[0]<<""<<std::endl;
		exit(1);
	}

	/* GLFW initialisation */
	GLFWwindow* window;
	if (!glfwInit()) return -1;

	/* Callback to a function if an error is rised by GLFW */
	glfwSetErrorCallback(onError);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (!window)
	{
		// If no context created : exit !
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	gladLoadGL();
	
	glfwSetWindowSizeCallback(window,onWindowResized);
	glfwSetKeyCallback(window, onKey);
	
	my_shader = ShaderManager::loadShader("../shaders/vx_shader.glsl","../shaders/fg_shader.glsl",true);
	if (!my_shader) {
		std::cerr<<"SHADER could not compile, see message before"<<std::endl;
		exit(1);
	} 
	glUseProgram(my_shader);

	init();
	onWindowResized(window,WINDOW_WIDTH,WINDOW_HEIGHT);

	while (!glfwWindowShouldClose(window))
    {
		if (animate_world) global_time += STEP_TIME;
		if (animate_light) global_light += STEP_TIME;
        // handle events

        // draw...
		drawFunc();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
    }

	return 0;
}
