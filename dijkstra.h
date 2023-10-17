#ifndef _DIJKSTRA_H_
#define _DIJKSTRA_H_

extern uint16_t **init_dijkstra(struct graphe *g, uint8_t idx_sommet_deb);

extern struct sommet *trouve_min(struct graphe *g, uint16_t **tab_dijkstra);

extern void maj_distances(uint16_t **tab_dijkstra, struct sommet *s_base, struct sommet *s_candidat);

extern uint16_t **dijkstra(struct graphe *g, struct sommet *sommet_deb);

extern void affiche_tab_dijkstra(struct graphe *g, uint16_t **tab_dijkstra);

extern void affiche_chemin_sommet(struct sommet *s, bool is_there_arrow);

#endif /* _DIJKSTRA_H_ */