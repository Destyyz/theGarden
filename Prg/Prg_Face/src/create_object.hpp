#ifndef __CREATE_OBJECT_H
#define __CREATE_OBJECT_H

#include <vector>

extern unsigned int vertex_number;
extern std::vector<float> vertex_coord;
extern std::vector<float> normal_coord;
extern unsigned int triangle_number;
extern std::vector<unsigned int> triangle_index;

void createCoordinates();

void createCylinder();

#endif
