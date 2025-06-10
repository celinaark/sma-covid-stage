import java.util.ArrayList;
import java.util.List;
import java.util.HashMap; // Option pour stocker les cellules si la grille est sparse
                          // Mais pour une grille dense, un tableau 2D de Listes est souvent plus direct.

public class Grille {
    public final int tailleX;
    public final int tailleY;

    // Structure pour stocker les agents sur la grille.
    // Un tableau 2D où chaque élément est une liste d'agents présents sur cette cellule.
    // List<Agent>[][] cellules; // Syntaxe pour un tableau de Listes d'Agents
    // Ou, pour une initialisation plus simple et flexible avec ArrayList:
    private List<Agent>[][] cellules;


    @SuppressWarnings("unchecked") // Pour la création du tableau de listes génériques
    public Grille(int tailleX, int tailleY) {
        if (tailleX <= 0 || tailleY <= 0) {
            throw new IllegalArgumentException("Les dimensions de la grille doivent être positives.");
        }
        this.tailleX = tailleX;
        this.tailleY = tailleY;

        // Initialisation du tableau de listes
        // On ne peut pas créer un tableau de génériques directement comme new List<Agent>[tailleX][tailleY];
        // Il faut passer par un cast depuis un tableau d'Object ou un tableau de List non typée.
        this.cellules = (ArrayList<Agent>[][]) new ArrayList[tailleX][tailleY];
        for (int i = 0; i < tailleX; i++) {
            for (int j = 0; j < tailleY; j++) {
                this.cellules[i][j] = new ArrayList<Agent>();
            }
        }
    }

    /**
     * Réinitialise la grille en vidant toutes les listes d'agents des cellules.
     */
    public void initialiserGrille() {
        for (int i = 0; i < tailleX; i++) {
            for (int j = 0; j < tailleY; j++) {
                this.cellules[i][j].clear();
            }
        }
    }

    /**
     * Ajoute un agent à sa position actuelle sur la grille.
     * @param agent L'agent à ajouter.
     */
    public void ajouterAgent(Agent agent) {
        if (agent == null) return;
        if (agent.posX >= 0 && agent.posX < tailleX && agent.posY >= 0 && agent.posY < tailleY) {
            this.cellules[agent.posX][agent.posY].add(agent);
        } else {
            // Gérer l'erreur: agent hors limites.
            // Pourrait lancer une exception ou afficher un message.
            System.err.println("Tentative d'ajout de l'agent " + agent.agentId + " à des coordonnées hors limites : (" + agent.posX + "," + agent.posY + ")");
        }
    }

    /**
     * Retire un agent de sa position spécifiée (oldX, oldY) sur la grille.
     * @param agent L'agent à retirer.
     * @param oldX Ancienne coordonnée X de l'agent.
     * @param oldY Ancienne coordonnée Y de l'agent.
     */
    public void retirerAgent(Agent agent, int oldX, int oldY) {
        if (agent == null) return;
        if (oldX >= 0 && oldX < tailleX && oldY >= 0 && oldY < tailleY) {
            this.cellules[oldX][oldY].remove(agent); // remove() sur une List utilise equals() pour trouver l'objet.
                                                   // S'assure que Agent a une implémentation correcte de equals() si nécessaire,
                                                   // mais ici on retire l'instance exacte.
        } else {
             System.err.println("Tentative de retrait de l'agent " + agent.agentId + " de coordonnées hors limites : (" + oldX + "," + oldY + ")");
        }
    }

    /**
     * Met à jour la position d'un agent sur la grille.
     * L'agent doit déjà avoir ses nouvelles coordonnées (posX, posY) mises à jour.
     * @param agent L'agent dont la position est mise à jour.
     * @param oldX Ancienne coordonnée X de l'agent.
     * @param oldY Ancienne coordonnée Y de l'agent.
     */
    public void mettreAJourPositionAgent(Agent agent, int oldX, int oldY) {
        retirerAgent(agent, oldX, oldY);
        ajouterAgent(agent); // L'agent a déjà ses nouvelles coordonnées
    }

    /**
     * Retourne la liste des agents présents sur une cellule spécifique.
     * @param x Coordonnée X de la cellule.
     * @param y Coordonnée Y de la cellule.
     * @return Une liste d'agents. Peut être vide si aucun agent n'est sur la cellule.
     */
    public List<Agent> getAgentsSurCellule(int x, int y) {
        // L'appelant est responsable de la gestion de la toroidalité pour les coordonnées x, y
        // avant d'appeler cette méthode si un accès toroïdal est souhaité.
        if (x >= 0 && x < tailleX && y >= 0 && y < tailleY) {
            return this.cellules[x][y];
        } else {
            // Coordonnées hors des limites brutes de la grille
            System.err.println("Accès hors limites dans getAgentsSurCellule pour (" + x + "," + y + "). Retourne une liste vide.");
            return new ArrayList<Agent>(); // Retourne une liste vide pour éviter NullPointerException
        }
    }

    // La méthode get_voisinage_moore n'est pas strictement nécessaire ici si la logique
    // de voisinage est entièrement gérée dans Agent.verifierInfection, comme c'est le cas.
    // Si elle était nécessaire, elle ressemblerait à ceci :
    /*
    public List<Agent> getVoisinageMoore(int xCentre, int yCentre) {
        List<Agent> voisinsComplets = new ArrayList<>();
        if (xCentre < 0 || xCentre >= tailleX || yCentre < 0 || yCentre >= tailleY) {
            System.err.println("Coordonnées centrales hors limites pour getVoisinageMoore.");
            return voisinsComplets;
        }

        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                int voisinX = (xCentre + dx + tailleX) % tailleX;
                int voisinY = (yCentre + dy + tailleY) % tailleY;
                voisinsComplets.addAll(getAgentsSurCellule(voisinX, voisinY));
            }
        }
        return voisinsComplets;
    }
    */
}
