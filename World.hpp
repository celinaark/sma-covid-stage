#ifndef _SIMU_EPIDEMIO_WORLD_
#define _SIMU_EPIDEMIO_WORLD_

#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <deque>

// #include <typeinfo> // Probablement pas nécessaire ici
#include <stdlib.h> // Pour malloc/free. Préférer les conteneurs STL et new/delete en C++ moderne.
#include <math.h>   // Pour pow, sqrt etc.
#include <iostream> // Pour std::cout, std::cerr dans le .cpp

#include "RandMT.hpp"
#include "Human.hpp"            // World contient des Human
#include "SimulationParams.hpp" // World utilise SimulationParams
// #include "City.hpp" // City.hpp est inclus par Human.hpp si Human a un City*

// using namespace std; // Éviter dans les .hpp

extern RandMT * randmt; // PRNG Global existant

class City; // Forward declaration pour City* _ville; dans Human, et World a une _listVille

class World {
private:
    // --- Paramètres Statiques de la Simulation ---
    // REFACTORING: Ces paramètres sont initialisés par initWorldParams à partir d'un objet SimulationParams.
    // Idéalement, la classe World utiliserait son membre _paramsInstance pour accéder à ces valeurs dynamiquement,
    // réduisant la dépendance aux statiques pour les paramètres de la simulation courante.
    // Conservés pour l'instant pour minimiser les modifications de la structure existante.
    static int              _size;
    // static int              _nbIteration; // REFACTORING: Remplacé par un membre d'instance _max_iterations
    static float            _facteurTailleHopitaux;
    static int              _nbHopitaux;
    // ... (liste complète des autres variables statiques de la version utilisateur) ...
    static float            _tauxAugmentationContaminationHopitaux;
    static float            _tauxContaminationHopitaux;
    static int              _nbSalleDeSport;
    static float            _tauxAugmentationContaminationGym;
    static float            _tauxContaminationGym;
    static int              _nbBoiteDeNuit;
    static float            _tauxAugmentationContaminationClub;
    static float            _tauxContaminationClub;
    static int              _nbMagasin;
    static float            _tauxAugmentationContaminationShop;
    static float            _tauxContaminationShop;
    static int              _nbRestaurant;
    static float            _tauxAugmentationContaminationRestaurant;
    static float            _tauxContaminationRestaurant;
    static int              _nbPlaceHospital;
    static int              _nbPlaceReanimation;
    static float            _tauxMortRea;
    static int              _nbHumain;
    static int              _nbMalade;
    static int              _isVaccin;
    static int              _isMedicament;
    static float            _tauxProtectionReaMedicament;
    static int              _isMasqueTissu;
    static float            _tauxProtectionMasqueTissu;
    static int              _isMasqueChir;
    static float            _tauxProtectionMasqueChir;
    static int              _isMasqueFFP2;
    static float            _tauxProtectionMasqueFFP2;
    static int              _isConfinement;
    static int              _nbDeplacementReductionConfinement;
    static int              _nbLimiteDistanceMaxConfinement;
    static int              _isDeplacementLimites;
    static int              _nbDistanceMax;
    static int              _isGelHydroalcolique;
    static float            _tauxProtectionTransmissionGelHydro;
    static int              _isTestCovid;
    static float            _tauxDeDivisionAsymptomatique;
    static int              _isCouvreFeu;
    static int              _nbDeplacementReductionCouvreFeu;
    static int              _nbSuperContaminateur;
    static int              _rayonSuperContaminateur;
    static int              _nbDeplacementJour;
    static int              _nbVariants;
    static float            _r0;
    static std::vector<float>   _defaultVariantHistoConta;
    static std::vector<float>   _variantsHistoConta;
    static std::vector<int>     _dureeVariants;
    static std::vector<float>   _pourcentAsymptomatiqueVariants;
    static std::vector<float>   _tableTauxHospitalisationByAgeVariants;
    static std::vector<int>      _tailleVilles;
    static std::vector<int>      _nbHabitants;
    static std::vector<int>      _coordonneesX;
    static std::vector<int>      _coordonneesY;
    static std::vector<float>  _tableTauxHospitalisationByAge;
    static std::vector<int>    _tableTailleHopitaux;
    static float             _pourcentAsymptomatique;
    static float            _tauxVaccination;
    static float            _tauxVaccinationRappel;
    static std::deque<int>        _timelineVaccination;
    static std::deque<int>        _timelineVaccinationRappel;
    static std::deque<int>        _timelineMasqueTissu;
    static std::deque<int>        _timelineMasqueChir;
    static std::deque<int>        _timelineMasqueFFP2;
    static std::deque<int>        _timelineGelHydroalcolique;
    static std::deque<int>        _timelineConfinement;
    static std::deque<int>        _timelineCouvreFeu;
    static std::deque<int>        _timelineMedicament;
    static std::deque<int>        _timelineDeplacementLimites;
    static float             _tauxDeChanceDeMourirHospitalFull;
    static float             _tauxContaDistanceDeux;
    static std::vector<float> _histogrammeContamination;
    static float            _tauxObesitePopulation;
    static float            _tauxDiabetePopulation; // Dernier statique listé par l'utilisateur


