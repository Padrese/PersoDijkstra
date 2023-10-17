#ifndef _GRAPHE_H_
#define _GRAPHE_H_

/* Définit un sommet dans un graphe pondéré (avec des valeurs positives seulement) */
struct sommet {
    struct sommet *predecesseur; /* Prédecesseur du sommet dans le chemin déterminé par l'algo */
    uint8_t id_sommet; /*On en a besoin dans l'application de l'algo de Dijkstra! */
    uint16_t **tab_adjacence; /* Tableau des voisins et des distances */ 
    uint8_t nb_voisins;
};

struct graphe {
    struct sommet **tab_adjacence_sommets; /* Tableau des sommets du graphe */
    uint8_t nb_sommets;
};

extern struct sommet* construit_sommet(uint8_t id_sommet, uint16_t **tab_adjacence, uint8_t nb_voisins);

extern void affiche_sommet(struct sommet *s);

extern void libere_sommet(struct sommet *s);

extern struct graphe *cree_graphe();

extern void libere_graphe(struct graphe *g);

#endif /* _GRAPHE_H_ */