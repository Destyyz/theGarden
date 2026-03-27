#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cmath>

#include "globject.hpp"
#include "visu.hpp"

void fillMatrixFrustum(float *matrix, float left, float right, float bottom, float top,
						float znear, float zfar)
{
	float temp=0;
	float temp2, temp3, temp4;
	temp = 2.0 * znear;
	temp2 = right - left;
	temp3 = top - bottom;
	temp4 = zfar - znear;
	matrix[0] = temp / temp2;
	matrix[1] = 0.0;
	matrix[2] = 0.0;
	matrix[3] = 0.0;
	matrix[4] = 0.0;
	matrix[5] = temp / temp3;
	matrix[6] = 0.0;
	matrix[7] = 0.0;
	matrix[8] = (right + left) / temp2;
	matrix[9] = (top + bottom) / temp3;
	matrix[10] = (-zfar - znear) / temp4;
	matrix[11] = -1.0;
	matrix[12] = 0.0;
	matrix[13] = 0.0;
	matrix[14] = (-temp * zfar) / temp4;
	matrix[15] = 0.0;
}

float normalize(float* a_vec) {
	float nrm = sqrt(a_vec[0]*a_vec[0]+a_vec[1]*a_vec[1]+a_vec[2]*a_vec[2]);
	if (nrm<0.0001) return 0.0;
	a_vec[0]/=nrm;a_vec[1]/=nrm;a_vec[2]/=nrm;
	return nrm;
}

void vectProduct(float* a,float* b,float* result) {
	result[0] = a[1]*b[2]-a[2]*b[1];
	result[1] = a[2]*b[0]-a[0]*b[2];
	result[2] = a[0]*b[1]-a[1]*b[0];
}
float dotProduct(float*a,float*b) {return (a[0]*b[0]+a[1]*b[1]+a[2]*b[2]);}

void computeLookAtMatrix( float *matrix, float *eyePosition3D,
							float *center3D, float *upVector3D )
{
	float forward[3], side[3], up[3];
	float matrix2[16];
	// --------------------
	forward[0] = center3D[0] - eyePosition3D[0];
	forward[1] = center3D[1] - eyePosition3D[1];
	forward[2] = center3D[2] - eyePosition3D[2];
	normalize(forward);
	// --------------------
	// Side = forward x up
	vectProduct(forward, upVector3D, side);
	normalize(side);
	// --------------------
	// Recompute up as: up = side x forward
	vectProduct(side, forward, up);
	// --------------------
	matrix2[0] = side[0];
	matrix2[4] = side[1];
	matrix2[8] = side[2];
	//matrix2[12] = 0.0;
	matrix2[12] = -dotProduct(side,eyePosition3D);
	// --------------------
	matrix2[1] = up[0];
	matrix2[5] = up[1];
	matrix2[9] = up[2];
	//matrix2[13] = 0.0;
	matrix2[13] = -dotProduct(up,eyePosition3D);
	// --------------------
	matrix2[2] = -forward[0];
	matrix2[6] = -forward[1];
	matrix2[10] = -forward[2];
	//matrix2[14] = 0.0;
	matrix2[14] = dotProduct(forward,eyePosition3D);
	// --------------------
	matrix2[3] = matrix2[7] = matrix2[11] = 0.0;
	matrix2[15] = 1.0;
	// --------------------
	memcpy(matrix, matrix2, 16*sizeof(float));
}

void computePerspectiveMatrix(float *matrix, float fovyInDegrees, float aspectRatio,
            		          float znear, float zfar)
{
	float ymax, xmax;
	ymax = znear * tanf(fovyInDegrees * MY_PI / 360.0);
	xmax = ymax * aspectRatio;
	// ymin = -ymax;
	// xmin = -ymax * aspectRatio; = -xmax;
	fillMatrixFrustum(matrix, -xmax, xmax, -ymax, ymax, znear, zfar);
}

