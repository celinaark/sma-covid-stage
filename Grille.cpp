// Grille.cpp
#include "Grille.hpp"
#include <stdexcept> // Pour std::invalid_argument ou std::out_of_range
#include <iostream>  // Pour d'éventuels messages d'erreur (std::cerr)

Grille::Grille(int tx, int ty) : taille_x(tx), taille_y(ty) {
    if (tx <= 0 || ty <= 0) {
        // Il est préférable de lancer une exception si les dimensions ne sont pas valides.
        throw std::invalid_argument("Les dimensions de la grille (taille_x, taille_y) doivent être strictement positives.");
    }
    // Redimensionne le vecteur externe (correspondant à la dimension x)
    cellules.resize(taille_x);
    for (int i = 0; i < taille_x; ++i) {
        // Redimensionne chaque vecteur interne (correspondant à la dimension y)
        cellules[i].resize(taille_y);
        // À ce stade, cellules[i][j] est un std::vector<Agent*> vide.
    }
}

void Grille::initialiser_grille() {
    // Vide la liste d'agents pour chaque cellule de la grille.
    // Utile si on veut réutiliser l'objet Grille pour une nouvelle simulation sans le reconstruire.
    for (int i = 0; i < taille_x; ++i) {
        for (int j = 0; j < taille_y; ++j) {
            cellules[i][j].clear();
        }
    }
}

// Ajoute un agent à sa position actuelle (déjà définie dans l'objet agent)
void Grille::ajouter_agent(Agent* agent) {
    if (!agent) {
        // Optionnel : gérer le cas d'un pointeur nul, bien que cela ne devrait pas arriver si bien utilisé.
        // std::cerr << "Tentative d'ajout d'un agent nul à la grille." << std::endl;
        return;
    }
    // Vérifier si les coordonnées de l'agent sont dans les limites de la grille
    if (agent->pos_x >= 0 && agent->pos_x < taille_x &&
        agent->pos_y >= 0 && agent->pos_y < taille_y) {
        cellules[agent->pos_x][agent->pos_y].push_back(agent);
    } else {
        // Gérer l'erreur : agent avec des coordonnées hors limites.
        // Ceci pourrait indiquer un problème logique ailleurs.
        // throw std::out_of_range("Tentative d'ajout d'un agent à des coordonnées hors limites.");
        // std::cerr << "Erreur: Agent " << agent->agent_id << " hors limites (" << agent->pos_x << "," << agent->pos_y << ") pour grille " << taille_x << "x" << taille_y << std::endl;
    }
}

// Retire un agent de la cellule spécifiée par old_x, old_y
void Grille::retirer_agent(Agent* agent, int old_x, int old_y) {
    if (!agent) return;

    // Vérifier si les anciennes coordonnées sont valides
    if (old_x >= 0 && old_x < taille_x &&
        old_y >= 0 && old_y < taille_y) {

        auto& agents_dans_cellule = cellules[old_x][old_y];
        // Recherche et suppression de l'agent dans le vecteur de la cellule
        for (size_t i = 0; i < agents_dans_cellule.size(); ++i) {
            if (agents_dans_cellule[i] == agent) {
                agents_dans_cellule.erase(agents_dans_cellule.begin() + i);
                break; // On suppose qu'un agent n'est listé qu'une fois par cellule
            }
        }
        // Optionnel : avertir si l'agent n'a pas été trouvé à old_pos,
        // cela pourrait indiquer une incohérence.
    } else {
        // Gérer l'erreur : anciennes coordonnées hors limites.
        // throw std::out_of_range("Tentative de retrait d'un agent de coordonnées hors limites.");
        // std::cerr << "Erreur: Coordonnées de retrait hors limites (" << old_x << "," << old_y << ") pour agent " << agent->agent_id << std::endl;
    }
}

// Met à jour la position de l'agent sur la grille.
// Prérequis : agent->pos_x et agent->pos_y contiennent déjà les NOUVELLES coordonnées.
void Grille::mettre_a_jour_position_agent(Agent* agent, int old_x, int old_y) {
    if (!agent) return;
    retirer_agent(agent, old_x, old_y); // Retire de l'ancienne position
    ajouter_agent(agent);              // Ajoute à la nouvelle position (stockée dans l'objet agent)
}

const std::vector<Agent*>& Grille::get_agents_sur_cellule(int x, int y) const {
    // Cette fonction attend des coordonnées DÉJÀ normalisées (par ex. après application de la toroidalité par l'appelant).
    // Si ce n'est pas le cas, et que x,y peuvent être hors limites brutes, il faut les normaliser ici ou lancer une erreur.
    // Pour la cohérence avec Agent::verifier_infection, l'appelant gère la toroidalité.
    if (x < 0 || x >= taille_x || y < 0 || y >= taille_y) {
        // Ceci indique une erreur logique si l'appelant était censé normaliser.
        // throw std::out_of_range("Accès à get_agents_sur_cellule avec des coordonnées brutes hors limites.");
        // Pour la robustesse, on peut retourner une référence à un vecteur vide statique.
        static const std::vector<Agent*> empty_vector;
        // std::cerr << "Avertissement : Coordonnées (" << x << "," << y << ") hors limites dans get_agents_sur_cellule. Retourne vecteur vide." << std::endl;
        return empty_vector;
    }
    return cellules[x][y];
}

// Renvoie une liste de tous les agents (pointeurs) dans le voisinage de Moore (les 8 cellules autour + la cellule centrale)
// en gérant la toroidalité.
std::vector<Agent*> Grille::get_voisinage_moore(int x_centre, int y_centre) const {
    std::vector<Agent*> agents_voisins;

    // Vérifier si les coordonnées du centre sont valides (optionnel, dépend si l'appelant garantit cela)
    if (x_centre < 0 || x_centre >= taille_x || y_centre < 0 || y_centre >= taille_y) {
        // std::cerr << "Avertissement : Coordonnées centrales hors limites pour get_voisinage_moore." << std::endl;
        return agents_voisins; // Retourne un vecteur vide
    }

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            // Calcul des coordonnées du voisin avec gestion de la toroidalité
            int voisin_x = (x_centre + dx + taille_x) % taille_x;
            int voisin_y = (y_centre + dy + taille_y) % taille_y;

            // Récupérer les agents de la cellule voisine (déjà normalisée)
            const std::vector<Agent*>& agents_cellule_actuelle = get_agents_sur_cellule(voisin_x, voisin_y);
            // Ajouter tous les agents de cette cellule à la liste des voisins
            agents_voisins.insert(agents_voisins.end(), agents_cellule_actuelle.begin(), agents_cellule_actuelle.end());
        }
    }
    return agents_voisins;
}
