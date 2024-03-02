#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "ecosys.h"

float p_ch_dir=0.4; //0.01
float p_reproduce_proie=0.2; //0.4
float p_reproduce_predateur=0.25; //0.5
int temps_repousse_herbe=-13; //-15

int main(void){

	
	Animal *liste_proies = NULL;
	Animal *liste_predateurs = NULL;
	int energie=10;
	
	srand(time(NULL));

	
	
	for (int i=0; i<20;i++){  // 20 animaux pour chaque liste
	
		//une proie est ajoutee
		int x_proie=rand()%SIZE_X;
		int y_proie=rand()%SIZE_Y;
		ajouter_animal(x_proie,y_proie,energie,&liste_proies);
		
		//un predateur est ajoute
		int x_predateur=rand()%SIZE_X;
		int y_predateur=rand()%SIZE_Y;
		ajouter_animal(x_predateur,y_predateur,energie,&liste_predateurs);
	}
	
	//on teste si on a 20 des animaux dans chaque liste
	assert(compte_animal_rec(liste_proies)==20);
	assert(compte_animal_rec(liste_proies)==20);
	
	afficher_ecosys(liste_proies,liste_predateurs);
	
	//on ajoute 2 animaux specifiques pour tester les fonctions d'addition et de supprimer
	Animal *a1 = creer_animal(1,1,10);
	Animal *a2 = creer_animal(2,2,10);
	
	liste_proies = ajouter_en_tete_animal(liste_proies,a1);
	liste_predateurs = ajouter_en_tete_animal(liste_predateurs,a2);
	
	assert(compte_animal_rec(liste_proies)==21);
	assert(compte_animal_rec(liste_predateurs)==21);
	
	afficher_ecosys(liste_proies,liste_predateurs);
	
	enlever_animal(&liste_proies,a1);
	enlever_animal(&liste_predateurs,a2);
	
	
	assert(compte_animal_rec(liste_proies)==20);
	assert(compte_animal_rec(liste_predateurs)==20);
	
	afficher_ecosys(liste_proies,liste_predateurs);
	
	
	//on teste les fonctions d'ecriture et de lecture
	
	//on ecrit les proies et les predateurs dans un fichier "ecosys.txt"
	ecrire_ecosys("ecosys.txt",liste_proies,liste_predateurs);
	
	//on initialise les liste dupliquees, pour verifier si ils sont les memes
	Animal *liste_proies_2=NULL;
	Animal *liste_predateurs_2=NULL; 
	
	
	lire_ecosys("ecosys.txt",&liste_proies_2,&liste_predateurs_2);
	
	printf("\n\nLes listes dupliquees \n");
	afficher_ecosys(liste_proies_2,liste_predateurs_2);

	
	liste_proies_2 = liberer_liste_animaux(liste_proies_2);
	liste_predateurs_2 = liberer_liste_animaux(liste_predateurs_2);

	//on elibere les listes pour pas avoir des fuites memoires
	liste_proies = liberer_liste_animaux(liste_proies);
	liste_predateurs = liberer_liste_animaux(liste_predateurs);
	
	return 0;
}
