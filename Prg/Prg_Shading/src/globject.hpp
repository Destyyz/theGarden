#pragma once

#include <cstdlib>

enum OBJ_CANONIQUE { CUBE, SPHERE, QUAD, CONE };

// Utilitary functions (DO NOT LOOK AT IT)
void computePerspectiveMatrix(float* matrix,float fovyInDegrees, float aspectRatio,float znear, float zfar);
void computeLookAtMatrix(float *matrix, float *eyePosition3D, float *center3D, float *upVector3D);

class GLObject {

	public:
		unsigned int type_objet;            // 0/1 cube 2 sphere 3 quad 4 cone
		unsigned int vertex_number;
		float* vertex_coord;
		float* normal_coord;
		float coef_ref_dif[3];
		unsigned int triangle_number;
		unsigned int* triangle_index;

		GLObject(unsigned int typ,float *dif) {
			vertex_coord = NULL;
			normal_coord = NULL;
			triangle_index = NULL;
			type_objet = typ;
			for(unsigned int i=0;i<3;i++) {coef_ref_dif[i] = dif[i];}
			createCoordinates();
		};

		~GLObject() {
			if (vertex_coord != NULL) free(vertex_coord);
			if (normal_coord != NULL) free(normal_coord);
			if (triangle_index != NULL) free(triangle_index);
		};

		void glDrawObject();

		static void glDrawRepere(float length);
		static void glDrawLight(float length);

	private:
		void createCoordinates();
		void createCoordinatesQuad();
		void createCoordinatesCube();
		void createCoordinatesSphere();
};

