#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

#include "graphe.h"
#include "dijkstra.h"


/* Initialisation du tableau associé à la sortie de l'algorithme de Dijkstra.
C'est un tableau de tableaux à 3 éléments: le nombre associé au sommet considéré
, la valeur du chemin le plus court à l'instant où l'on regarde le tableau de Dijkstra,
et un booléen indiquant si le sommet a déjà été étudié ou non par l'algorithme. */
uint16_t **init_dijkstra(struct graphe *g, uint8_t idx_sommet_deb){
    uint16_t **tab_dijkstra = calloc(g->nb_sommets, sizeof(uint16_t*));
    if (tab_dijkstra == NULL){
        perror("tab dijkstra error");
        exit(EXIT_FAILURE);
    }
    for (uint8_t i=0; i<g->nb_sommets; i++){
        tab_dijkstra[i] = calloc(3, sizeof(uint16_t));
        if (tab_dijkstra[i] == NULL){
            perror("tab dijkstra error");
            exit(EXIT_FAILURE); 
        }
        tab_dijkstra[i][0] = i;
        if(i != idx_sommet_deb){
            tab_dijkstra[i][1] = pow(2,16) - 1; /*On initialise les valeurs des autres sommets comme étant "l'infini" */
        }
        else if (i == idx_sommet_deb){
            tab_dijkstra[i][1] = 0; /* La valeur du chemin du sommet initial est elle initialisée à 0 */ 
        }
        tab_dijkstra[i][2] = 0;
    }
    return tab_dijkstra;
}

/* Trouve le sommet de distance minimale par rapport au sommet choisi à l'initialisation de l'algorithme. */
struct sommet *trouve_min(struct graphe *g, uint16_t **tab_dijkstra){
    uint16_t dist_min = pow(2,16) - 1;
    struct sommet *sommet_min;
    for (uint8_t i=0; i<g->nb_sommets; i++){
        if (tab_dijkstra[i][2] == 0 && tab_dijkstra[i][1] < dist_min){
            dist_min = tab_dijkstra[i][1];
            sommet_min = g->tab_adjacence_sommets[i];
        }
    }
    tab_dijkstra[sommet_min->id_sommet][2] = 1; /*On marque le sommet de distance minimale comme étant étudié*/
    return sommet_min;
}

/* Met à jour les distances en se demandant si c'est mieux de passer par le sommet candidat ou non. */
void maj_distances(uint16_t **tab_dijkstra, struct sommet *s_base, struct sommet *s_candidat){

    uint16_t dist_base_candidat;
    for (uint8_t i=0; i<s_base->nb_voisins; i++){ /*On récupère la distance entre le sommet choisi de base et le sommet candidat */
        if (s_base->tab_adjacence[i][0] == s_candidat->id_sommet){
            dist_base_candidat = s_base->tab_adjacence[i][1];
        }
    }

    if (tab_dijkstra[s_base->id_sommet][1] > tab_dijkstra[s_candidat->id_sommet][1] + dist_base_candidat) {
        tab_dijkstra[s_base->id_sommet][1] = tab_dijkstra[s_candidat->id_sommet][1] + dist_base_candidat;
        s_base->predecesseur = s_candidat;
    }
}


uint16_t **dijkstra(struct graphe *g, struct sommet *sommet_deb){
    uint16_t **tab_dijkstra = init_dijkstra(g, sommet_deb->id_sommet); /*Initialisation */
    struct sommet *sommet_courant;
    uint8_t tous_sommets_etudies = 0; /* Condition de sortie de la boucle infinie */
    while (tous_sommets_etudies != 1){
        for (uint8_t i=0; i<g->nb_sommets; i++){
            if (tab_dijkstra[i][2] == 0){ /* Tant qu'il reste un sommet non étudié, on applique les étapes de l'algo */
                sommet_courant = trouve_min(g, tab_dijkstra);
                for(uint8_t j=0; j<sommet_courant->nb_voisins; j++){ /*On regarde chacun des voisins du sommet courant */
                    struct sommet *sommet_voisin = g->tab_adjacence_sommets[sommet_courant->tab_adjacence[j][0]];
                    maj_distances(tab_dijkstra, sommet_voisin, sommet_courant); /*Et on fait la mise à jour des distances */
                    g->tab_adjacence_sommets[sommet_courant->id_sommet] = sommet_courant; /* Màj des informations dans la structure */
                }
                break;
            }
            if (i == g->nb_sommets - 1){ /*Ici, on a parcouru tous les sommets, et on arrête l'algorithme */
                tous_sommets_etudies = 1;
            }
        }
    }
    return tab_dijkstra;
}

void affiche_tab_dijkstra(struct graphe *g, uint16_t **tab_dijkstra){
    printf("Distances des plus courts chemins pour chaque sommet: \n");
    printf("[ ");
    for (uint8_t i=0; i<g->nb_sommets; i++){
        printf("(%u,%u) ",tab_dijkstra[i][0], tab_dijkstra[i][1]);
    }
    printf(" ]\n");
}

/*Affiche le plus court chemin entre le sommet s en paramètre et le sommet initial.
On utilisera cette fonction après avoir appliqué l'algo de Dijkstra. */

void affiche_chemin_sommet(struct sommet *s, bool is_there_arrow){
    if (s->predecesseur != NULL){
        affiche_chemin_sommet(s->predecesseur, true);
    }

    if (is_there_arrow == true){
        printf("%u -> ", s->id_sommet);
    }
    else{
        printf("%u\n", s->id_sommet);
    }
}

/*Affiche les plus courts chemins dans un graphe pour chaque sommet. */
void affiche_chemins_graphe(struct graphe *g){
    for (uint8_t i=0; i<g->nb_sommets; i++){
        printf("Chemin le plus court pour le sommet %u : \n", g->tab_adjacence_sommets[i]->id_sommet);
        affiche_chemin_sommet(g->tab_adjacence_sommets[i], false);
    }
}

int main(void){
    struct graphe *g1 = cree_graphe();
    uint16_t **tab_dijkstra_g1 = dijkstra(g1, g1->tab_adjacence_sommets[0]);
    affiche_tab_dijkstra(g1, tab_dijkstra_g1);
    affiche_chemins_graphe(g1);
    return EXIT_SUCCESS;
}


