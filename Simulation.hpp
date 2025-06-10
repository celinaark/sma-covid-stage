#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <vector>
#include <string>
#include <fstream>  // Pour la sauvegarde des résultats CSV
#include <random>   // Pour std::mt19937 et les distributions

// Inclure les définitions des classes Agent et Grille
#include "Agent.hpp"
#include "Grille.hpp"

// Structure pour stocker les résultats agrégés à chaque itération
struct ResultatIteration {
    int iteration;
    int nb_S;
    int nb_E;
    int nb_I;
    int nb_R;
};

class Simulation {
public:
    // Paramètres de la simulation (publics pour un accès facile depuis main, ou via getters si encapsulation préférée)
    int nb_individus;
    int nb_infectes_init;
    double moyenne_dE;       // Paramètre pour la distribution exponentielle de la durée en état E
    double moyenne_dI;       // Paramètre pour la distribution exponentielle de la durée en état I
    double moyenne_dR;       // Paramètre pour la distribution exponentielle de la durée en état R
    double force_infection;
    int nb_max_iterations;

    // État de la simulation
    Grille grille;                             // La simulation "possède" une grille
    std::vector<Agent> liste_agents;           // La simulation "possède" les agents. La grille stockera des pointeurs.
    int iteration_actuelle;
    std::vector<ResultatIteration> resultats_par_iteration; // Stockage des résultats

    // Moteur de génération de nombres pseudo-aléatoires (PRNG) et distributions
    std::mt19937 prng_engine; // Moteur Mersenne Twister
    std::uniform_real_distribution<double> uniform_dist_01; // Pour probabilités [0,1)

    // Constructeur
    Simulation(
        int _nb_individus,
        int taille_grille_x,
        int taille_grille_y,
        int _nb_infectes_init,
        double _moyenne_dE,
        double _moyenne_dI,
        double _moyenne_dR,
        double _force_infection = 0.5,
        int _nb_max_iterations = 730
    );

    // Méthodes principales de la simulation
    void initialiser_simulation(unsigned int seed); // Initialise ou réinitialise la simulation avec une graine
    void lancer_une_iteration();                   // Exécute un pas de temps de la simulation
    void lancer_simulation_complete(unsigned int seed); // Lance la simulation complète pour nb_max_iterations

    // Méthodes utilitaires
    void collecter_statistiques();                 // Compte les agents S, E, I, R et stocke
    void sauvegarder_resultats_csv(const std::string& nom_fichier) const; // Sauvegarde les résultats dans un fichier

private:
    // Fonction utilitaire privée pour générer les durées dE, dI, dR
    double generer_temps_exponentiel(double moyenne);
};

#endif // SIMULATION_HPP
