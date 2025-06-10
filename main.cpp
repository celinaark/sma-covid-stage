// main.cpp
#include "Simulation.hpp" // Inclut Simulation.hpp, qui inclut Agent.hpp et Grille.hpp
#include <iostream>      // Pour std::cout, std::endl
#include <string>        // Pour std::string, std::to_string
#include <vector>        // Bien que non directement utilisé ici, souvent utile dans les main
#include <chrono>        // Pour mesurer le temps d'exécution (optionnel mais recommandé)
#include <stdexcept>     // Pour std::exception (si les classes en lancent)

int main() {
    // Paramètres de la simulation (selon les spécifications du professeur)
    // Ces valeurs pourraient aussi être lues depuis un fichier de configuration ou des arguments de ligne de commande.
    const int NB_INDIVIDUS = 20000;
    const int TAILLE_GRILLE_X = 300;
    const int TAILLE_GRILLE_Y = 300;
    const int NB_INFECTES_INIT = 20;

    const double MOYENNE_DE = 3.0;  // jours
    const double MOYENNE_DI = 7.0;  // jours
    const double MOYENNE_DR = 365.0;// jours (longue immunité)

    const double FORCE_INFECTION = 0.5; // Paramètre p = 1 - exp(-force_infection * Ni)
    const int NB_MAX_ITERATIONS = 730; // Nombre de jours pour la simulation (ex: 2 ans)

    const int NOMBRE_REPLICATIONS = 100; // Comme demandé pour l'analyse statistique

    std::cout << "Lancement des simulations SEIR multi-agents en C++..." << std::endl;
    std::cout << "----------------------------------------------------" << std::endl;
    std::cout << "Paramètres généraux :" << std::endl;
    std::cout << "  Nombre d'individus       : " << NB_INDIVIDUS << std::endl;
    std::cout << "  Taille de la grille      : " << TAILLE_GRILLE_X << "x" << TAILLE_GRILLE_Y << std::endl;
    std::cout << "  Infectés initiaux        : " << NB_INFECTES_INIT << std::endl;
    std::cout << "  Moyenne dE (jours)       : " << MOYENNE_DE << std::endl;
    std::cout << "  Moyenne dI (jours)       : " << MOYENNE_DI << std::endl;
    std::cout << "  Moyenne dR (jours)       : " << MOYENNE_DR << std::endl;
    std::cout << "  Force d'infection        : " << FORCE_INFECTION << std::endl;
    std::cout << "  Itérations par simulation : " << NB_MAX_ITERATIONS << std::endl;
    std::cout << "  Nombre de réplications   : " << NOMBRE_REPLICATIONS << std::endl;
    std::cout << "----------------------------------------------------" << std::endl;

    auto temps_debut_total_simulations = std::chrono::high_resolution_clock::now();

    try {
        for (int i = 0; i < NOMBRE_REPLICATIONS; ++i) {
            // Utiliser l'index de la boucle comme graine est simple et garantit des graines différentes
            // pour chaque réplication, rendant chaque simulation unique mais reproductible si la même
            // séquence de graines est utilisée.
            unsigned int seed = static_cast<unsigned int>(i);

            std::cout << "\nLancement de la Réplication " << (i + 1) << "/" << NOMBRE_REPLICATIONS
                      << " avec graine (seed) = " << seed << std::endl;

            // Création d'un nouvel objet Simulation pour chaque réplication
            // pour s'assurer que chaque simulation part d'un état initial propre.
            Simulation simulation_actuelle(
                NB_INDIVIDUS, TAILLE_GRILLE_X, TAILLE_GRILLE_Y,
                NB_INFECTES_INIT, MOYENNE_DE, MOYENNE_DI, MOYENNE_DR,
                FORCE_INFECTION, NB_MAX_ITERATIONS
            );

            auto temps_debut_replication = std::chrono::high_resolution_clock::now();

            simulation_actuelle.lancer_simulation_complete(seed); // Lance la simulation

            auto temps_fin_replication = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duree_replication = temps_fin_replication - temps_debut_replication;
            std::cout << "Réplication " << (i + 1) << " terminée en " << duree_replication.count() << " secondes." << std::endl;

            // Construction du nom de fichier pour les résultats de cette réplication
            std::string nom_fichier_csv = "toy_model_cpp_repl_" + std::to_string(i + 1)
                                        + "_seed" + std::to_string(seed) + ".csv";
            simulation_actuelle.sauvegarder_resultats_csv(nom_fichier_csv);
        }
    } catch (const std::exception& e) {
        std::cerr << "Une erreur majeure est survenue pendant les simulations : " << e.what() << std::endl;
        return 1; // Quitter avec un code d'erreur
    } catch (...) {
        std::cerr << "Une erreur inconnue majeure est survenue pendant les simulations." << std::endl;
        return 2; // Quitter avec un autre code d'erreur
    }


    auto temps_fin_total_simulations = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duree_totale_simulations = temps_fin_total_simulations - temps_debut_total_simulations;

    std::cout << "\n----------------------------------------------------" << std::endl;
    std::cout << "Toutes les " << NOMBRE_REPLICATIONS << " réplications sont terminées." << std::endl;
    std::cout << "Temps total d'exécution pour toutes les réplications : "
              << duree_totale_simulations.count() << " secondes." << std::endl;
    if (NOMBRE_REPLICATIONS > 0) {
        std::cout << "Temps moyen par réplication : "
                  << duree_totale_simulations.count() / NOMBRE_REPLICATIONS << " secondes." << std::endl;
    }
    std::cout << "Les fichiers CSV des résultats ont été générés." << std::endl;
    std::cout << "L'étape suivante serait d'utiliser un Jupyter Notebook pour analyser ces fichiers." << std::endl;
    std::cout << "----------------------------------------------------" << std::endl;

    return 0; // Succès
}

// Pour compiler ce projet (exemple avec g++ et C++11 ou plus récent) :
// g++ -std=c++11 main.cpp Agent.cpp Grille.cpp Simulation.cpp -o seir_simulation_cpp
// ou avec une version plus récente de C++ :
// g++ -std=c++17 main.cpp Agent.cpp Grille.cpp Simulation.cpp -o seir_simulation_cpp
// Assurez-vous que tous les fichiers .cpp et .hpp sont dans le même dossier,
// ou ajustez les chemins d'inclusion et la commande de compilation en conséquence.
