#include "create_object.hpp"

#include <iostream>
#include <cmath>

unsigned int vertex_number;
std::vector<float> vertex_coord;
std::vector<float> normal_coord;
unsigned int triangle_number;
std::vector<unsigned int> triangle_index;

void createCoordinates() {
	// Vous devez fixer :
	// - le nombre de sommets (vertex_number)
	// - le tableau des coordonnees des sommets (vertex_coord)
	// - le tableau des coordonnees des normales (normal_coord)
	// - le nombre de triangles (triangle_number)
	// - le tableau des indices des sommets constituant les triangles (triangle_index)
	

	createCylinder();

	// Vous pouvez maintenant définir directement la taille du vecteur avec les instructions ci-dessous
	// ou ne rien faire ici et utiliser l'operateurs push_back à vous de voir ce qui est le mieux.
	// Exemple ajustement de taille : vertex_coord.reasize(3*vertex_number); 
	// Exemple de remplissage direct : vertex_coord = {1.0,2.0,0.0};

	return;
}


void createCylinder(){
	
	triangle_number = 720;
	vertex_number = triangle_number*2;
	for (auto i = 0; i < 360; i++){
		auto angle = i * M_PI / 180.f;

		vertex_coord.push_back(cos(angle));
		vertex_coord.push_back(sin(angle));
		vertex_coord.push_back(-0.5);

		vertex_coord.push_back(cos(angle));
		vertex_coord.push_back(sin(angle));
		vertex_coord.push_back(0.5);

		
	}

	for (auto i = 0; i < 360; i++){

		triangle_index.push_back(i * 2);
		triangle_index.push_back((i * 2) + 1);
		triangle_index.push_back((i * 2) + 2 % vertex_number);

		triangle_index.push_back(i * 2 + 2);
		triangle_index.push_back((i * 2) + 1);
		triangle_index.push_back((i * 2) + 3);
	}
}
