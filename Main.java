import java.time.Duration;
import java.time.Instant;

public class Main {

    public static void main(String[] args) {
        // Paramètres de la simulation (selon les spécifications du professeur)
        final int NB_INDIVIDUS = 20000;
        final int TAILLE_GRILLE_X = 300;
        final int TAILLE_GRILLE_Y = 300;
        final int NB_INFECTES_INIT = 20;

        final double MOYENNE_DE = 3.0;  // jours
        final double MOYENNE_DI = 7.0;  // jours
        final double MOYENNE_DR = 365.0;// jours (longue immunité)

        final double FORCE_INFECTION = 0.5;
        final int NB_MAX_ITERATIONS = 730; // Nombre de jours pour la simulation (ex: 2 ans)

        final int NOMBRE_REPLICATIONS = 100; // Comme demandé pour l'analyse statistique

        System.out.println("Lancement des simulations SEIR multi-agents en Java...");
        System.out.println("----------------------------------------------------");
        System.out.println("Paramètres généraux :");
        System.out.println("  Nombre d'individus       : " + NB_INDIVIDUS);
        System.out.println("  Taille de la grille      : " + TAILLE_GRILLE_X + "x" + TAILLE_GRILLE_Y);
        System.out.println("  Infectés initiaux        : " + NB_INFECTES_INIT);
        System.out.println("  Moyenne dE (jours)       : " + MOYENNE_DE);
        System.out.println("  Moyenne dI (jours)       : " + MOYENNE_DI);
        System.out.println("  Moyenne dR (jours)       : " + MOYENNE_DR);
        System.out.println("  Force d'infection        : " + FORCE_INFECTION);
        System.out.println("  Itérations par simulation : " + NB_MAX_ITERATIONS);
        System.out.println("  Nombre de réplications   : " + NOMBRE_REPLICATIONS);
        System.out.println("----------------------------------------------------");

        Instant debutTotalSimulations = Instant.now();

        try {
            for (int i = 0; i < NOMBRE_REPLICATIONS; i++) {
                // Utiliser l'index de la boucle comme graine est simple et garantit des graines différentes.
                long seed = (long)i;

                System.out.println("\nLancement de la Réplication " + (i + 1) + "/" + NOMBRE_REPLICATIONS
                                  + " avec graine (seed) = " + seed);

                Simulation simulationActuelle = new Simulation(
                    NB_INDIVIDUS, TAILLE_GRILLE_X, TAILLE_GRILLE_Y,
                    NB_INFECTES_INIT, MOYENNE_DE, MOYENNE_DI, MOYENNE_DR,
                    FORCE_INFECTION, NB_MAX_ITERATIONS
                );

                Instant debutReplication = Instant.now();

                simulationActuelle.lancerSimulationComplete(seed); // Lance la simulation

                Instant finReplication = Instant.now();
                Duration dureeReplication = Duration.between(debutReplication, finReplication);
                System.out.println("Réplication " + (i + 1) + " terminée en " + dureeReplication.toMillis() / 1000.0 + " secondes.");

                // Construction du nom de fichier pour les résultats de cette réplication
                String nomFichierCsv = "toy_model_java_repl_" + (i + 1)
                                     + "_seed" + seed + ".csv";
                simulationActuelle.sauvegarderResultatsCsv(nomFichierCsv);
                System.out.println("Résultats de la réplication " + (i + 1) + " sauvegardés dans " + nomFichierCsv);
            }
        } catch (IllegalArgumentException e) {
            System.err.println("Erreur de configuration de la simulation: " + e.getMessage());
            // Terminer ou gérer l'erreur comme il se doit
            return;
        } catch (Exception e) {
            System.err.println("Une erreur majeure est survenue pendant les simulations : " + e.getMessage());
            e.printStackTrace();
            return;
        }

        Instant finTotalSimulations = Instant.now();
        Duration dureeTotaleSimulations = Duration.between(debutTotalSimulations, finTotalSimulations);

        System.out.println("\n----------------------------------------------------");
        System.out.println("Toutes les " + NOMBRE_REPLICATIONS + " réplications sont terminées.");
        System.out.println("Temps total d'exécution pour toutes les réplications : "
                          + dureeTotaleSimulations.toMillis() / 1000.0 + " secondes.");
        if (NOMBRE_REPLICATIONS > 0) {
            System.out.println("Temps moyen par réplication : "
                              + (dureeTotaleSimulations.toMillis() / 1000.0) / NOMBRE_REPLICATIONS + " secondes.");
        }
        System.out.println("Les fichiers CSV des résultats ont été générés.");
        System.out.println("L'étape suivante serait d'utiliser un Jupyter Notebook pour analyser ces fichiers.");
        System.out.println("----------------------------------------------------");
    }
}

// Pour compiler ce projet (exemple avec javac) :
// 1. Assurez-vous que tous les fichiers .java (Main.java, Agent.java, Grille.java, Simulation.java, Statut.java si séparé)
//    sont dans le même dossier.
// 2. Ouvrez un terminal dans ce dossier.
// 3. Compilez : javac Main.java Agent.java Grille.java Simulation.java
//    (Si Statut est dans son propre fichier, ajoutez Statut.java à la commande)
//    (Si vous avez mis Statut en tant que nested enum ou inner enum dans Agent.java, alors Agent.java suffit pour le compiler)
// 4. Exécutez : java Main
