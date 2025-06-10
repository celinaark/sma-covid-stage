import random
import math

class Agent:
    def __init__(self, agent_id, pos_x, pos_y, dE, dI, dR, statut="S"):
        self.agent_id = agent_id  # Un identifiant unique pour l'agent
        self.pos_x = pos_x
        self.pos_y = pos_y
        self.dE = dE  # Durée de la période d'exposition pour cet agent
        self.dI = dI  # Durée de la période infectieuse pour cet agent
        self.dR = dR  # Durée de la période d'immunité pour cet agent
        self.statut = statut  # S, E, I, ou R
        self.temps_dans_statut = 0

    def __str__(self):
        return f"Agent {self.agent_id}: Statut={self.statut}, Pos=({self.pos_x},{self.pos_y}), TempsInStatut={self.temps_dans_statut}"

    def se_deplacer(self, grille_taille_x, grille_taille_y):
        """
        Déplace l'agent aléatoirement vers une nouvelle cellule sur la grille.
        La nouvelle position est choisie au hasard sur toute la grille.
        """
        self.pos_x = random.randrange(grille_taille_x)
        self.pos_y = random.randrange(grille_taille_y)
        # La grille devra être mise à jour pour refléter ce changement

    def verifier_infection(self, grille, force_infection):
        """
        Si l'agent est Susceptible (S), vérifie la présence d'infectieux (I)
        dans son voisinage de Moore sur la grille.
        Change le statut à Exposé (E) avec une certaine probabilité.
        """
        if self.statut == "S":
            infectieux_voisins = 0
            # Parcourir les voisins de Moore (y compris la cellule actuelle)
            for dx in [-1, 0, 1]:
                for dy in [-1, 0, 1]:
                    # Calcul des coordonnées toroïdales
                    # Assurer que grille.taille_x et grille.taille_y sont accessibles et corrects
                    voisin_x = (self.pos_x + dx) % grille.taille_x
                    voisin_y = (self.pos_y + dy) % grille.taille_y

                    agents_sur_cellule = grille.get_agents_sur_cellule(voisin_x, voisin_y)
                    for autre_agent in agents_sur_cellule:
                        if autre_agent.statut == "I":
                            infectieux_voisins += 1

            if infectieux_voisins > 0:
                probabilite_infection = 1 - math.exp(-force_infection * infectieux_voisins)
                if random.random() < probabilite_infection:
                    self.statut = "E"
                    self.temps_dans_statut = 0
                    # print(f"Agent {self.agent_id} est devenu Exposé")


    def mettre_a_jour_statut_exposition(self):
        """
        Si l'agent est Exposé (E) et a dépassé dE, il devient Infectieux (I).
        """
        if self.statut == "E":
            if self.temps_dans_statut >= self.dE:
                self.statut = "I"
                self.temps_dans_statut = 0
                # print(f"Agent {self.agent_id} est devenu Infectieux")

    def mettre_a_jour_statut_infection(self):
        """
        Si l'agent est Infectieux (I) et a dépassé dI, il devient Rétabli (R).
        """
        if self.statut == "I":
            if self.temps_dans_statut >= self.dI:
                self.statut = "R"
                self.temps_dans_statut = 0
                # print(f"Agent {self.agent_id} est devenu Rétabli")

    def mettre_a_jour_statut_retablissement(self):
        """
        Si l'agent est Rétabli (R) et a dépassé dR, il redevient Susceptible (S).
        """
        if self.statut == "R":
            if self.temps_dans_statut >= self.dR:
                self.statut = "S"
                self.temps_dans_statut = 0
                # print(f"Agent {self.agent_id} est redevenu Susceptible")

    def incrementer_temps_dans_statut(self):
        self.temps_dans_statut += 1

class Grille:
    def __init__(self, taille_x, taille_y):
        self.taille_x = taille_x
        self.taille_y = taille_y
        # Une façon de stocker les agents : un dictionnaire où les clés sont des tuples (x,y)
        # et les valeurs sont des listes d'agents sur cette cellule.
        self.cellules = {} # {(x,y): [agent1, agent2], ...}

    def ajouter_agent(self, agent):
        """Ajoute un agent à sa position sur la grille."""
        pos = (agent.pos_x, agent.pos_y)
        if pos not in self.cellules:
            self.cellules[pos] = []
        self.cellules[pos].append(agent)

    def retirer_agent(self, agent, ancienne_pos_tuple):
        """Retire un agent de son ancienne position sur la grille."""
        # ancienne_pos = (agent.pos_x, agent.pos_y) # L'agent a déjà sa nouvelle position au moment de l'appel
        if ancienne_pos_tuple in self.cellules and agent in self.cellules[ancienne_pos_tuple]:
            self.cellules[ancienne_pos_tuple].remove(agent)
            if not self.cellules[ancienne_pos_tuple]: # Si la liste est vide
                del self.cellules[ancienne_pos_tuple]

    def mettre_a_jour_position_agent(self, agent, old_pos_x, old_pos_y):
        """Met à jour la position d'un agent sur la grille après un déplacement."""
        ancienne_pos_tuple = (old_pos_x, old_pos_y)
        self.retirer_agent(agent, ancienne_pos_tuple)

        nouvelle_pos_tuple = (agent.pos_x, agent.pos_y)
        self.ajouter_agent_a_position(agent, nouvelle_pos_tuple)

    def ajouter_agent_a_position(self, agent, pos_tuple):
        """Ajoute un agent à une position spécifique sur la grille."""
        if pos_tuple not in self.cellules:
            self.cellules[pos_tuple] = []
        self.cellules[pos_tuple].append(agent)


    def get_agents_sur_cellule(self, x, y):
        """Retourne la liste des agents sur la cellule (x,y)."""
        return self.cellules.get((x,y), [])

