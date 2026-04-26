#include "init_tree.hpp"
#include "tools/shaders.hpp"
#include "open_file.hpp"

extern GLBI_Engine myEngine;
extern StandardMesh* coneMesh;
extern IndexedMesh* cylinderMesh;
extern IndexedMesh* sphereMesh;

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

		auto offset = 0.4f;
		auto height = 1.2f;
		auto width = 0.5f;
		
		myEngine.setFlatColor(55/255., 100/255., 2/255.);
		for (auto i = 0; i < 3; i++){

			myEngine.mvMatrixStack.pushMatrix();
				myEngine.mvMatrixStack.addTranslation({0., offset, 0.});
				myEngine.mvMatrixStack.addHomothety({width, 0.02, width});
				myEngine.updateMvMatrix();
				sphereMesh->draw();
			myEngine.mvMatrixStack.popMatrix();

			myEngine.mvMatrixStack.pushMatrix();
				myEngine.mvMatrixStack.addTranslation({0., offset, 0.});
				myEngine.mvMatrixStack.addHomothety({width, height, width});
				myEngine.updateMvMatrix();
				coneMesh->draw();
			myEngine.mvMatrixStack.popMatrix();


			offset += 0.2f;
			height -= 0.2f;
			width -= 0.05f;
			
		}
		
	myEngine.mvMatrixStack.popMatrix();
	myEngine.setFlatColor(1., 1., 1.);
}