void GLObject::createCoordinatesCube() {
	// CUBE
	vertex_number = 24;
	triangle_number = 12;
	
	vertex_coord = (float*) calloc(sizeof(float),3*vertex_number);
	normal_coord = (float*) calloc(sizeof(float),3*vertex_number);
	triangle_index = (unsigned int*) calloc(sizeof(unsigned int),3*triangle_number);
	
	// CONSTRUIRE LES TABLEAUX
	float width = 1.0;
	float coord[3*24] = {
		// Face front (z = -width/2) (E,F,G,H)
		-width/2.0f,-width/2.0f,-width/2.0f,	// E
		width/2.0f,-width/2.0f,-width/2.0f,		// F
		width/2.0f,width/2.0f,-width/2.0f,		// G
		-width/2.0f,width/2.0f,-width/2.0f,		// H
		// Face back (z = width/2) (A,B,C,D)
		-width/2.0f,-width/2.0f,width/2.0f,		// A
		width/2.0f,-width/2.0f,width/2.0f,		// B
		width/2.0f,width/2.0f,width/2.0f,		// C
		-width/2.0f,width/2.0f,width/2.0f,		// D
		// Face left (x = -width/2) (E,A,D,H)
		-width/2.0f,-width/2.0f,-width/2.0f,	// E
		-width/2.0f,-width/2.0f,width/2.0f,		// A
		-width/2.0f,width/2.0f,width/2.0f,		// D
		-width/2.0f,width/2.0f,-width/2.0f,		// H
		// Face right (x = +width/2) (B,F,G,C)
		width/2.0f,-width/2.0f,width/2.0f,		// B
		width/2.0f,-width/2.0f,-width/2.0f,		// F
		width/2.0f,width/2.0f,-width/2.0f,		// G
		width/2.0f,width/2.0f,width/2.0f,		// C
		// Face top (y = +width/2) (D,C,G,H)
		-width/2.0f,width/2.0f,width/2.0f,		// D
		width/2.0f,width/2.0f,width/2.0f,		// C
		width/2.0f,width/2.0f,-width/2.0f,		// G
		-width/2.0f,width/2.0f,-width/2.0f,		// H
		// Face down (y = -width/2) (A,B,F,E)
		-width/2.0f,-width/2.0f,width/2.0f,		// A
		width/2.0f,-width/2.0f,width/2.0f,		// B
		width/2.0f,-width/2.0f,-width/2.0f,		// F
		-width/2.0f,-width/2.0f,-width/2.0f		// E
	};
	float normals[3*24] = {
		// Face back (z = -width/2) (E,F,G,H)
		0.0f,0.0f,-1.0f,	0.0f,0.0f,-1.0f,	0.0f,0.0f,-1.0f,	0.0f,0.0f,-1.0f,
		// Face front (z = width/2) (A,B,C,D)
		0.0f,0.0f,1.0f,		0.0f,0.0f,1.0f,		0.0f,0.0f,1.0f,		0.0f,0.0f,1.0f,
		// Face left (x = -width/2) (E,A,D,H)
		-1.0,0.0,0.0,		-1.0,0.0,0.0,		-1.0,0.0,0.0,		-1.0,0.0,0.0,
		// Face right (x = +width/2) (B,F,G,C)
		1.0,0.0,0.0,		1.0,0.0,0.0,		1.0,0.0,0.0,		1.0,0.0,0.0,
		// Face top (y = +width/2) (D,C,G,H)
		0.0,1.0,0.0,		0.0,1.0,0.0,		0.0,1.0,0.0,		0.0,1.0,0.0,
		// Face down (y = -width/2) (A,B,F,E)
		0.0,-1.0,0.0,		0.0,-1.0,0.0,		0.0,-1.0,0.0,		0.0,-1.0,0.0
	};
	memcpy(vertex_coord,coord,3*vertex_number*sizeof(float));
	memcpy(normal_coord,normals,3*vertex_number*sizeof(float));
	unsigned int indexes[3*12] = {
		// Face front (z = -width/2) (E,F,G,H)
		0,1,2,		0,2,3,
		// Face back (z = width/2) (A,B,C,D)
		4,5,6,		4,6,7,
		// Face left (x = -width/2) (E,A,D,H)
		8,9,10,		8,10,11,
		// Face right (x = +width/2) (B,F,G,C)
		12,13,14,		12,14,15,
		// Face top (y = +width/2) (D,C,G,H)
		16,17,18,		16,18,19,
		// Face down (y = -width/2) (A,B,F,E)
		20,21,22,		20,22,23
	};
	memcpy(triangle_index,indexes,3*triangle_number*sizeof(float));
}

