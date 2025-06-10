#ifndef GRILLE_HPP
#define GRILLE_HPP

#include <vector>
#include <map> // Bien que non utilisé dans l'implémentation actuelle de Grille.cpp,
             // c'était une option envisagée. Laisser pour réflexion ou retirer si non pertinent.
             // L'implémentation actuelle utilise vector<vector<vector<Agent*>>>.

// Il est crucial que Agent.hpp soit inclus ici si Grille stocke des Agent*
// ou si ses méthodes prennent/retournent des types Agent ou Agent*.
#include "Agent.hpp" // Grille a besoin de connaître la définition complète d'Agent

class Grille {
public:
    int taille_x;
    int taille_y;

    // Structure pour stocker les agents :
    // Un vecteur de vecteurs (représentant les lignes y)
    // de vecteurs (représentant les colonnes x)
    // de pointeurs d'Agent (la liste des agents sur la cellule [x][y]).
    std::vector<std::vector<std::vector<Agent*>>> cellules;

    Grille(int tx, int ty);

    void initialiser_grille(); // Pour (ré)initialiser/vider la grille

    // Ajoute un pointeur d'agent à la cellule correspondant aux coordonnées actuelles de l'agent
    void ajouter_agent(Agent* agent);

    // Retire un pointeur d'agent de la cellule spécifiée par old_x, old_y
    void retirer_agent(Agent* agent, int old_x, int old_y);

    // Met à jour la position de l'agent sur la grille.
    // L'agent doit déjà avoir ses nouvelles coordonnées (pos_x, pos_y) mises à jour.
    void mettre_a_jour_position_agent(Agent* agent, int old_x, int old_y);

    // Retourne une référence constante au vecteur de pointeurs d'agents pour la cellule (x,y).
    // L'appelant doit s'assurer que x et y sont dans les limites ou gérer la toroidalité avant l'appel.
    const std::vector<Agent*>& get_agents_sur_cellule(int x, int y) const;

    // Méthode utilitaire (non utilisée directement par Agent::verifier_infection dans la version actuelle,
    // car Agent::verifier_infection gère lui-même la boucle de voisinage).
    // Pourrait être utile pour d'autres logiques ou si Agent::verifier_infection était refactorisé.
    std::vector<Agent*> get_voisinage_moore(int x, int y) const;
};

#endif // GRILLE_HPP
