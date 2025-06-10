import java.util.Random;
import java.lang.Math; // Pour Math.exp et Math.log

// Enum pour le statut (peut être dans son propre fichier Statut.java si plus complexe ou partagé)
enum Statut {
    S, // Susceptible
    E, // Exposed
    I, // Infected
    R  // Recovered
}

public class Agent {
    public int agentId;
    public int posX;
    public int posY;
    public double dE; // Durée cible de la période d'exposition pour cet agent
    public double dI; // Durée cible de la période infectieuse pour cet agent
    public double dR; // Durée cible de la période d'immunité pour cet agent
    public Statut statut;
    public int tempsDansStatut; // Compteur de jours passés dans le statut actuel

    private Random prng; // Référence au générateur de nombres aléatoires partagé

    public Agent(int id, int x, int y, double dE, double dI, double dR, Statut statutInitial, Random prngInstance) {
        this.agentId = id;
        this.posX = x;
        this.posY = y;
        this.dE = dE;
        this.dI = dI;
        this.dR = dR;
        this.statut = statutInitial;
        this.tempsDansStatut = 0;
        this.prng = prngInstance; // Utiliser l'instance de Random fournie par la Simulation
    }

    /**
     * Déplace l'agent aléatoirement vers une nouvelle cellule sur la grille.
     * La nouvelle position (posX, posY) est choisie au hasard sur toute la grille.
     * @param grilleTailleX Taille X de la grille.
     * @param grilleTailleY Taille Y de la grille.
     */
    public void seDeplacer(int grilleTailleX, int grilleTailleY) {
        if (grilleTailleX > 0) {
            this.posX = prng.nextInt(grilleTailleX); // prng.nextInt(n) -> [0, n-1]
        }
        if (grilleTailleY > 0) {
            this.posY = prng.nextInt(grilleTailleY);
        }
        // La mise à jour de la position de l'agent DANS la structure de la grille
        // sera gérée par la classe Grille ou Simulation après cet appel.
    }

    /**
     * Si l'agent est Susceptible (S), vérifie la présence d'infectieux (I)
     * dans son voisinage de Moore sur la grille.
     * Change le statut à Exposé (E) avec une certaine probabilité.
     * @param grille L'objet Grille pour interroger le voisinage.
     * @param forceInfection Paramètre de force de l'infection.
     */
    public void verifierInfection(Grille grille, double forceInfection) {
        if (this.statut == Statut.S) {
            int infectieuxVoisins = 0;
            // Parcourir les voisins de Moore (y compris la cellule actuelle de l'agent)
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    // Calcul des coordonnées toroïdales pour le voisin
                    int voisinX = (this.posX + dx + grille.tailleX) % grille.tailleX;
                    int voisinY = (this.posY + dy + grille.tailleY) % grille.tailleY;

                    // Récupérer les agents sur la cellule voisine
                    for (Agent autreAgent : grille.getAgentsSurCellule(voisinX, voisinY)) {
                        if (autreAgent.statut == Statut.I) {
                            infectieuxVoisins++;
                        }
                    }
                }
            }

            if (infectieuxVoisins > 0) {
                double probabiliteInfection = 1.0 - Math.exp(-forceInfection * infectieuxVoisins);
                // prng.nextDouble() génère un double dans l'intervalle [0.0, 1.0)
                if (prng.nextDouble() < probabiliteInfection) {
                    this.statut = Statut.E;
                    this.tempsDansStatut = 0; // Réinitialiser le compteur pour le nouvel état
                    // System.out.println("Agent " + agentId + " est devenu Exposé.");
                }
            }
        }
    }

    /**
     * Si l'agent est Exposé (E) et a dépassé sa durée dE, il devient Infectieux (I).
     */
    public void mettreAJourStatutExposition() {
        if (this.statut == Statut.E && this.tempsDansStatut >= this.dE) {
            this.statut = Statut.I;
            this.tempsDansStatut = 0;
            // System.out.println("Agent " + agentId + " est devenu Infectieux.");
        }
    }

    /**
     * Si l'agent est Infectieux (I) et a dépassé sa durée dI, il devient Rétabli (R).
     */
    public void mettreAJourStatutInfection() {
        if (this.statut == Statut.I && this.tempsDansStatut >= this.dI) {
            this.statut = Statut.R;
            this.tempsDansStatut = 0;
            // System.out.println("Agent " + agentId + " est devenu Rétabli.");
        }
    }

    /**
     * Si l'agent est Rétabli (R) et a dépassé sa durée dR, il redevient Susceptible (S).
     */
    public void mettreAJourStatutRetablissement() {
        if (this.statut == Statut.R && this.tempsDansStatut >= this.dR) {
            this.statut = Statut.S;
            this.tempsDansStatut = 0;
            // System.out.println("Agent " + agentId + " est redevenu Susceptible.");
        }
    }

    /**
     * Incrémente le compteur de jours passés dans le statut actuel.
     */
    public void incrementerTempsDansStatut() {
        this.tempsDansStatut++;
    }

    @Override
    public String toString() {
        return "Agent " + agentId +
               ": Statut=" + statut +
               ", Pos=(" + posX + "," + posY + ")" +
               ", TempsInStatut=" + tempsDansStatut +
               ", dE=" + String.format("%.2f", dE) +
               ", dI=" + String.format("%.2f", dI) +
               ", dR=" + String.format("%.2f", dR);
    }
}
