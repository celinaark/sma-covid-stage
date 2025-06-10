#ifndef AGENT_HPP
#define AGENT_HPP

#include <string>
#include <random> // Pour std::mt19937 si utilisé directement ici, sinon Simulation le gère

// Forward declaration pour éviter les dépendances circulaires si Agent a besoin de connaître Grille
class Grille;

enum class Statut { S, E, I, R };

class Agent {
public:
    int agent_id;
    int pos_x;
    int pos_y;
    double dE; // Durée d'exposition
    double dI; // Durée d'infection
    double dR; // Durée de récupération/immunité
    Statut statut;
    int temps_dans_statut;

    Agent(int id, int x, int y, double _dE, double _dI, double _dR, Statut _statut = Statut::S);

    // Utilise le PRNG de la simulation
    void se_deplacer(int grille_taille_x, int grille_taille_y, std::mt19937& prng_engine);

    // La grille sera passée en argument pour que l'agent puisse vérifier son voisinage
    // random_val_01 est un nombre aléatoire [0,1) fourni par la simulation
    void verifier_infection(Grille& grille, double force_infection, double random_val_01);

    void mettre_a_jour_statut_exposition();
    void mettre_a_jour_statut_infection();
    void mettre_a_jour_statut_retablissement();
    void incrementer_temps_dans_statut();

    std::string statut_to_string() const; // Pour l'affichage (optionnel)
};

#endif // AGENT_HPP