class Simulation:
    def __init__(self, nb_individus, taille_grille_x, taille_grille_y,
                 nb_infectes_init, params_duree_expo, params_duree_inf, params_duree_rec,
                 force_infection=0.5, nb_iterations=730):
        self.nb_individus = nb_individus
        self.taille_grille_x = taille_grille_x
        self.taille_grille_y = taille_grille_y
        self.grille = Grille(taille_grille_x, taille_grille_y)
        self.liste_agents = []
        self.nb_infectes_init = nb_infectes_init
        self.params_duree_expo = params_duree_expo # ex: 3
        self.params_duree_inf = params_duree_inf # ex: 7
        self.params_duree_rec = params_duree_rec # ex: 365
        self.force_infection = force_infection
        self.nb_iterations = nb_iterations
        self.iteration_actuelle = 0
        self.resultats_par_iteration = []

    def _neg_exp(self, moyenne):
        """Génère un nombre selon une distribution exponentielle négative."""
        if moyenne <= 0:
            # Retourner une très grande valeur ou gérer l'erreur comme il se doit
            # Pour dE, dI, dR, une moyenne de 0 ou négative n'a pas de sens.
            # On pourrait retourner float('inf') ou lever une exception.
            # Pour simplifier, si c'est 0, on peut imaginer un changement instantané (non réaliste ici)
            # ou une durée très longue. Pour les specs: 3, 7, 365 sont > 0.
            raise ValueError("La moyenne pour la distribution exponentielle doit être positive.")
        # random.random() retourne une valeur dans [0.0, 1.0).
        # Pour éviter log(0), on s'assure que l'argument de log est > 0.
        # 1.0 - random.random() est dans (0.0, 1.0].
        val_alea = random.random()
        while val_alea == 1.0: # Pour éviter math.log(0) si random.random() retourne 1.0 (très improbable mais possible)
            val_alea = random.random()
        return -moyenne * math.log(1.0 - val_alea)

    def initialiser_simulation(self, seed=None):
        """Initialise la simulation : crée les agents, les place, définit les statuts initiaux."""
        if seed is not None:
            random.seed(seed)

        self.liste_agents = []
        self.grille = Grille(self.taille_grille_x, self.taille_grille_y)
        self.resultats_par_iteration = []
        self.iteration_actuelle = 0

        for i in range(self.nb_individus):
            pos_x = random.randrange(self.taille_grille_x)
            pos_y = random.randrange(self.taille_grille_y)

            dE = self._neg_exp(self.params_duree_expo)
            dI = self._neg_exp(self.params_duree_inf)
            dR = self._neg_exp(self.params_duree_rec)

            agent = Agent(agent_id=i, pos_x=pos_x, pos_y=pos_y, dE=dE, dI=dI, dR=dR)

            if i < self.nb_infectes_init:
                agent.statut = "I"
            else:
                agent.statut = "S"

            self.liste_agents.append(agent)
            self.grille.ajouter_agent_a_position(agent, (pos_x, pos_y))

        # print(f"Initialisation terminée. {len(self.liste_agents)} agents créés.")
        self._collecter_et_sauvegarder_stats(self.iteration_actuelle)


    def lancer_une_iteration(self):
        """Lance une itération (un pas de temps = un jour) de la simulation."""
        self.iteration_actuelle += 1

        agents_a_traiter = list(self.liste_agents)
        random.shuffle(agents_a_traiter)

        for agent in agents_a_traiter:
            ancienne_pos_x, ancienne_pos_y = agent.pos_x, agent.pos_y

            agent.se_deplacer(self.taille_grille_x, self.taille_grille_y)
            self.grille.mettre_a_jour_position_agent(agent, ancienne_pos_x, ancienne_pos_y)

            agent.incrementer_temps_dans_statut()

            current_statut = agent.statut

            if current_statut == "S":
                agent.verifier_infection(self.grille, self.force_infection)
            elif current_statut == "E":
                agent.mettre_a_jour_statut_exposition()
            elif current_statut == "I":
                agent.mettre_a_jour_statut_infection()
            elif current_statut == "R":
                agent.mettre_a_jour_statut_retablissement()

        self._collecter_et_sauvegarder_stats(self.iteration_actuelle)

    def _collecter_et_sauvegarder_stats(self, iteration_num_pour_enregistrement):
        """Collecte les statistiques (nombre de S, E, I, R) et les stocke."""
        comptes = {"S": 0, "E": 0, "I": 0, "R": 0}
        for agent in self.liste_agents:
            comptes[agent.statut] += 1

        self.resultats_par_iteration.append({
            "iteration": iteration_num_pour_enregistrement,
            "S": comptes["S"],
            "E": comptes["E"],
            "I": comptes["I"],
            "R": comptes["R"]
        })
        # Décommenter pour un suivi détaillé pendant le développement
        # if iteration_num_pour_enregistrement % 10 == 0 or iteration_num_pour_enregistrement < 5 or iteration_num_pour_enregistrement == self.nb_iterations:
        #     print(f"Stats Itération {iteration_num_pour_enregistrement}: S={comptes['S']}, E={comptes['E']}, I={comptes['I']}, R={comptes['R']}")


    def lancer_simulation_complete(self, seed=None):
        """Lance la simulation complète pour le nombre d'itérations défini."""
        self.initialiser_simulation(seed)
        print(f"Lancement de la simulation pour {self.nb_iterations} itérations avec seed={seed}...")
        for i in range(self.nb_iterations):
            if (i+1) % 100 == 0: # Log tous les 100 pas de temps
                 print(f"  Itération {i+1}/{self.nb_iterations}...")
            self.lancer_une_iteration()
        print("Simulation terminée.")
        return self.resultats_par_iteration

    def sauvegarder_resultats_csv(self, nom_fichier):
        """Sauvegarde les résultats de la simulation dans un fichier CSV."""
        if not self.resultats_par_iteration:
            print("Aucun résultat à sauvegarder.")
            return

        with open(nom_fichier, 'w') as f:
            f.write("iteration,S,E,I,R\n")
            for res in self.resultats_par_iteration:
                f.write(f"{res['iteration']},{res['S']},{res['E']},{res['I']},{res['R']}\n")
        print(f"Résultats sauvegardés dans {nom_fichier}")

