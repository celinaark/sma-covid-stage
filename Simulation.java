import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Random;
import java.io.FileWriter;
import java.io.IOException;
import java.lang.Math; // Pour Math.log, Math.exp

// Structure simple pour stocker les résultats d'une itération.
// En Java, on ferait souvent une petite classe ou un record (Java 14+).
class ResultatIteration {
    public int iteration;
    public int nbS;
    public int nbE;
    public int nbI;
    public int nbR;

    public ResultatIteration(int iteration, int nbS, int nbE, int nbI, int nbR) {
        this.iteration = iteration;
        this.nbS = nbS;
        this.nbE = nbE;
        this.nbI = nbI;
        this.nbR = nbR;
    }

    public String toCsvString() {
        return iteration + "," + nbS + "," + nbE + "," + nbI + "," + nbR;
    }
}

public class Simulation {
    public int nbIndividus;
    public Grille grille;
    public List<Agent> listeAgents;

    public int nbInfectesInit;
    public double moyenneDE;
    public double moyenneDI;
    public double moyenneDR;
    public double forceInfection;
    public int nbMaxIterations;
    public int iterationActuelle;

    public List<ResultatIteration> resultatsParIteration;
    private Random prng; // Générateur de nombres aléatoires

    public Simulation(int nbIndividus, int tailleGrilleX, int tailleGrilleY,
                      int nbInfectesInit, double moyenneDE, double moyenneDI, double moyenneDR,
                      double forceInfection, int nbMaxIterations) {

        if (nbIndividus <= 0 || nbInfectesInit < 0 || nbInfectesInit > nbIndividus) {
            throw new IllegalArgumentException("Nombre d'individus ou d'infectés initiaux invalide.");
        }
        if (moyenneDE <= 0 || moyenneDI <= 0 || moyenneDR <= 0) {
            throw new IllegalArgumentException("Les durées moyennes dE, dI, dR doivent être positives.");
        }

        this.nbIndividus = nbIndividus;
        this.grille = new Grille(tailleGrilleX, tailleGrilleY);
        this.listeAgents = new ArrayList<>(nbIndividus); // Pré-allouer avec la capacité
        this.resultatsParIteration = new ArrayList<>(nbMaxIterations + 1);

        this.nbInfectesInit = nbInfectesInit;
        this.moyenneDE = moyenneDE;
        this.moyenneDI = moyenneDI;
        this.moyenneDR = moyenneDR;
        this.forceInfection = forceInfection;
        this.nbMaxIterations = nbMaxIterations;
        this.iterationActuelle = 0;

        // Le PRNG sera initialisé avec une graine dans initialiserSimulation
        this.prng = new Random();
    }

    private double genererTempsExponentiel(double moyenne) {
        // Assure que la moyenne est positive (déjà vérifié dans le constructeur)
        // prng.nextDouble() retourne une valeur dans [0.0, 1.0)
        double u = prng.nextDouble();
        // Pour éviter log(0) si u est 0.0 (ce qui donnerait une durée infinie, ou négative avec -moyenne)
        // ou si u est très proche de 1.0 (ce qui donnerait log(epsilon_tres_petit) -> durée très grande)
        // Si u = 0, log(1-0) = log(1) = 0. Durée = 0.
        // Si u -> 1, log(1-u) -> log(epsilon_positif) -> -infini. Durée -> +infini.
        if (u == 0.0) { // Pour éviter une durée de 0 si cela n'est pas souhaité (une transition instantanée)
            // On peut relancer le tirage ou retourner une petite valeur non nulle.
            // Pour simplifier, on accepte une durée de 0 si u=0.
            // Si on veut absolument éviter 0, on peut faire : while (u == 0.0) { u = prng.nextDouble(); }
        }
         if (u == 1.0) { // Pour éviter log(0)
            return Double.POSITIVE_INFINITY; // Ou une très grande valeur si l'infini n'est pas gérable
        }
        return -moyenne * Math.log(1.0 - u);
    }

