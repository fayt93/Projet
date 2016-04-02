/*=================================================================*/
/*= E.Incerti - incerti@upem.fr                                   =*/
/*= Université Paris-Est-Marne-la-Vallée                          =*/
/*= Exemple de fonctionalités de lib. graphique <g3x>             =*/
/*=================================================================*/

#include <g3x.h>
#include <g3x_macros.h>
#include <g3x_basix.h>
#include <g3x_colors.h>
#include <g3x_quaternions.h>
#include <g3x_transfo.h>
#include <g3x_capture.h>
#include <g3x_pnm.h>
#include <g3x_tools.h>
#include <g3x_types.h>

typedef struct {
	G3Xpoint p ;
	G3Xvector v ;
} Ray ;

/* des couleurs prédéfinies */  
static G3Xcolor rouge  ={1.0,0.0,0.0};
static G3Xcolor jaune  ={1.0,1.0,0.0};
static G3Xcolor vert   ={0.0,1.0,0.0};
static G3Xcolor cyan   ={0.0,1.0,1.0};
static G3Xcolor bleu   ={0.0,0.0,1.0};
static G3Xcolor magenta={1.0,0.0,1.0};
/* paramètres géométriques */
static double angle= 0.00;
static double rayon= 0.66;
/* paramètres de lumière/matière */
static double alpha= 0.5;
static double ambi = 0.2;
static double diff = 0.3;
static double spec = 0.4;
static double shin = 0.5;

#define MAXCOL 25
static G3Xcolor colmap[MAXCOL];

static G3Xvector W={1.,2.,3.};
static double    b=0.1;
void Anim(void)
{
	static double pas=0.1;
	b+=pas;
	W[0]+=pas;
	W[1]-=pas;
	W[2]+=pas;
	if (W[0]<-10. || W[0]>+10.) pas=-pas;
}

/* flag d'affichag/masquage */
static bool FLAG_TEAPOT=true;
static bool FLAG_TORUS =true;
static bool FLAG_CONE  =true;
static bool FLAG_ICOS  =true;

/*= FONCTION DE DESSIN PRINCIPALE =*/
static void Dessin(void)
{
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	/* une sphere */
	g3x_Material(rouge,ambi,diff,spec,shin,1.);
	glutSolidSphere(rayon,40,40);
	/* un cube transparent */
	/*--------------LIMITES DE LA TRANSPARENCE Gl ---------*/
	/* -> TESTER :  DEFINIR LE CUBE TRANSP. AVANT LA SPHERE */
	/* -> TESTER :  RENDRE LA SPHERE TRANSPARENTE           */
	g3x_Material(vert,ambi,diff,spec,shin,alpha);
	glutSolidCube(1.);
	glDisable(GL_BLEND);



	/* les cubes animes -> cf fonction <Anim> */
	int i=0;
	double a=360./MAXCOL;
	glPushMatrix();
	glRotatef(-2.*b,0.,0.,W[2]);
	while (i<MAXCOL)
	{
		g3x_Material(colmap[i],ambi,diff,spec,shin,1.);		
		glRotatef(a,0.,0.,1.);
		glPushMatrix();
		glTranslatef(1.,0.,0.);
		glRotatef(i*a,1.,0.,0.);
		glRotatef(20.*b,W[0],W[1],W[2]);
		glutSolidCube(1.25*PI/MAXCOL);
		glPopMatrix();
		i++;
	}
	glPopMatrix();




}



/*=    ACTION A EXECUTER EN SORTIE   =*/
/*= libération de mémoire, nettoyage =*/
/*= -> utilise la pile de <atexit()> =*/
static void Exit(void)
{
	/* rien à faire ici puisqu'il n'y a pas d'allocation dynamique */
	fprintf(stdout,"\nbye !\n");
}

/*= action : variation de couleur =*/
static void action1(void)
{
	rouge[0]-=0.01;  rouge[1]+=0.01;
	vert[1] -=0.01;  vert[2] +=0.01;  
	bleu[2] -=0.01;  bleu[0] +=0.01;
}
/*= action : variation de couleur =*/
static void action2(void)
{
	rouge[0]+=0.01;  rouge[1]-=0.01;
	vert[1] +=0.01;  vert[2] -=0.01;  
	bleu[2] +=0.01;  bleu[0] -=0.01;
}

/*= action : affiche les parametre camera dans le terminal =*/
static void camera_info(void)
{
	G3Xcamera* cam=g3x_GetCamera();
	fprintf(stderr,"position (x:%lf,y:%lf,z:%lf)\n",(*cam->pos)[0],(*cam->pos)[1],(*cam->pos)[2]);
	fprintf(stderr,"cible    (x:%lf,y:%lf,z:%lf)\n",(*cam->tar)[0],(*cam->tar)[1],(*cam->tar)[2]);
	fprintf(stderr,"coord. spheriques (d:%lf,theta:%lf,phi:%lf)\n",cam->dist,cam->theta,cam->phi);
}

static void init(void){
	;
}



void drawSphere(){

	/* DESSIN DUN CUPBE */
	g3x_Material(G3Xrb_c,0.1,0.5,0.5,0.,0.);
	glPushMatrix();
	glTranslated(1.,1.,1.);
	glRotatef(30.,1.,1.,1.); /* la rotation */
	glScalef(2.,1.,0.5); /* mise a lechelle */

	/* attention l'odre d'appell , pour 2 objet independant il faut faire 2 bloque*/

	/*glutSolidCube(2.);*/
	glutSolidSphere(2.,40.,40.);

	glPopMatrix();
}

