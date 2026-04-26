#pragma once

#include "glbasimac/glbi_engine.hpp"
#include "tools/mesh.hpp"

using namespace glbasimac;
using namespace STP3D;

void initPtero();

void renderPteroBody();

void renderPteroHead();

void renderPteroLegs();

void renderPteroWings(float wingAngle);

void renderPtero();

void shadePtero(Vector3D vector, float angle);