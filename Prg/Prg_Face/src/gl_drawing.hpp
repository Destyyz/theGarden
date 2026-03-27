#ifndef __GL_DRAWING_H
#define __GL_DRAWING_H

#include "create_object.hpp"
#include "tools/gl_tools.hpp"
#include "tools/matrix4d.hpp"
#include "tools/matrix_stack.hpp"
#include "tools/shaders.hpp"
#include "tools/basic_mesh.hpp"

using namespace STP3D;

// Application and GL parameters
extern GLuint idShader[2];
extern bool flagAnim;
extern GLuint vao_princ_object;

// World definition and transformation
extern Matrix4D projMatrix;
extern Matrix4D viewMatrix;
extern MatrixStack modelviewStack;
extern StandardMesh* repere;
extern IndexedMesh* the_object;

// Conversion degré en radians
inline double deg2rad(double deg) {
	return deg*M_PI/180.0;
}

// Envoie des transformations geometriques a OpenGL
void updateMvMatrix(int ids = 0);

void resetProjMatrix();

// Fixe la couleur de base du prochain objet dessine
void setFlatColor(float r,float g,float b);

void initGL();

void drawFunc();

#endif
