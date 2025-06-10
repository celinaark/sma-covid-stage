// Agent.cpp
#include "Agent.hpp"
#include "Grille.hpp" // Nécessaire pour la méthode verifier_infection et l'accès à grille.taille_x/y
#include <cmath>     // Pour std::exp
#include <vector>    // Utilisé par Grille, et verifier_infection parcourt un vector<Agent*>
#include <iostream>  // Pour d'éventuels messages de débogage (laisser commenté en prod)

// Constructeur
Agent::Agent(int id, int x, int y, double _dE, double _dI, double _dR, Statut _statut)
    : agent_id(id), pos_x(x), pos_y(y), dE(_dE), dI(_dI), dR(_dR), statut(_statut), temps_dans_statut(0) {
    // L'initialisation principale est faite via la liste d'initialisation du constructeur.
}

// Méthode pour convertir le statut enum en string (utile pour l'affichage/débogage)
std::string Agent::statut_to_string() const {
    switch (statut) {
        case Statut::S: return "S";
        case Statut::E: return "E";
        case Statut::I: return "I";
        case Statut::R: return "R";
        default: return "?"; // Cas inconnu
    }
}

void Agent::se_deplacer(int grille_taille_x, int grille_taille_y, std::mt19937& prng_engine) {
    if (grille_taille_x > 0) {
        std::uniform_int_distribution<int> distX(0, grille_taille_x - 1);
        pos_x = distX(prng_engine);
    } else {
        pos_x = 0;
    }

    if (grille_taille_y > 0) {
        std::uniform_int_distribution<int> distY(0, grille_taille_y - 1);
        pos_y = distY(prng_engine);
    } else {
        pos_y = 0;
    }
    // La mise à jour de la position de l'agent DANS la structure de la grille
    // sera gérée par la classe Grille ou Simulation après cet appel.
}

void Agent::verifier_infection(Grille& grille, double force_infection, double random_val_01) {
    if (statut == Statut::S) {
        int infectieux_voisins = 0;

        // Parcourir les voisins de Moore (y compris la cellule actuelle)
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                // Calcul des coordonnées toroïdales
                int voisin_x = (pos_x + dx + grille.taille_x) % grille.taille_x;
                int voisin_y = (pos_y + dy + grille.taille_y) % grille.taille_y;

                // Accéder aux agents sur la cellule voisine
                // Grille::get_agents_sur_cellule retourne une const std::vector<Agent*>&
                const std::vector<Agent*>& agents_sur_cellule = grille.get_agents_sur_cellule(voisin_x, voisin_y);
                for (const Agent* autre_agent : agents_sur_cellule) {
                    if (autre_agent != this && autre_agent->statut == Statut::I) { // S'assurer de ne pas se compter soi-même si on était I (pas le cas ici car statut == S)
                        infectieux_voisins++;
                    }
                }
            }
        }

        if (infectieux_voisins > 0) {
            double probabilite_infection = 1.0 - std::exp(-force_infection * static_cast<double>(infectieux_voisins));
            // random_val_01 est un nombre aléatoire entre [0,1) fourni par la simulation
            if (random_val_01 < probabilite_infection) {
                statut = Statut::E;
                temps_dans_statut = 0;
                // std::cout << "Agent " << agent_id << " (" << pos_x << "," << pos_y << ") est devenu Exposé à cause de " << infectieux_voisins << " voisins infectieux." << std::endl;
            }
        }
    }
}

void Agent::mettre_a_jour_statut_exposition() {
    if (statut == Statut::E) {
        if (temps_dans_statut >= dE) {
            statut = Statut::I;
            temps_dans_statut = 0;
            // std::cout << "Agent " << agent_id << " est devenu Infectieux." << std::endl;
        }
    }
}

void Agent::mettre_a_jour_statut_infection() {
    if (statut == Statut::I) {
        if (temps_dans_statut >= dI) {
            statut = Statut::R;
            temps_dans_statut = 0;
            // std::cout << "Agent " << agent_id << " est devenu Rétabli." << std::endl;
        }
    }
}

void Agent::mettre_a_jour_statut_retablissement() {
    if (statut == Statut::R) {
        if (temps_dans_statut >= dR) {
            statut = Statut::S;
            temps_dans_statut = 0;
            // std::cout << "Agent " << agent_id << " est redevenu Susceptible." << std::endl;
        }
    }
}

void Agent::incrementer_temps_dans_statut() {
    temps_dans_statut++;
}