void GLObject::createCoordinatesSphere() {
	// SPHERE
	unsigned int nb_div_tranche = 40;
	unsigned int nb_div_circle = 60;
	// 2 points à chaque extremitee + des cercles concentriques
	vertex_number = 2 + nb_div_tranche*nb_div_circle;
	// La sphere a 2 calottes en haut et en bas + (nb_div_tranche - 1) bandes de triangles 
	triangle_number = 2*nb_div_circle + (nb_div_tranche - 1)*2*nb_div_circle;
	
	vertex_coord = (float*) calloc(sizeof(float),3*vertex_number);
	normal_coord = (float*) calloc(sizeof(float),3*vertex_number);
	triangle_index = (unsigned int*) calloc(sizeof(unsigned int),3*triangle_number);

	// On rentre les sommets sans les deux points haut / bas
	float theta,phy;
	unsigned int idxV = 0;
	unsigned int idxN = 0;
	for(unsigned int i = 0;i < nb_div_tranche;i++) {
		phy = (i+1)*MY_PI/(float)(nb_div_tranche+1);
		for(unsigned int j = 0; j < nb_div_circle; j++) {
			theta = j*2*MY_PI/nb_div_circle;
			vertex_coord[idxV++] = sin(phy)*cos(theta);
			vertex_coord[idxV++] = cos(phy);
			vertex_coord[idxV++] = sin(phy)*sin(theta);
			normal_coord[idxN++] = sin(phy)*cos(theta);
			normal_coord[idxN++] = cos(phy);
			normal_coord[idxN++] = sin(phy)*sin(theta);
		}
	}
	// Le sommet (haut) => indice : nb_div_tranche*nb_div_circle
	vertex_coord[idxV++] = normal_coord[idxN++] = 0.0;
	vertex_coord[idxV++] = normal_coord[idxN++] = 1.0;
	vertex_coord[idxV++] = normal_coord[idxN++] = 0.0;
	// Le sommet (bas) => indice : nb_div_tranche*nb_div_circle + 1
	vertex_coord[idxV++] = normal_coord[idxN++] = 0.0;
	vertex_coord[idxV++] = normal_coord[idxN++] = -1.0;
	vertex_coord[idxV++] = normal_coord[idxN++] = 0.0;

	// Calotte haute 
	unsigned int idxTr = 0;
	for(unsigned int i=0;i<nb_div_circle;i++) {
		triangle_index[idxTr++] = nb_div_tranche*nb_div_circle;
		triangle_index[idxTr++] = i;
		triangle_index[idxTr++] = (i==nb_div_circle-1) ? 0 : i+1;
	}
	// Calotte basse 
	unsigned int first_idx = (nb_div_tranche-1)*nb_div_circle;
	for(unsigned int i=0;i<nb_div_circle;i++) {
		triangle_index[idxTr++] = nb_div_tranche*nb_div_circle+1;
		triangle_index[idxTr++] = first_idx + i;
		triangle_index[idxTr++] = (i==0) ? first_idx + nb_div_circle -1 : first_idx + i - 1;
	}
	// Bandes de triangles
	unsigned int A,B,C,D; // A (i,j) / B(i+1,j) / C(i+1,j+1) / D(i,j+1)
	for(unsigned int i = 0;i < nb_div_tranche-1;i++) {
		for(unsigned int j = 0;j < nb_div_circle ; j++) {
			A = i*nb_div_circle+j;
			B = (i+1)*nb_div_circle+j;
			C = (j==nb_div_circle-1) ? (i+1)*nb_div_circle : (i+1)*nb_div_circle+j+1;
			D = (j==nb_div_circle-1) ? i*nb_div_circle : i*nb_div_circle + j + 1;
			triangle_index[idxTr++] = A;
			triangle_index[idxTr++] = B;
			triangle_index[idxTr++] = C;
			triangle_index[idxTr++] = A;
			triangle_index[idxTr++] = C;
			triangle_index[idxTr++] = D;
		}
	}

}

