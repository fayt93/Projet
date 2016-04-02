
/*


Le main ce sera toujours le même.
la fonction annime on esdt pas obligé de la faire, elle est optionnel
du coup on enlever aussl le lancemenr del a fonctionnaime dans le main.
ENsuite on a la fonction name.


*/

/* void g3x_Material(G3Xcolor col, float ambi, float diff, float spec, float shine, float alpha); */

g3x_Material(G3Xred,0.1,0.5,0.5,0.,0.);
glPushMatric();
glTranslated(1.,1.,1.);
glRotatef(30.,1.,1.,1.);
glScalef(2.,1.,0.5);
glutSolidCube(2.)
glPopMatrix();:



shine : la briance de l'objet.


