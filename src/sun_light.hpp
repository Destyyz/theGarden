#pragma once
#include "glbasimac/glbi_engine.hpp"
#include "tools/mesh.hpp"

using namespace glbasimac;
using namespace STP3D;

void initSun();

void renderSun();

void renderPointLight(Vector3D vector, float angle);