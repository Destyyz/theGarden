#include "create_object.hpp"

#include <iostream>
#include <cmath>

unsigned int vertex_number;
std::vector<float> vertex_coord;
std::vector<float> normal_coord;

void createCoordinates() {
	// Vous devez fixer :
	// - le nombre de sommets (vertex_number)q
	// - le tableau des coordonnees des sommets (vertex_coord)
	// - le tableau des coordonnees des normales (normal_coord)


	createBoule();

	// Vous pouvez maintenant définir directement la taille du vecteur avec les instructions ci-dessous
	// ou ne rien faire ici et utiliser l'operateurs push_back à vous de voir ce qui est le mieux.
	// Exemple ajustement de taille : vertex_coord.reasize(3*vertex_number); 
	// Exemple de remplissage direct : vertex_coord = {1.0,2.0,0.0};

	return;
}

void createBoule(){
	vertex_number = 180*360;
	for (auto alpha = 0; alpha < 180; alpha++){ 		//latitude
		for (auto beta = 0; beta < 360; beta++){ 		//longitude
			auto lat = alpha * M_PI / 180.f;
			auto longi = beta * M_PI / 180.f;

			vertex_coord.push_back(cos(lat)*sin(longi));
			vertex_coord.push_back(sin(lat)*sin(longi));
			vertex_coord.push_back(cos(longi));
		}
	}
}


