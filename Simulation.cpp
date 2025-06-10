// Simulation.cpp
#include "Simulation.hpp"
#include <iostream>  // Pour std::cout, std::cerr pour les messages et debugging
#include <algorithm> // Pour std::shuffle pour l'ordre aléatoire des agents
#include <vector>    // Utilisation intensive de std::vector
#include <cmath>     // Pour std::log (dans generer_temps_exponentiel) et std::exp
#include <stdexcept> // Pour std::invalid_argument (optionnel, pour une gestion d'erreur plus stricte)

// Constructeur
Simulation::Simulation(
    int _nb_individus,
    int taille_grille_x,
    int taille_grille_y,
    int _nb_infectes_init,
    double _moyenne_dE,
    double _moyenne_dI,
    double _moyenne_dR,
    double _force_infection,
    int _nb_max_iterations)
    : nb_individus(_nb_individus),
      nb_infectes_init(_nb_infectes_init),
      moyenne_dE(_moyenne_dE),
      moyenne_dI(_moyenne_dI),
      moyenne_dR(_moyenne_dR),
      force_infection(_force_infection),
      nb_max_iterations(_nb_max_iterations),
      grille(taille_grille_x, taille_grille_y), // Initialise la grille avec ses dimensions
      iteration_actuelle(0),
      // prng_engine est initialisé par défaut, sera seedé dans initialiser_simulation
      uniform_dist_01(0.0, 1.0) // Distribution pour [0.0, 1.0)
{
    if (_nb_individus <= 0 || _nb_infectes_init < 0 || _nb_infectes_init > _nb_individus) {
        throw std::invalid_argument("Nombre d'individus ou d'infectés initiaux invalide.");
    }
    if (moyenne_dE <= 0 || moyenne_dI <= 0 || moyenne_dR <= 0) {
        throw std::invalid_argument("Les durées moyennes dE, dI, dR doivent être positives.");
    }
    // Pré-allouer de la mémoire pour les vecteurs si les tailles sont connues
    liste_agents.reserve(nb_individus);
    resultats_par_iteration.reserve(nb_max_iterations + 1); // +1 pour l'état initial à l'itération 0
}

// Fonction utilitaire privée pour générer les durées dE, dI, dR
double Simulation::generer_temps_exponentiel(double moyenne) {
    // Assure que la moyenne est positive (déjà vérifié dans le constructeur pour les paramètres principaux)
    if (moyenne <= 0) {
        // Normalement, cela ne devrait pas être atteint si les paramètres de sim sont valides.
        // Pourrait retourner une valeur par défaut ou lancer une exception si appelé incorrectement.
        return 1e9; // Une "grande" valeur arbitraire
    }
    // Génère un nombre aléatoire u dans [0, 1)
    double u = uniform_dist_01(prng_engine);
    // Pour éviter log(0) si u est 1.0 (uniform_dist_01 génère dans [min, max) donc u < 1.0)
    // Cependant, pour être absolument sûr avec l'arithmétique à virgule flottante et log(1-u)
    // si u est très proche de 1, 1-u peut être 0 ou très petit.
    // log(x) requiert x > 0.
    if (u == 1.0) { // Si uniform_dist_01 pouvait retourner 1.0 (ce n'est pas le cas pour std::uniform_real_distribution)
        u = std::nextafter(1.0, 0.0); // Le plus grand double représentable inférieur à 1.0
    }
    // Si u est 0.0, log(1.0 - 0.0) = log(1.0) = 0. Cela donne une durée de 0.
    return -moyenne * std::log(1.0 - u);
}

void Simulation::initialiser_simulation(unsigned int seed) {
    prng_engine.seed(seed); // Initialise le moteur PRNG avec la graine fournie
    iteration_actuelle = 0;

    liste_agents.clear();       // S'assurer que la liste est vide avant de la remplir
    grille.initialiser_grille(); // S'assurer que la grille est vide
    resultats_par_iteration.clear();

    // Création et initialisation des agents
    for (int i = 0; i < nb_individus; ++i) {
        // Position aléatoire sur la grille
        std::uniform_int_distribution<int> distX(0, grille.taille_x - 1);
        std::uniform_int_distribution<int> distY(0, grille.taille_y - 1);
        int pos_x = distX(prng_engine);
        int pos_y = distY(prng_engine);

        // Durées individuelles pour les états E, I, R
        double dE_agent = generer_temps_exponentiel(moyenne_dE);
        double dI_agent = generer_temps_exponentiel(moyenne_dI);
        double dR_agent = generer_temps_exponentiel(moyenne_dR);

        // Statut initial
        Statut statut_initial = (i < nb_infectes_init) ? Statut::I : Statut::S;

        liste_agents.emplace_back(i, pos_x, pos_y, dE_agent, dI_agent, dR_agent, statut_initial);
    }

    // Ajout des agents (maintenant qu'ils sont tous créés et leurs adresses stables) à la grille
    for (size_t i = 0; i < liste_agents.size(); ++i) {
        grille.ajouter_agent(&liste_agents[i]);
    }

    // std::cout << "Initialisation (seed " << seed << ") terminée. " << liste_agents.size() << " agents créés." << std::endl;
    collecter_statistiques(); // Collecter les statistiques pour l'itération 0 (état initial)
}

