#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "glad/glad.h"

#include "shaders.hpp"
#include "globject.hpp"
#include "scene.hpp"

#define MY_PI 3.141592653f


#define STEP_TIME	0.32f
#define STEP_ANGLE	MY_PI/90.0
#define STEP_PROF	MY_PI/90.0

/* global variables for camera specifications */
extern float profondeur;
extern float latitude;
extern float longitude;

/* global variables for scene handling */
extern float global_time;
extern float global_light;
extern bool use_shader;
extern bool animate_world;
extern bool animate_light;

/* IHM Functions */
void init();

