#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>

#include "graphe.h"

/* Alloue la mémoire associée, et construit un sommet */
struct sommet* construit_sommet(uint8_t id_sommet, uint16_t **tab_adjacence, uint8_t nb_voisins){
    struct sommet* nv_sommet = calloc(1, sizeof(struct sommet));
    if (nv_sommet == NULL){
        perror("erreur sommet");
        exit(EXIT_FAILURE);
    }  
    nv_sommet->predecesseur = NULL; 
    nv_sommet->id_sommet = id_sommet;
    nv_sommet->tab_adjacence = tab_adjacence;
    nv_sommet->nb_voisins = nb_voisins;
    return nv_sommet;
}

void affiche_sommet(struct sommet *s){
    printf("Affichage du sommet numéro %u\n", s->id_sommet);
    for (size_t i=0; i<s->nb_voisins; i++){
        printf("( %u , %u )\n",s->tab_adjacence[i][0],s->tab_adjacence[i][1]);
    } 
}

/* Libère la mémoire associée un sommet */
void libere_sommet(struct sommet *s){
    for (uint8_t i=0; i<s->nb_voisins; i++){
        free(s->tab_adjacence[i]);
    }
    free(s->tab_adjacence);
    free(s);
}

/* Crée un graphe, représenté par un tableau de sommets. 
L'utilisateur entre les paramètres liés au tableau d'adjacence de chaque sommet,
et ces derniers sont créés et implantés itérativement dans le tableau d'adjacence
caractérisant le graphe.*/
struct graphe *cree_graphe(){

    uint8_t mode = 2;
    while (mode != 0 && mode != 1){
        printf("Graphe implanté à la main (0) ou graphe aléatoire? (1)?\n");
        scanf("%hhd", &mode);
    }


    struct graphe *nv_graphe = calloc(1, sizeof(struct graphe));
    if (nv_graphe == NULL){
        perror("graph error");
        exit(EXIT_FAILURE);
    }

    if(mode == 0){ /*Ici, on construit un graphe à la main */

        uint8_t nb_sommets;
        printf("Nombre de sommets dans le graphe? \n");
        scanf("%hhd", &nb_sommets);

        struct sommet **tab_graphe = calloc(nb_sommets, sizeof(struct sommet));
        if (tab_graphe == NULL){
            perror("tab graph error");
            exit(EXIT_FAILURE);
        }

        for (uint8_t i=0; i<nb_sommets; i++){
            /* On crée le tableau d'adjacence associé au sommet considéré */

            uint8_t nb_voisins;
            printf("Nombre de voisins du sommet %u ? \n", i);
            scanf("%hhd", &nb_voisins);
            uint16_t **tab_adjacence_sommet = calloc(nb_voisins, sizeof(uint16_t*));

            for (uint8_t j=0; j<nb_voisins; j++){
                /* On définit alors chaque voisin, ainsi que le poids associé */
                
                tab_adjacence_sommet[j] = calloc(2, sizeof(uint16_t));
                if (tab_adjacence_sommet[j] == NULL){
                    perror("couple error");
                    exit(EXIT_FAILURE);
                }
                uint16_t voisin;
                uint16_t poids;
                printf("Quel est le voisin numéro %u ? \n", j+1);
                scanf("%hd", &voisin);
                printf("Quel est le poids du voisin %u ? \n", j+1);
                scanf("%hd", &poids);
                tab_adjacence_sommet[j][0] = voisin;
                tab_adjacence_sommet[j][1] = poids;
            }

            struct sommet *sommet_courant = construit_sommet(i, tab_adjacence_sommet, nb_voisins);
            tab_graphe[i] = sommet_courant;
        }

        nv_graphe->tab_adjacence_sommets = tab_graphe;
        nv_graphe->nb_sommets = nb_sommets;

        for (uint8_t i=0; i<nb_sommets; i++){
            affiche_sommet(nv_graphe->tab_adjacence_sommets[i]);
        }
        printf("Nombre de sommets construits: %u\n", nv_graphe->nb_sommets);

    }

    else if (mode == 1) { /*Ici, on construit un graphe aléatoire */
        srand(time(NULL));

        uint8_t nb_sommets_alea = (rand() % 255) + 1;

        struct sommet **tab_graphe = calloc(nb_sommets_alea, sizeof(struct sommet));
        if (tab_graphe == NULL){
            perror("tab graph error");
            exit(EXIT_FAILURE);
        }

        for (uint8_t i=0; i<nb_sommets_alea; i++){
            /* On crée le tableau d'adjacence associé au sommet considéré */

            uint8_t nb_voisins = (rand() % ((nb_sommets_alea)-1))+1;
            uint16_t **tab_adjacence_sommet = calloc(nb_voisins, sizeof(uint16_t*));

            for (uint8_t j=0; j<nb_voisins; j++){
                /* On définit alors chaque voisin, ainsi que le poids associé */

                tab_adjacence_sommet[j] = calloc(2, sizeof(uint16_t));
                if (tab_adjacence_sommet[j] == NULL){
                    perror("couple error");
                    exit(EXIT_FAILURE);
                }
                uint16_t voisin = rand() % nb_sommets_alea;

                while (voisin == i){ /*Un sommet ne peut pas avoir lui-même comme voisin */
                    voisin = rand() % nb_sommets_alea;
                }

                uint16_t poids = rand() % 11;  

                tab_adjacence_sommet[j][0] = voisin;
                tab_adjacence_sommet[j][1] = poids;          

            }

            struct sommet *sommet_courant = construit_sommet(i, tab_adjacence_sommet, nb_voisins);
            tab_graphe[i] = sommet_courant;
        }

        nv_graphe->tab_adjacence_sommets = tab_graphe;
        nv_graphe->nb_sommets = nb_sommets_alea;

        for (uint8_t i=0; i<nb_sommets_alea; i++){
            affiche_sommet(nv_graphe->tab_adjacence_sommets[i]);
        }
        printf("Nombre de sommets construits: %u\n", nv_graphe->nb_sommets);
    }

    return nv_graphe;
}

/* Libère la mémoire associée au graphe considéré. */
void libere_graphe(struct graphe *g){
    for (uint8_t i=0; i<g->nb_sommets; i++){
        libere_sommet(g->tab_adjacence_sommets[i]);
    }
    free(g->tab_adjacence_sommets);
    free(g);
}

/* Banc de tests du module. */

/*
int main(){

   struct graphe *nv_graphe = cree_graphe(6);
   libere_graphe(nv_graphe);

   return EXIT_SUCCESS;
}
*/
