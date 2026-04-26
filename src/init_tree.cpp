#include "init_tree.hpp"
#include "tools/shaders.hpp"
#include "open_file.hpp"

extern GLBI_Engine myEngine;
extern StandardMesh* coneMesh;
extern IndexedMesh* cylinderMesh;

void initTree() {
    coneMesh = basicCone(1, 1);
    coneMesh->createVAO();
    cylinderMesh = STP3D::basicCylinder(1, 1);
    cylinderMesh->createVAO();
}

void renderTree() {
	myEngine.setFlatColor(101/255.,67/255.,33/255.);

	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addRotation(M_PI_2, {1, 0., 0.});
		myEngine.updateMvMatrix();

		myEngine.mvMatrixStack.pushMatrix();
			myEngine.mvMatrixStack.addHomothety({0.1, 0.8, 0.1});
			myEngine.updateMvMatrix();
			cylinderMesh->draw();
		myEngine.mvMatrixStack.popMatrix();

		myEngine.setFlatColor(55/255.,100/255.,2/255.);
		myEngine.mvMatrixStack.pushMatrix();
			myEngine.mvMatrixStack.addHomothety({0.5, 1.2, 0.5});
			myEngine.mvMatrixStack.addTranslation({0., 0.4, 0.});
			myEngine.updateMvMatrix();
			coneMesh->draw();
		myEngine.mvMatrixStack.popMatrix();
	myEngine.mvMatrixStack.popMatrix();
	myEngine.setFlatColor(1., 1., 1.);
}