void GLObject::createCoordinatesQuad() {
	// QUAD
	// Des bandes de n*n points donc n-1 * n-1 carrés
	int nb_pt_per_side = 100;
	int nb_div = nb_pt_per_side - 1;
	vertex_number = nb_pt_per_side*nb_pt_per_side;
	// 2 triange par carré 
	triangle_number = 2*nb_div*nb_div;
	
	vertex_coord = (float*) calloc(sizeof(float),3*vertex_number);
	normal_coord = (float*) calloc(sizeof(float),3*vertex_number);
	triangle_index = (unsigned int*) calloc(sizeof(unsigned int),3*triangle_number);

	unsigned int idx = 0;
	unsigned int idxn = 0;
	float width = 1.0;
	for(int i=0;i<nb_pt_per_side;i++) {
		for(int j=0;j<nb_pt_per_side;j++) {
			vertex_coord[idx++]  = - width/2.0 + j*width/(float)nb_div; 
			vertex_coord[idx++]  = 0.0;
			vertex_coord[idx++]  = width/2.0 - i*width/(float)nb_div; 
			normal_coord[idxn++] = 0.0;
			normal_coord[idxn++] = 1.0;
			normal_coord[idxn++] = 0.0;
		}
	}

	unsigned int idxt = 0;
	for(int i=0;i<nb_div;i++) {
		for(int j=0;j<nb_div;j++) {
			triangle_index[idxt++] = i*nb_pt_per_side + j;
			triangle_index[idxt++] = i*nb_pt_per_side + j + 1;
			triangle_index[idxt++] = (i+1)*nb_pt_per_side + j + 1;
			triangle_index[idxt++] = i*nb_pt_per_side + j;
			triangle_index[idxt++] = (i+1)*nb_pt_per_side + j + 1;
			triangle_index[idxt++] = (i+1)*nb_pt_per_side + j;
		}
	}
}

void GLObject::createCoordinates() {

	if (triangle_index != NULL) free(triangle_index);
	if (vertex_coord != NULL) free(vertex_coord);
	if (normal_coord != NULL) free(normal_coord);

	if (type_objet == CUBE) {
		createCoordinatesCube();
	}
	else if (type_objet == SPHERE) {
		createCoordinatesSphere();
	}
	else if (type_objet == QUAD) {
		createCoordinatesQuad();
	}
	else  {
		std::cerr<<"Primitive non connue"<<std::endl;
		exit(1);
	}
}

void GLObject::glDrawObject() {
	float black[3] = {0.0,0.0,0.0};
	float white[3] = {1.0,1.0,1.0};
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,black);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,coef_ref_dif);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,128.0);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3,GL_FLOAT,0,vertex_coord);
	glNormalPointer(GL_FLOAT,0,normal_coord);
	glDrawElements(GL_TRIANGLES,3*triangle_number,GL_UNSIGNED_INT,triangle_index);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void GLObject::glDrawRepere(float length) {
	// dessin du repère
	glBegin(GL_LINES);
		glColor3f(1.,0.,0.);
		glVertex3f(0.,0.,0.);
		glVertex3f(length,0.,0.);
		glColor3f(0.,1.,0.);
		glVertex3i(0.,0.,0.);
		glVertex3i(0.,length,0.);
		glColor3f(0.,0.,1.);
		glVertex3i(0.,0.,0.);
		glVertex3i(0.,0.,length);
	glEnd();
}

void GLObject::glDrawLight(float length) {
	float hl = length/2.;
	glColor3f(1.,1.,0.);
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0.,hl,0.);
		glVertex3f(hl,0.,0.);
		glVertex3f(0.,0.,hl);
		glVertex3f(-hl,0.,0.);
		glVertex3f(0.,0.,-hl);
		glVertex3f(hl,0.,0.);
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0.,-hl,0.);
		glVertex3f(hl,0.,0.);
		glVertex3f(0.,0.,hl);
		glVertex3f(-hl,0.,0.);
		glVertex3f(0.,0.,-hl);
		glVertex3f(hl,0.,0.);
	glEnd();
}


