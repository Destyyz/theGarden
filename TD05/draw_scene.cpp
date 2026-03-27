#include "draw_scene.hpp"

/// Camera parameters
float angle_theta {45.0};      // Angle between x axis and viewpoint
float angle_phy {30.0};      // Angle between z axis and viewpoint
float dist_zoom {30.0};      // Distance between origin and viewpoint

float rotate {0.0};
float balance {0.0};
bool flag_anim_rot_scale {false};
bool flag_anim_rot_arm {false};

GLBI_Engine myEngine;
GLBI_Set_Of_Points somePoints(3);
GLBI_Convex_2D_Shape ground{3};
GLBI_Set_Of_Points frame(3);
IndexedMesh* sphere;
GLBI_Convex_2D_Shape disk;
StandardMesh* cone;
GLBI_Set_Of_Points line(3);

void initScene() {
	std::vector<float> points {0.0,0.0,0.0};
	somePoints.initSet(points,1.0,1.0,1.0);

	std::vector<float> baseCarre{-10.0,-10.0,0.0,
								 10.0,-10.0,0.0,
								 10.0,10.0,0.0,
								 -10.0,10.0,0.0};
	ground.initShape(baseCarre);
	ground.changeNature(GL_TRIANGLE_FAN);

	frame.initSet({0,0,0, 10,0,0,
				0,0,0, 0,10,0,
				0,0,0, 0,0,10},
				{1.0,0.0,0.0,
				1.0,0.0,0.0, 
				0.,1.0,0.0,
				0.,1.0,0.0,
				0.,0.0,1.0,
				0.,0.0,1.0});
	frame.changeNature(GL_LINES);

	sphere = basicSphere();
	sphere->createVAO();
	
	std::vector<float> circle_pts = {};
	int numPoints = 100;
	for (int i = 0; i < numPoints; i++) {
		float angle = i * 2.0f * M_PI / numPoints;
		circle_pts.push_back(cos(angle));
		circle_pts.push_back(sin(angle));
	}

	disk.initShape(circle_pts);
	disk.changeNature(GL_TRIANGLE_FAN);
	cone = basicCone(1., 1.);
	cone->createVAO();

	std::vector<float> linePoint {
		0.,0.,5., 6 * (float) cos(2*M_PI/3), 6 * (float) sin(2*M_PI/3), 0.,
		0.,0.,5., 6 * (float) cos(4*M_PI/3), 6 * (float) sin(4*M_PI/3), 0.,
		0.,0.,5., 6 * (float) cos(0), 6 * (float) sin(0), 0.
	};
	line.initSet(linePoint, 1.0, 0., 0.);
	line.changeNature(GL_LINES);
}

void drawFrame() {
	frame.drawSet();
}

void drawBase() {
	myEngine.setFlatColor(235./255.,207./255.,52./255.);
	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addTranslation({0., 0., 0.01});
		myEngine.mvMatrixStack.pushMatrix();
			myEngine.mvMatrixStack.addHomothety(6.);
			myEngine.updateMvMatrix();
			disk.drawShape();
		myEngine.mvMatrixStack.popMatrix();
	myEngine.mvMatrixStack.popMatrix();

	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addRotation(M_PI_2, {1, 0., 0.});
			myEngine.mvMatrixStack.pushMatrix();
			myEngine.mvMatrixStack.addHomothety({4., 10., 4.});
			myEngine.updateMvMatrix();
			cone->draw();
		myEngine.mvMatrixStack.popMatrix();
	myEngine.mvMatrixStack.popMatrix();

	myEngine.updateMvMatrix();
}

void drawArm() {
	myEngine.setFlatColor(245./255.,164./255.,66./255.);
	myEngine.mvMatrixStack.pushMatrix();

		myEngine.mvMatrixStack.addTranslation({0., 0., 11.6});
		myEngine.mvMatrixStack.addRotation(M_PI/9 * sin(balance / 10), {1., 0., 0.});
		myEngine.mvMatrixStack.pushMatrix();
			myEngine.mvMatrixStack.addHomothety(3.2);
			myEngine.updateMvMatrix();
			sphere->draw();
		myEngine.mvMatrixStack.popMatrix();

		myEngine.mvMatrixStack.pushMatrix();
			myEngine.mvMatrixStack.addHomothety({2., 10., 2.});
			myEngine.updateMvMatrix();
			cone->draw();
			myEngine.mvMatrixStack.pushMatrix();
				myEngine.mvMatrixStack.addRotation(M_PI, {1., 0., 0.});
				myEngine.updateMvMatrix();
				cone->draw();
			myEngine.mvMatrixStack.popMatrix();
		myEngine.mvMatrixStack.popMatrix();

	myEngine.mvMatrixStack.popMatrix();
	myEngine.updateMvMatrix();
}

void drawPan() {

	myEngine.setFlatColor(89./255.,115./255.,0.);
	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addTranslation({0., 0., -5.});
		myEngine.mvMatrixStack.pushMatrix();
			myEngine.mvMatrixStack.addHomothety(6.);
			myEngine.updateMvMatrix();
			disk.drawShape();
		myEngine.mvMatrixStack.popMatrix();
		myEngine.updateMvMatrix();
		line.drawSet();
	myEngine.mvMatrixStack.popMatrix();
	myEngine.updateMvMatrix();
}