# --- Exemple d'utilisation ---
if __name__ == "__main__":
    NB_INDIVIDUS = 20000
    TAILLE_GRILLE_X = 300
    TAILLE_GRILLE_Y = 300
    NB_INFECTES_INIT = 20

    PARAM_DE = 3.0
    PARAM_DI = 7.0
    PARAM_DR = 365.0

    FORCE_INFECTION = 0.5
    NB_ITERATIONS = 730

    # # Pour une seule réplication de test rapide
    # print("Lancement d'une simulation unique pour test (paramètres réduits)...")
    # sim_test = Simulation(
    #     nb_individus=200, # NB_INDIVIDUS,
    #     taille_grille_x=30, # TAILLE_GRILLE_X,
    #     taille_grille_y=30, # TAILLE_GRILLE_Y,
    #     nb_infectes_init=2, # NB_INFECTES_INIT,
    #     params_duree_expo=PARAM_DE,
    #     params_duree_inf=PARAM_DI,
    #     params_duree_rec=PARAM_DR,
    #     force_infection=FORCE_INFECTION,
    #     nb_iterations=73 # NB_ITERATIONS
    # )
    # resultats_test = sim_test.lancer_simulation_complete(seed=42)
    # sim_test.sauvegarder_resultats_csv(nom_fichier="resultats_simulation_test_toy_model.csv")
    # print(f"Simulation de test terminée. Résultats dans resultats_simulation_test_toy_model.csv")

    # Boucle pour les 100 réplications
    print("\nLancement des 100 réplications...")
    for i in range(100):
        print(f"  Réplication {i+1}/100 (Seed: {i})...")
        sim_repl = Simulation(
            nb_individus=NB_INDIVIDUS,
            taille_grille_x=TAILLE_GRILLE_X,
            taille_grille_y=TAILLE_GRILLE_Y,
            nb_infectes_init=NB_INFECTES_INIT,
            params_duree_expo=PARAM_DE,
            params_duree_inf=PARAM_DI,
            params_duree_rec=PARAM_DR,
            force_infection=FORCE_INFECTION,
            nb_iterations=NB_ITERATIONS
        )
        sim_repl.lancer_simulation_complete(seed=i)
        nom_fichier_repl = f"toy_model_repl_{i+1}_seed{i}.csv"
        sim_repl.sauvegarder_resultats_csv(nom_fichier=nom_fichier_repl)
        print(f"  Réplication {i+1} terminée. Résultats dans {nom_fichier_repl}")

    print("\nLes 100 réplications sont terminées.")
    print("L'étape suivante serait d'utiliser un Jupyter Notebook pour analyser ces fichiers CSV.")
