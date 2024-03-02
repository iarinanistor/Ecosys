#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <strings.h>
#include "ecosys.h"



#define NB_PROIES 20
#define NB_PREDATEURS 50
#define T_WAIT 40000


/* Parametres globaux de l'ecosysteme (externes dans le ecosys.h)*/
float p_ch_dir = 0.4; //0.01
float p_reproduce_proie = 0.2; //0.4
float p_reproduce_predateur = 0.25; //0.5
int temps_repousse_herbe = -13; //-15


int main(void) {
 
   
    srand(time(NULL));
  	int tab[SIZE_X][SIZE_Y];
  	for (int i = 0; i < SIZE_X ; i++) {
  		for (int j = 0; j < SIZE_Y ; j++ ){
  			tab[i][j] = 0;
  		}
  	}
  	
  	Animal *liste_proies = NULL;
  	Animal *liste_predateurs = NULL;
  	
  	int energie=10;

  	
  	for (int i=0; i<NB_PROIES;i++){
    	int x_proie=rand()%SIZE_X;
    	int y_proie=rand()%SIZE_Y;
    	ajouter_animal(x_proie,y_proie,energie,&liste_proies);
    	
    	int x_predateur=rand()%SIZE_X;
    	int y_predateur=rand()%SIZE_Y;
    	ajouter_animal(x_predateur,y_predateur,energie,&liste_predateurs);
  	}
  	
    assert(compte_animal_rec(liste_proies)==20);
    afficher_ecosys(liste_proies,liste_predateurs);
    
   
    FILE *f = fopen("Evol_Pop.txt","w");
    if (f == NULL){
    	printf("Erreur ouverture Evol_Pop.txt\n");
    }
    
    int cpt = 0;
	while(((compte_animal_it(liste_proies)!=0)||(compte_animal_it(liste_predateurs)!=0))&&cpt<250){
    	cpt++;
    	
    	//l'herbe pousse
      	rafraichir_monde(tab);
      	//on fait la gestion d'energie (et les autres taches) en premier pour les predateurs et apres pour les proies
      	rafraichir_predateurs(&liste_predateurs,&liste_proies);
      	rafraichir_proies(&liste_proies,tab);
      
      	fprintf(f,"%d %d %d\n",cpt,(compte_animal_it(liste_proies)),(compte_animal_it(liste_predateurs)));
      	afficher_ecosys(liste_proies,liste_predateurs);
      	//pour avoir le temps de voir l'etat de l'ecosysteme
      	usleep(T_WAIT);
      
    }
    
    fclose(f);
     
    
    
    liste_proies = liberer_liste_animaux(liste_proies);
    liste_predateurs = liberer_liste_animaux(liste_predateurs);



  return 0;
}

