#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ecosys.h"


Animal *creer_animal(int x, int y, float energie) {
	Animal *na = (Animal *)malloc(sizeof(Animal));
	assert(na); //on verifie si l'espace memoire est allouee
	na->x = x;
	na->y = y;
	na->energie = energie;
	na->dir[0] = rand() % 3 - 1;  //on choisit par hasard un numero entre -1 et 1
	na->dir[1] = rand() % 3 - 1;
	na->suivant = NULL;
    return na;
}


Animal *ajouter_en_tete_animal(Animal *liste, Animal *animal) {
  	assert(animal); //on verifie si c'est pas null
  	assert(!animal->suivant); //on verifie que c'est juste un animal
  	animal->suivant = liste;
  	return animal;
}

void ajouter_animal(int x, int y,  float energie, Animal **liste_animal) {
	assert(x>=0 && x<SIZE_X);
    assert(y>=0 && y<SIZE_Y);
    Animal *animal = creer_animal(x,y,energie);
    assert(animal);
    Animal *ajouter_en_tete_animal(Animal *liste, Animal *animal);
    *liste_animal = ajouter_en_tete_animal(*liste_animal,animal);
}


void enlever_animal(Animal **liste, Animal *animal) {

	//on verifie si l'animal existe
	assert(animal);
	
	//on verifie si l'element est le premier de la liste
    if (*liste == animal) {
    	*liste = animal->suivant;
    	free(animal);
    	return;
	}

	//sinon, on parcourt la liste, pour trouver l'element
	Animal *current = *liste;
	while(current->suivant){
    	if (current->suivant == animal) {
        	current->suivant = animal->suivant;
            free(animal);
            return;
        }
        current = current->suivant;
	}
	return ;
}


Animal* liberer_liste_animaux(Animal *liste) {
	Animal *tmp;
	while (liste) {
    	tmp = liste;
        liste = liste->suivant;
        free(tmp);
   }
	return NULL;
}



unsigned int compte_animal_rec(Animal *la) {
 	if (!la) return 0;
 	return 1 + compte_animal_rec(la->suivant);
}


unsigned int compte_animal_it(Animal *la) {
  	int cpt=0;
  	while (la) {
    	++cpt;
    	la=la->suivant;
  	}
  	return cpt;
}

void ecrire_ecosys(const char *nom_fichier,Animal *liste_proie, Animal *liste_predateur){

	//on ouvre le fichier, en mode d'ecriture
	FILE *f = fopen(nom_fichier,"w");
	
	//on verifie si l'ouverture a reussi
	if (f==NULL) {
		printf("Erreur ouverture %s\n",nom_fichier);
		return;
	}
	
	Animal *tmp;
	tmp = liste_proie;
	
	fprintf(f,"<proies>\n");
	while (tmp) {
		fprintf(f,"x=%d y=%d dir=[%d %d] e=%f\n",tmp->x,tmp->y,tmp->dir[0],tmp->dir[1],tmp->energie);
		tmp = tmp->suivant;
	}
	fprintf(f,"</proies>\n");
	
	tmp = liste_predateur;
	
	fprintf(f,"<predateurs>\n");
	while (tmp) {
		fprintf(f,"x=%d y=%d dir=[%d %d] e=%f\n",tmp->x,tmp->y,tmp->dir[0],tmp->dir[1],tmp->energie);
		tmp = tmp->suivant;
	}
	fprintf(f,"</predateurs>\n");
	
	fclose(f);
	
}

void lire_ecosys(const char *nom_fichier, Animal **liste_proie, Animal **liste_predateur){

	//on ouvre le fichier, en mode de lecture
	FILE *f = fopen(nom_fichier,"r");
	
	//on verifie si l'ouverture a reussi
	if (f==NULL) {
		printf("Erreur ouverture %s\n",nom_fichier);
		return;
	}
	
	Animal *ani;
	int x,y;
	int dir[2];
	float e;
	char buffer[256];
	
	fgets(buffer,256,f); //on stocke le contenu de la ligne dans buffer
	assert(strncmp(buffer,"<proies>",8)==0); //on verifie que le model de fichier est comme le modele qu'on a ecrit (il commence par <proies> etc.)
	fgets(buffer,256,f); //on avance
	
	//on fait cette boucle jusqu'on fini la liste de proies
	while (strncmp(buffer,"</proies>",9)) {
	
		//on scan le contenu du buffer
		sscanf(buffer,"x=%d y=%d dir=[%d %d] e=%f\n",&x,&y,&dir[0],&dir[1],&e);
		
		ani = creer_animal(x,y,e);
		ani->dir[0] = dir[0];
		ani->dir[1] = dir[1];
		*liste_proie = ajouter_en_tete_animal(*liste_proie,ani);
		fgets(buffer,256,f);
		
	}
	fgets(buffer,256,f);
	
	assert(strncmp(buffer,"<predateurs>",12)==0);
	fgets(buffer,256,f);
	
	//on fait cette boucle jusqu'on fini la liste de predateurs
	while (strncmp(buffer,"</predateurs>",13)) {
	
		sscanf(buffer,"x=%d y=%d dir=[%d %d] e=%f\n",&x,&y,&dir[0],&dir[1],&e);
		ani = creer_animal(x,y,e);
		ani->dir[0] = dir[0];
		ani->dir[1] = dir[1];
		*liste_predateur = ajouter_en_tete_animal(*liste_predateur,ani);
		fgets(buffer,256,f);
		
	}
	
	fclose(f);
}

