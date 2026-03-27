#include "glad/glad.h"

#include <iostream>
#include <cmath>
#include <cstring>

#include "gl_drawing.hpp"
#include "visu.hpp"
#include "create_object.hpp"

// Application and GL parameters
GLuint idShader[2];

// World parameters
Matrix4D projMatrix;
Matrix4D viewMatrix;
MatrixStack modelviewStack;
StandardMesh* repere = NULL;
IndexedMesh* the_object = NULL;

void setFlatColor(float r,float g,float b) {
	glVertexAttrib3f(glGetAttribLocation(idShader[0],"vx_col"),r,g,b);
}

void updateMvMatrix(int ids) {
	Matrix4D nmlMatrix = modelviewStack.getTopGLMatrix();
	nmlMatrix.invert();
	nmlMatrix.transpose();
	glUniformMatrix4fv(glGetUniformLocation(idShader[ids],"modelviewMat"),1,GL_FALSE,modelviewStack.getTopGLMatrix());
	if (ids==0) glUniformMatrix4fv(glGetUniformLocation(idShader[ids],"normalMat"),1,GL_FALSE,nmlMatrix);
}

void resetProjMatrix() {
	projMatrix = Matrix4D::perspective(60.0,ratio,0.1f,2000.0f);
}

void initGL() {
	/*****************************************
	 * GL BUSINESS - DO NOT TRY TO UNDERSTAND
	 *****************************************/
	/* activation du ZBuffer */
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	projMatrix = Matrix4D::perspective(60.0,ratio,1.0f,2000.0f);

	idShader[0] = ShaderManager::loadShader("../shaders/flat_shading.vert","../shaders/flat_shading.frag",true);
	printf("Shader initialized !\n");

	glUseProgram(idShader[0]);

	/*****************************************
	 * CREATION OBJETS DE BASE (CANONIQUES)
	 *****************************************/
	repere = createRepere(5.0f);
	repere->createVAO();

	the_object = new IndexedMesh(triangle_number,vertex_number);
	the_object->addOneBuffer(0,3,vertex_coord.data(),"Coord",true);
	the_object->addIndexBuffer(triangle_index.data(),true);
	the_object->createVAO();

	std::cout<<"Initialisation is done !"<<std::endl;
}

/*********************************************************/
/* fonction de dessin de la scène à l'écran              */
void drawFunc(void) { 
	/* reinitialisation des buffers : couleur et ZBuffer */
	glClearColor(0.2,0.2,0.2,0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*****************************************
	 * GL BUSINESS - DO NOT TRY TO UNDERSTAND
	 *****************************************/
	glUseProgram(idShader[0]);
	glUniformMatrix4fv(glGetUniformLocation(idShader[0],"projectionMat"),1,GL_FALSE,projMatrix);

	/***************************************
	 * CAMERA SETTING
	 ***************************************/
	//> EXO 3
	Vector3D viewed_point,pos_camera;
	pos_camera = Vector3D(profondeur*cos(deg2rad(longitude))*cos(deg2rad(latitude)),
						  profondeur*sin(deg2rad(latitude)),
						  profondeur*sin(deg2rad(longitude))*cos(deg2rad(latitude)));
	viewed_point = Vector3D(0.0,0.0,0.0);
	Vector3D up_vector = Vector3D(0.0,1.0,0.0); // DO NOT TOUCH IT
	viewMatrix = Matrix4D::lookAt(pos_camera,viewed_point,up_vector);
	CHECK_GL;
	//< FIN EXO 3

	/***************************************
	 * SCENE RENDERING
	 ***************************************/
	modelviewStack.loadIdentity();
	modelviewStack.pushMatrix();
	modelviewStack.addTransformation(viewMatrix);
	updateMvMatrix(0);
	repere->draw();

	glVertexAttrib3f(3,1.0,1.0,1.0);
	the_object->draw();
}