void Simulation::lancer_une_iteration() {
    iteration_actuelle++;

    // Créer un vecteur d'indices pour mélanger l'ordre d'exécution des agents
    std::vector<int> agent_indices(liste_agents.size());
    for(size_t i = 0; i < agent_indices.size(); ++i) {
        agent_indices[i] = i;
    }
    std::shuffle(agent_indices.begin(), agent_indices.end(), prng_engine);

    for (int idx : agent_indices) {
        Agent& agent = liste_agents[idx]; // Travailler avec une référence à l'agent

        int old_pos_x = agent.pos_x;
        int old_pos_y = agent.pos_y;

        // 1. Déplacement de l'agent
        agent.se_deplacer(grille.taille_x, grille.taille_y, prng_engine);
        // Mettre à jour la position de l'agent dans la structure de données de la grille
        grille.mettre_a_jour_position_agent(&agent, old_pos_x, old_pos_y);

        // 2. Incrémenter le temps passé dans le statut actuel
        agent.incrementer_temps_dans_statut();

        // 3. Logique de changement d'état (asynchrone)
        // Le statut est vérifié et potentiellement modifié pour l'agent courant.
        Statut statut_avant_transition = agent.statut;

        if (statut_avant_transition == Statut::S) {
            double val_alea_infection = uniform_dist_01(prng_engine); // Générer la valeur aléatoire ici
            agent.verifier_infection(grille, force_infection, val_alea_infection);
        } else if (statut_avant_transition == Statut::E) {
            agent.mettre_a_jour_statut_exposition();
        } else if (statut_avant_transition == Statut::I) {
            agent.mettre_a_jour_statut_infection();
        } else if (statut_avant_transition == Statut::R) {
            agent.mettre_a_jour_statut_retablissement();
        }
    }
    collecter_statistiques(); // Collecter les stats après que tous les agents ont agi pour cette itération
}

void Simulation::lancer_simulation_complete(unsigned int seed) {
    initialiser_simulation(seed); // Initialise avec la graine
    // std::cout << "Lancement de la simulation complète (seed " << seed << ") pour " << nb_max_iterations << " itérations..." << std::endl;

    for (int i = 0; i < nb_max_iterations; ++i) {
        lancer_une_iteration();
        // Affichage de la progression (peut être coûteux, donc pas à chaque itération)
        if ((iteration_actuelle % 100 == 0 && iteration_actuelle > 0) || iteration_actuelle == 1 ) {
             std::cout << "  Sim (seed " << seed << ") - Itération " << iteration_actuelle << "/" << nb_max_iterations << std::endl;
        }
    }
    // std::cout << "Simulation (seed " << seed << ") terminée après " << iteration_actuelle << " itérations." << std::endl;
}

void Simulation::collecter_statistiques() {
    ResultatIteration res_iter;
    res_iter.iteration = iteration_actuelle; // Itération actuelle (0 pour initial, 1 à nb_max_iterations pour les suivantes)
    res_iter.nb_S = 0;
    res_iter.nb_E = 0;
    res_iter.nb_I = 0;
    res_iter.nb_R = 0;

    for (const auto& agent : liste_agents) { // Itérer sur les agents stockés dans la simulation
        switch (agent.statut) {
            case Statut::S: res_iter.nb_S++; break;
            case Statut::E: res_iter.nb_E++; break;
            case Statut::I: res_iter.nb_I++; break;
            case Statut::R: res_iter.nb_R++; break;
        }
    }
    resultats_par_iteration.push_back(res_iter);

    // Affichage optionnel des statistiques (pour débogage)
    // if (iteration_actuelle % 50 == 0 || iteration_actuelle <= 1 || iteration_actuelle == nb_max_iterations) {
    //     std::cout << "Iter " << iteration_actuelle
    //               << ": S=" << res_iter.nb_S
    //               << ", E=" << res_iter.nb_E
    //               << ", I=" << res_iter.nb_I
    //               << ", R=" << res_iter.nb_R << std::endl;
    // }
}

void Simulation::sauvegarder_resultats_csv(const std::string& nom_fichier) const {
    std::ofstream fichier_sortie(nom_fichier);
    if (!fichier_sortie.is_open()) {
        std::cerr << "Erreur critique : Impossible d'ouvrir le fichier de sortie : " << nom_fichier << std::endl;
        // Peut-être lancer une exception ici ou retourner un code d'erreur
        return;
    }

    // Écrire l'en-tête du CSV
    fichier_sortie << "iteration,S,E,I,R\n";

    // Écrire les données pour chaque itération
    for (const auto& res : resultats_par_iteration) {
        fichier_sortie << res.iteration << ","
                       << res.nb_S << ","
                       << res.nb_E << ","
                       << res.nb_I << ","
                       << res.nb_R << "\n";
    }

    fichier_sortie.close();
    // std::cout << "Résultats de la simulation sauvegardés dans " << nom_fichier << std::endl;
}
