#include "scene.hpp"
#include "visu.hpp"

void Scene::addOneObject(unsigned int typ_obj,float* diffuse,float* trans,
                         float rot,int typ_rot,float scale) {
    les_objets.push_back(new GLObject(typ_obj,diffuse));
    for(unsigned int i=0;i<3;i++) {translations.push_back(trans[i]);}
    rotation_angles.push_back(rot);
    rotation_axes.push_back(typ_rot);
    scales.push_back(scale);
    nb_object++;
}

void Scene::createScene() {
    float tr1[3] = {0.0f,-1.0f,0.0f};
    float col1[3] = {1.0f,1.0f,1.0f};
    addOneObject(QUAD,col1,tr1,0.0,2,10.0);

    float tr2[3] = {2.0f,0.0f,0.0f};
    float col2[3] = {1.0f,1.0f,0.0f};
    addOneObject(SPHERE,col2,tr2,0.0,2,0.5);

    float tr3[3] = {1.0f,0.0f,0.0f};
    float col3[3] = {0.0f,0.8f,0.2f};
    addOneObject(CUBE,col3,tr3,0.0,2,0.25);
}

void Scene::makeRot(int trot,float arot) {
    if(trot==1) {
        // Axe X
        glRotatef(arot,1.0,0.0,0.0);
    }
    else if (trot==2) {
        glRotatef(arot,0.0,1.0,0.0);
    }
    else {
        glRotatef(arot,0.0,0.0,1.0);
    }
}

void Scene::drawScene(float cur_time) {
    glPushMatrix();

    // Le sol
    int i = 0;
    glPushMatrix();
        glTranslatef(translations[3*i],translations[3*i+1],translations[3*i+2]);
        makeRot(rotation_axes[i],rotation_angles[i]);
        glScalef(scales[i],scales[i],scales[i]);
        les_objets[i]->glDrawObject();
    glPopMatrix();
    
    i++;
    // La planete 
    glPushMatrix();
        makeRot(2,cur_time);
        glTranslatef(translations[3*i],translations[3*i+1],translations[3*i+2]);
        glPushMatrix();
            makeRot(rotation_axes[i],rotation_angles[i]);
            glScalef(scales[i],scales[i],scales[i]);
            les_objets[i]->glDrawObject();
        glPopMatrix();
        i++;
        glPushMatrix();
            makeRot(2,-7*cur_time);
            glTranslatef(translations[3*i],translations[3*i+1],translations[3*i+2]);
            glPushMatrix();
                makeRot(rotation_axes[i],rotation_angles[i]-0*cur_time);
                glScalef(scales[i],scales[i],scales[i]);
                les_objets[i]->glDrawObject();
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();    
    
	glPopMatrix();
}