    // --- Attributs d'Instance de la Simulation ---
    Human*** _carte;
    char**   _carteLieu;

    std::vector<City*> _listVille;
    double   _tabAugmentionContaminationParLieu[256];

    int      _totalPersonneVaccine; // Retiré =0 pour être initialisé dans constructeur
    int      _totalPersonneVaccineRappel; // Retiré =0

    float**  _timelineContamination; // Initialisé à nullptr dans constructeur
    float*   _timelineAsymptomatique; // Initialisé à nullptr
    float**  _timelineHospByAge;    // Initialisé à nullptr

    std::vector<Position> _humanSafePositions;
    std::vector<Position> _humanAsymptomatiquePositions;
    std::vector<Position> _humanConfinedPositions;
    std::vector<Position> _humanHospitalPositions;
    std::vector<Position> _humanReanimationPositions;

    std::vector<Position> _newHumanSafePositions;
    std::vector<Position> _newCurrentHumanAsymptomatiquePositions;
    std::vector<Position> _newNextHumanAsymptomatiquePositions;
    std::vector<Position> _newHumanConfinedPositions;
    std::vector<Position> _newHumanHospitalPositions;
    std::vector<Position> _newHumanReanimationPositions;


    bool      _log;
    std::fstream _logfile;

    int       _iteration;
    int       _max_iterations;

    std::map<std::string, int> _stats;

    static const char SYMBOL_EMPTY = '.';
    static constexpr float MAX_HUMANS = 0.8f;

    std::vector<int> _ageOfDeadHumansDaily;
    std::vector<int> _ageOfSymptomaticDailyHuman;
    int              _nbNouveauxCas;
    int              _nbPersonneHospital;
    int              _nbPersonneReanimation;
    int              _nbNouveauxHospitalisation;
    int              _nbNouveauxReanimation;
    int              _nbMorts;
    int              _nbCasCovidConnuTotal;
    int              _nbHumainContaminePlusieursFois;
    std::deque<int>  _nbNouveauxCasSemaine;

    double _ratioHospRea[633];

    SimulationParams* _paramsInstance;
    std::string       _modeSimulationInternal;


public:
    World(SimulationParams* params, char* nomFichierLog, bool log);
    ~World();

    void                         writeLog(const std::string& inMsg);
    void                         initTimeline();
    static void                  pause();
    void                         display();
    void                         updateStats(const std::string& inState);
    void                         displayStats() const;
    bool                         isValid(int r, int c) const;
    bool                         isHuman(int r, int c) const;
    bool                         isEmpty(int r, int c) const;

    void                         addAgent(SimulationParams* inSimulationParams, float inWorldMax);
    void                         initialize(SimulationParams* inSimulationParams);

    std::map<std::string, std::vector<Position>> vision (int length, int r, int c) const;

    void                         contamination(int targetRow, int targetCol, int sourceRow, int sourceCol);
    void                         humanGoFromTo(int fromRow, int fromCol, int toRow, int toCol, bool die = false);

    void                         gestionVaccination();
    void                         gestionMesuresBarrieres();

    void                         moveHumanSafe(int r, int c);
    void                         moveHumanAsymptomatique(int r, int c, int rDebut, int cDebut);
    void                         moveHumanConfined(int r, int c);
    void                         moveHumanHospital(int r, int c);
    void                         moveHumanReanimation(int r, int c);

    void                         nextIteration();
    void                         startSimulation(SimulationParams* params);

    static void                  initWorldParams(SimulationParams* params);

    void                         logIterationSummary();

    // static SimulationParams* getStaticParamsInstance() { return _paramsInstance; } // Example if needed
    // static std::string getStaticModeSimulationInternal() { return _modeSimulationInternal; } // Example
};

#endif // _SIMU_EPIDEMIO_WORLD_