/* il faut se ramener au cercle canonique pou faire les calcul */
/* nous ce qui nous interresse c'esdt la caméra canonique */
/* a chaque fois qu'on voudra aller un d'un repère à l'autre on sera obliger d'inverser les matrice */
/* En quoi une inversion de matrice permet de passer un d'un repère à l'autre */

void drawRect( ){


	g3x_Material(G3Xrb_c,0.1,0.5,0.5,0.,0.);
	glPushMatrix();
	glTranslated(3.,3.,3.);
	glRotatef(30.,1.,1.,1.); /* la rotation */
	glScalef(2.,1.,0.5); /* mise a lechelle */


	/* attention l'odre d'appell , pour 2 objet independant il faut faire 2 bloque*/

	glutSolidCube(2.);

	glPopMatrix();


}


void drawLigne(G3Xpoint A , G3Xpoint B , G3Xcolor col){
	/* we disable the light ( on debramche la lumiere ) */
	glDisable(GL_LIGHTING);
	/* on fixe la couleur du tracé */

	/* les couleur c'est 4f pour 4 vlauer en floatant, et pas 4d car ce ne sont pas des double */
	glColor4fv(col); /* 4 ou 3 selon si on à la 4eme constante (RGBA) , lA opour la transparance */
	glBegin(GL_LINES);
	/* on ne met que des points , tant que je mes des points, il va tracer une ligne */
	glVertex3dv(A);/* le v c'est pour vecteur */
	glVertex3dv(B);
	glEnd();
	glEnable(GL_LIGHTING);
}


/*
 * RayInterC0:
 *
 * fonction qui renvoit si une droite (un lancé de ranyon ) 
 * traverse un cercle on renvoyant true si c'est vrai et false si non
 *
 **/
bool RayInterC0( G3Xpoint A, G3Xvector u , G3Xpoint I ){

	/* on peut le faire car le cercle canonique est de centre 0 */
	double ps = G3Xprodscal(A,u); 
	if (ps >= 0 ) return false ;
	double d2 = G3Xsqrvnorm(A) - ps*ps;
	if( d2 >= 1 ) return false ;
	d2 = - (ps+sqrt(1-d2));
	I[0] = A[0] + d2*u[0];
	I[1] = A[1] + d2*u[1];
	I[2] = A[2] + d2*u[2];
	return true ;
}


/* c'est le calcul de la normal */
bool RayInterS0( Ray Au , Ray *Ir ) {
	G3Xpoint I;
	G3Xvector r;
	double ps = G3Xprodscal(Au.p,Au.v); 
	if (ps >= 0 ) return false ;
	double d2 = G3Xsqrvnorm(Au.p) - ps*ps;
	if( d2 >= 1 ) return false ;
	d2 = - (ps+sqrt(1-d2));
	I[0] = Au.p[0] + d2*Au.v[0];
	I[1] = Au.p[1] + d2*Au.v[1];
	I[2] = Au.p[2] + d2*Au.v[2];
	return true ;
}


/*
static void drawNormal( ){


	Ray Au = ;
	Ray Ir ;

	for( i = 0 ; i < 6 ; i++ ) {


		if ( RayInterS0(Au,&Ir) ) {
			draw point 
		}

	}

}
*/







/* pour savoir sile point est bien dans la facette */
bool RayInterK0( Ray Au , Ray *Ir ){
	int i = 0 ;
	static G3Xpoint nc[6]={(0,0,1),(0,0,-1),(0,1,0),(0,-1,0),(1,0,0),(-1,0,0)};
	bool flag[6] = { false , false ,false , false ,false , false };	
	if( A[0] > 1 ) flag[0]=true; else if ( A[0] < 1 ) flag[1]=true;
	if( A[1] > 1 ) flag[2]=true; else if ( A[1] < 1 ) flag[1]=true;
	if( A[2] > 1 ) flag[4]=true; else if ( A[2] < 1 ) flag[1]=true;

	for( i = 0 ; i < 6 ; i++ ) {

	/*
		if ( flag[i]=false) continue;
		ps = G3Xprodscal(u,nc[i]);
		if ( ps>=0) continue ;
		AC = vecteor(A,nc[i]);
		t = G3Xprodscal(AC,nc[i]/ps);
		I = A + Tu;
		*/
	}

	return false ;

}

/* avec un cylindre :

Si je trouve unintersection avec la face supérieur et la face inférieur, il va faloire que j'arrète.
Parcontre si non il va faloire que je trouve une intersection avec la face lateral 
Ne pas oublier de normaliser v 


*/

static void draw(){
	drawSphere( );
	drawRect();
	drawLigne((G3Xpoint){0.,0.,0.},(G3Xpoint){10.,10.,10.},G3Xyb);
	/* qunad le rayon touche un objet, il recupère la couleur de l'objet */

}
/*static void anim(void){
  ;
  }
 */
static void quit(void){
	;
}





int main(int argc, char **argv)
{
	/* creation de la fenetre - titre et tailles (pixels) */
	g3x_InitWindow(*argv,512,512);
	/* zone graphique reelle associee a la fenetre */	
	g3x_SetInitFunction(init); /* fonction d'initialisation */
	g3x_SetDrawFunction(draw); /* fonction de dessin        */
	/*g3x_SetAnimFunction(anim);*/ /* fonction d'animation      */
	g3x_SetExitFunction(quit); /* fonction de sorite        */

	/* lancement de la boucle principale */
	return g3x_MainStart();
	/* RIEN APRES CA */
}