void afficher_ecosys(Animal *liste_proie, Animal *liste_predateur) {
  unsigned int i, j;
  char ecosys[SIZE_X][SIZE_Y];
  Animal *pa=NULL;

  /* on initialise le tableau */
  for (i = 0; i < SIZE_X; i++) {
    for (j = 0; j < SIZE_Y; j++) {
      ecosys[i][j]=' ';
    }
  }

  /* on ajoute les proies */
  pa = liste_proie;
  while (pa) {
    ecosys[pa->x][pa->y] = '*';
    pa=pa->suivant;
  }

  /* on ajoute les predateurs */
  pa = liste_predateur;
  while (pa) {
      if ((ecosys[pa->x][pa->y] == '@') || (ecosys[pa->x][pa->y] == '*')) { /* proies aussi present */
        ecosys[pa->x][pa->y] = '@';
      } else {
        ecosys[pa->x][pa->y] = 'O';
      }
    pa = pa->suivant;
  }

  /* on affiche le tableau */
  printf("+");
  for (j = 0; j < SIZE_Y; ++j) {
    printf("-");
  }  
  printf("+\n");
  for (i = 0; i < SIZE_X; ++i) {
    printf("|");
    for (j = 0; j < SIZE_Y; ++j) {
      putchar(ecosys[i][j]);
    }
    printf("|\n");
  }
  printf("+");
  for (j = 0; j<SIZE_Y; ++j) {
    printf("-");
  }
  printf("+\n");
  int nbproie=compte_animal_it(liste_proie);
  int nbpred=compte_animal_it(liste_predateur);
  
  printf("Nb proies : %5d\tNb predateurs : %5d\n", nbproie, nbpred);

}


void clear_screen() {
  printf("\x1b[2J\x1b[1;1H");  /* code ANSI X3.4 pour effacer l'ecran */
}


void bouger_animaux(Animal *la) {
    while(la){
      	if ((float)rand()/RAND_MAX < p_ch_dir) {
        	la->dir[0] = (rand()%3)-1;
        	la->dir[1] = (rand()%3)-1;
      	}
      	la->x = (la->x + la->dir[0] + SIZE_X)%SIZE_X;
      	la->y = (la->y + la->dir[1] + SIZE_Y)%SIZE_Y;
      	la = la->suivant;
    }



}


void reproduce(Animal **liste_animal, float p_reproduce) {
   	assert(liste_animal);
   	Animal *ani = *liste_animal;
   	while(ani){
     	if ((float)rand()/RAND_MAX < p_reproduce) {
      		ajouter_animal(ani->x,ani->y,(ani->energie)/2.0,liste_animal);
      		ani->energie /= 2.0;
   		}
    	ani=ani->suivant;
   	}


}


void rafraichir_proies(Animal **liste_proie, int monde[SIZE_X][SIZE_Y]) {
    assert(liste_proie);
    Animal *ani = *liste_proie;
    Animal *tmp;
    
    //on fait bouger les animaux
	bouger_animaux(ani);
	
	//on parcourt la liste des proies
  	while (ani){
  	
  		//on baisse leur energie de 1
    	ani->energie--;
    	
    	//dans mon vision, en premier ils mangent de l'herbe (si il y a disponible) et apres on va verifier leur energie pour voir si ils vont mourir ou pas (la derniere chance)
    	if (monde[ani->x][ani->y]>0) {
    		ani->energie += monde[ani->x][ani->y];
    		monde[ani->x][ani->y] = temps_repousse_herbe;
    	}
    	//si ils vont mourir, ils vont etre effacee
    	if (ani->energie < 0){
      		tmp = ani->suivant;
      		enlever_animal(liste_proie,ani);
      		ani = tmp;
      		continue;
    	}
    	ani = ani->suivant;
    
  	}
  	
  	reproduce(liste_proie,p_reproduce_proie);

}


Animal *animal_en_XY(Animal *l, int x, int y) {
	//on parcourt la liste pour voir le premiere animal qui se situe sur les coordonnees x et y
    while (l){
    	if ( ((l->x)==x) && ((l->y)==y) ) 
    		return l;
    	l = l->suivant;
    }

  return NULL;
} 


void rafraichir_predateurs(Animal **liste_predateur, Animal **liste_proie) {
   assert(liste_predateur&&liste_proie);
   Animal *predateur = *liste_predateur;
   Animal *tmp;
   Animal *proie;
   
   //on fait bouger les animaux
   bouger_animaux(predateur);
   
   while (predateur){
   		predateur->energie--;
   		
   		//on verifie, en premier, si il y a une proie sur les memes coodonnes, alors que le predateur peut la manger, pour gagner son energie (avant de mourir)
   		proie = animal_en_XY(*liste_proie,predateur->x,predateur->y);
   		
   		//si il y a une proie elle va etre mange
       	if (proie != NULL) {
       		predateur->energie += proie->energie;
       		enlever_animal(liste_proie,proie);
       	}
       	
       	//meme si apres manger une proie (si c'etait le cas), si le predateur est epuise, il va etre elimine
    	if (predateur->energie < 0){
      		tmp = predateur->suivant;
      		enlever_animal(liste_predateur,predateur);
      		predateur = tmp;
      		continue;
    	}
       		
    	predateur = predateur->suivant;
    }
    
 	reproduce(liste_predateur,p_reproduce_predateur);

}


void rafraichir_monde(int monde[SIZE_X][SIZE_Y]){
	for (int i = 0; i < SIZE_X ; i++) {
  		for (int j = 0; j < SIZE_Y ; j++ ){
  			monde[i][j]++; //l'herbe qui augmente de 1
  	}
  }


}

