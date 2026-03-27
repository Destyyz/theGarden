#ifndef __VISU_CM_H
#define __VISU_CM_H

#define STEP_ANGLE	1.
#define STEP_PROF	1.

/* variables globales pour la gestion de la caméra */
extern float profondeur;
extern float latitude;
extern float longitude;
extern float ratio;

/* variables globales pour la gestion de l'objet */
extern float obj_rot;
extern bool flag_anim;

/* Déclaration des fonctions */
void init();

#endif