    public void initialiserSimulation(long seed) {
        this.prng.setSeed(seed);
        this.iterationActuelle = 0;

        this.listeAgents.clear();
        this.grille.initialiserGrille(); // Vide les listes d'agents dans chaque cellule
        this.resultatsParIteration.clear();

        for (int i = 0; i < nbIndividus; i++) {
            int posX = prng.nextInt(grille.tailleX);
            int posY = prng.nextInt(grille.tailleY);

            double dEAgent = genererTempsExponentiel(moyenneDE);
            double dIAgent = genererTempsExponentiel(moyenneDI);
            double dRAgent = genererTempsExponentiel(moyenneDR);

            Statut statutInitial = (i < nbInfectesInit) ? Statut.I : Statut.S;

            Agent agent = new Agent(i, posX, posY, dEAgent, dIAgent, dRAgent, statutInitial, this.prng);
            this.listeAgents.add(agent);
            this.grille.ajouterAgent(agent); // L'agent est ajouté à sa position initiale sur la grille
        }
        // System.out.println("Initialisation (seed " + seed + ") terminée. " + this.listeAgents.size() + " agents créés.");
        collecterStatistiques(); // Pour l'itération 0
    }

    public void lancerUneIteration() {
        this.iterationActuelle++;

        // Ordre d'exécution aléatoire des agents
        Collections.shuffle(this.listeAgents, this.prng);

        for (Agent agent : this.listeAgents) {
            int oldPosX = agent.posX;
            int oldPosY = agent.posY;

            agent.seDeplacer(grille.tailleX, grille.tailleY);
            grille.mettreAJourPositionAgent(agent, oldPosX, oldPosY);

            agent.incrementerTempsDansStatut();

            Statut statutAvantTransition = agent.statut;

            if (statutAvantTransition == Statut.S) {
                agent.verifierInfection(grille, forceInfection);
            } else if (statutAvantTransition == Statut.E) {
                agent.mettreAJourStatutExposition();
            } else if (statutAvantTransition == Statut.I) {
                agent.mettreAJourStatutInfection();
            } else if (statutAvantTransition == Statut.R) {
                agent.mettreAJourStatutRetablissement();
            }
        }
        collecterStatistiques();
    }

    public void lancerSimulationComplete(long seed) {
        initialiserSimulation(seed);
        // System.out.println("Lancement de la simulation complète (seed " + seed + ") pour " + nbMaxIterations + " itérations...");
        for (int i = 0; i < nbMaxIterations; i++) {
            lancerUneIteration();
            if ((iterationActuelle % 100 == 0 && iterationActuelle > 0) || iterationActuelle == 1) {
                 System.out.println("  Sim (seed " + seed + ") - Itération " + iterationActuelle + "/" + nbMaxIterations);
            }
        }
        // System.out.println("Simulation (seed " + seed + ") terminée après " + iterationActuelle + " itérations.");
    }

    public void collecterStatistiques() {
        int s = 0, e = 0, inf = 0, r = 0; // 'inf' pour 'infectious' pour éviter conflit avec 'i' d'itération
        for (Agent agent : this.listeAgents) {
            switch (agent.statut) {
                case S: s++; break;
                case E: e++; break;
                case I: inf++; break;
                case R: r++; break;
            }
        }
        resultatsParIteration.add(new ResultatIteration(iterationActuelle, s, e, inf, r));
        // if (iterationActuelle % 50 == 0 || iterationActuelle <=1 || iterationActuelle == nbMaxIterations) {
        //     System.out.println("Iter " + iterationActuelle + ": S=" + s + ", E=" + e + ", I=" + inf + ", R=" + r);
        // }
    }

    public void sauvegarderResultatsCsv(String nomFichier) {
        try (FileWriter writer = new FileWriter(nomFichier)) {
            writer.append("iteration,S,E,I,R\n");
            for (ResultatIteration res : resultatsParIteration) {
                writer.append(res.toCsvString()).append("\n");
            }
            // System.out.println("Résultats sauvegardés dans " + nomFichier);
        } catch (IOException ex) {
            System.err.println("Erreur lors de la sauvegarde des résultats dans " + nomFichier + ": " + ex.getMessage());
        }
    }
}
