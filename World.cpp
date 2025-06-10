#include "World.hpp"
#include "SimulationParams.hpp"
#include "Human.hpp"
#include "City.hpp"
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdexcept>

// Static member definitions
int              World::_size;
//int              World::_nbIteration; // Instance member _max_iterations now used
float            World::_facteurTailleHopitaux;
int              World::_nbHopitaux;
float            World::_tauxAugmentationContaminationHopitaux;
float            World::_tauxContaminationHopitaux;
int              World::_nbSalleDeSport;
float            World::_tauxAugmentationContaminationGym;
float            World::_tauxContaminationGym;
int              World::_nbBoiteDeNuit;
float            World::_tauxAugmentationContaminationClub;
float            World::_tauxContaminationClub;
int              World::_nbMagasin;
float            World::_tauxAugmentationContaminationShop;
float            World::_tauxContaminationShop;
int              World::_nbRestaurant;
float            World::_tauxAugmentationContaminationRestaurant;
float            World::_tauxContaminationRestaurant;
int              World::_nbPlaceHospital;
int              World::_nbPlaceReanimation;
float            World::_tauxMortRea;
int              World::_nbHumain;
int              World::_nbMalade;
int              World::_isVaccin;
int              World::_isMedicament;
float            World::_tauxProtectionReaMedicament;
int              World::_isMasqueTissu;
float            World::_tauxProtectionMasqueTissu;
int              World::_isMasqueChir;
float            World::_tauxProtectionMasqueChir;
int              World::_isMasqueFFP2;
float            World::_tauxProtectionMasqueFFP2;
int              World::_isConfinement;
int              World::_nbDeplacementReductionConfinement;
int              World::_nbLimiteDistanceMaxConfinement;
int              World::_isDeplacementLimites;
int              World::_nbDistanceMax;
int              World::_isGelHydroalcolique;
float            World::_tauxProtectionTransmissionGelHydro;
int              World::_isTestCovid;
float            World::_tauxDeDivisionAsymptomatique;
int              World::_isCouvreFeu;
int              World::_nbDeplacementReductionCouvreFeu;
int              World::_nbSuperContaminateur;
int              World::_rayonSuperContaminateur;
int              World::_nbDeplacementJour;
int              World::_nbVariants;
float            World::_r0;
std::vector<float>   World::_defaultVariantHistoConta;
std::vector<float>   World::_variantsHistoConta;
std::vector<int>     World::_dureeVariants;
std::vector<float>   World::_pourcentAsymptomatiqueVariants;
std::vector<float>   World::_tableTauxHospitalisationByAgeVariants;
std::vector<int>      World::_tailleVilles;
std::vector<int>      World::_nbHabitants;
std::vector<int>      World::_coordonneesX;
std::vector<int>      World::_coordonneesY;
std::vector<float>  World::_tableTauxHospitalisationByAge;
std::vector<int>    World::_tableTailleHopitaux;
float             World::_pourcentAsymptomatique;
float            World::_tauxVaccination;
float            World::_tauxVaccinationRappel;
std::deque<int>        World::_timelineVaccination;
std::deque<int>        World::_timelineVaccinationRappel;
std::deque<int>        World::_timelineMasqueTissu;
std::deque<int>        World::_timelineMasqueChir;
std::deque<int>        World::_timelineMasqueFFP2;
std::deque<int>        World::_timelineGelHydroalcolique;
std::deque<int>        World::_timelineConfinement;
std::deque<int>        World::_timelineCouvreFeu;
std::deque<int>        World::_timelineMedicament;
std::deque<int>        World::_timelineDeplacementLimites;
float             World::_tauxDeChanceDeMourirHospitalFull;
float             World::_tauxContaDistanceDeux;
std::vector<float> World::_histogrammeContamination;
float            World::_tauxObesitePopulation;
float            World::_tauxDiabetePopulation;

SimulationParams* World::_paramsInstance = nullptr;
std::string World::_modeSimulationInternalStatic;


World::World(SimulationParams* params, char* inNomFichierLog, bool inLog)
    : _paramsInstance(params),
      _log(inLog),
      _iteration(0),
      _totalPersonneVaccine(0),
      _totalPersonneVaccineRappel(0),
      _nbNouveauxCas(0),
      _nbPersonneHospital(0),
      _nbPersonneReanimation(0),
      _nbNouveauxHospitalisation(0),
      _nbNouveauxReanimation(0),
      _nbMorts(0),
      _nbCasCovidConnuTotal(0),
      _nbHumainContaminePlusieursFois(0),
      _timelineContamination(nullptr),
      _timelineAsymptomatique(nullptr),
      _timelineHospByAge(nullptr)
{
    if (!_paramsInstance) {
        throw std::runtime_error("Erreur CRITIQUE: SimulationParams est nul dans le constructeur de World.");
    }
    _modeSimulationInternal = _paramsInstance->getModeSimulation();
    _max_iterations = _paramsInstance->getNbIteration();

    int current_size = _paramsInstance->getSize();

    _carte = (Human ***)malloc(current_size * sizeof(Human**));
    for(int i = 0; i < current_size; i++) {
        _carte[i] = (Human **)malloc(current_size * sizeof(Human*));
        for(int j = 0; j < current_size; j++) {
            _carte[i][j] = nullptr;
        }
    }
    _carteLieu = (char **) malloc(current_size * sizeof(char*));
    for(int i =0; i < current_size; i++) {
        _carteLieu[i] = (char *) malloc (current_size * sizeof(char));
        for(int j = 0; j < current_size; j++) {
            _carteLieu[i][j] = 0;
        }
    }

    for(int i = 0; i < 256; i++) { _tabAugmentionContaminationParLieu[i] = 0.0; }
    _tabAugmentionContaminationParLieu['H'] = _paramsInstance->getTauxAugmentationContaminationHopitaux();
    _tabAugmentionContaminationParLieu['G'] = _paramsInstance->getTauxAugmentationContaminationGym();
    _tabAugmentionContaminationParLieu['C'] = _paramsInstance->getTauxAugmentationContaminationClub();
    _tabAugmentionContaminationParLieu['S'] = _paramsInstance->getTauxAugmentationContaminationShop();
    _tabAugmentionContaminationParLieu['R'] = _paramsInstance->getTauxAugmentationContaminationRestaurant();

    if (_listVille.empty() && !_paramsInstance->getTailleVilles().empty()) {
        const std::vector<int>& tailles = _paramsInstance->getTailleVilles();
        const std::vector<int>& nbHabs = _paramsInstance->getNbHabitants();
        const std::vector<int>& coordsX = _paramsInstance->getCoordonneesX();
        const std::vector<int>& coordsY = _paramsInstance->getCoordonneesY();
        for(size_t i=0; i < tailles.size(); ++i) {
           _listVille.push_back(new City(tailles[i], nbHabs[i], coordsX[i], coordsY[i]));
        }
    }

    _stats["dead"] = 0; _stats["contamined"] = 0; _stats["recovered"] = 0; _stats["safe"] = 0;

    if(_log) {
        _logfile.open(inNomFichierLog, std::ios::out);
        if (!_logfile.is_open()) {
            std::cerr << "Erreur: Impossible d'ouvrir le fichier log: " << inNomFichierLog << std::endl;
            _log = false;
        }
    }
    initTimeline();
}

World::~World() {
    int current_size = _paramsInstance ? _paramsInstance->getSize() : World::_size; // Prefer instance if available for safety
    if (_carte) {
        for(int i = 0; i < current_size ;i++) {
          for(int j = 0; j < current_size ; j++) {
             delete(_carte[i][j]); _carte[i][j] = nullptr;
          }
          if (_carte[i]) free(_carte[i]); _carte[i] = nullptr;
       }
       free(_carte); _carte = nullptr;
    }

   if (_carteLieu) {
       for(int i = 0; i < current_size; i++){
          if (_carteLieu[i]) free(_carteLieu[i]); _carteLieu[i] = nullptr;
       }
       free(_carteLieu); _carteLieu = nullptr;
   }

    int timeline_len = _max_iterations;

   if (_timelineContamination) {
       for(int i = 0; i < timeline_len; i++){
          if(_timelineContamination[i]) free(_timelineContamination[i]); _timelineContamination[i] = nullptr;
       }
       free(_timelineContamination); _timelineContamination = nullptr;
   }
   if (_timelineHospByAge) {
       for(int i = 0; i < timeline_len; i++){
          if(_timelineHospByAge[i]) free(_timelineHospByAge[i]); _timelineHospByAge[i] = nullptr;
       }
       free(_timelineHospByAge); _timelineHospByAge = nullptr;
   }
   if (_timelineAsymptomatique) {
       free(_timelineAsymptomatique); _timelineAsymptomatique = nullptr;
   }
    for(City* c : _listVille) { delete c; }
    _listVille.clear();
   if(_log && _logfile.is_open()) { _logfile.close(); }
}


void World::initTimeline() {
    if (!_paramsInstance || _paramsInstance->getNbVariants() == 0 || _paramsInstance->getDureeVariants().empty() || _max_iterations == 0) {
        return;
    }
    int num_variants = _paramsInstance->getNbVariants();
    const std::vector<int>& duree_variants_vec = _paramsInstance->getDureeVariants();
    const std::vector<float>& variants_histo_conta_vec = _paramsInstance->getVariantsHistoConta();
    const std::vector<float>& default_variant_histo_conta_vec = _paramsInstance->getDefaultVariantHistoConta();
    const std::vector<float>& percent_asympt_variants_vec = _paramsInstance->getPourcentAsymptomatiqueVariants();
    const std::vector<float>& table_hosp_by_age_variants_vec = _paramsInstance->getTableTauxHospitalisationByAgeVariants();
    const std::vector<float>& table_hosp_by_age_default_vec = _paramsInstance->getTableTauxHospitalisationByAge();

    int* dureeVariantsCumulative = new int[num_variants];
    if (num_variants > 0 && !duree_variants_vec.empty()) { // Check not empty
        dureeVariantsCumulative[0] = duree_variants_vec[0];
        for(int i = 1; i < num_variants; i++) {
            if (static_cast<size_t>(i) < duree_variants_vec.size()) { // Bounds check
                 dureeVariantsCumulative[i] = duree_variants_vec[i] + dureeVariantsCumulative[i-1];
            } else { // Should not happen if duree_variants_vec.size() == num_variants
                 dureeVariantsCumulative[i] = dureeVariantsCumulative[i-1];
            }
        }
    }


    _timelineContamination = (float **) malloc(_max_iterations * sizeof(float*));
    for(int i =0; i < _max_iterations; i++) {
        _timelineContamination[i] = (float *) malloc (11 * sizeof(float));
    }
    int compteur = 0;
    for(int nb = 0; nb < num_variants; nb++) {
        int end_day_variant = (nb < num_variants && static_cast<size_t>(nb) < num_variants) ? dureeVariantsCumulative[nb] : _max_iterations; // Check bounds
        for(compteur = (nb==0?0:dureeVariantsCumulative[nb-1]); compteur < end_day_variant && compteur < _max_iterations ;compteur++) {
            for(int j = 0; j<11 ; j++) {
                if ( static_cast<size_t>(j + 11*nb) < variants_histo_conta_vec.size())
                    _timelineContamination[compteur][j] = variants_histo_conta_vec[j + 11*nb];
                else if (static_cast<size_t>(j) < default_variant_histo_conta_vec.size())
                    _timelineContamination[compteur][j] = default_variant_histo_conta_vec[j];
                else _timelineContamination[compteur][j] = 0;
            }
        }
    }
    for(; compteur < _max_iterations ;compteur++) {
        for(int j = 0; j<11 ; j++) {
            if (static_cast<size_t>(j) < default_variant_histo_conta_vec.size())
                _timelineContamination[compteur][j] = default_variant_histo_conta_vec[j];
            else _timelineContamination[compteur][j] = 0;
        }
    }

    _timelineAsymptomatique = (float *) malloc(_max_iterations * sizeof(float));
    compteur = 0;
    for(int nb = 0; nb < num_variants; nb++) {
        int end_day_variant = (nb < num_variants && static_cast<size_t>(nb) < num_variants) ? dureeVariantsCumulative[nb] : _max_iterations;
        for(compteur = (nb==0?0:dureeVariantsCumulative[nb-1]); compteur < end_day_variant && compteur < _max_iterations; compteur++) {
            if (static_cast<size_t>(nb) < percent_asympt_variants_vec.size())
                _timelineAsymptomatique[compteur] = percent_asympt_variants_vec[nb];
            else _timelineAsymptomatique[compteur] = _paramsInstance->getPourcentAsymptomatique();
        }
    }
    for(; compteur < _max_iterations; compteur++) {
        _timelineAsymptomatique[compteur] = _paramsInstance->getPourcentAsymptomatique();
    }

    _timelineHospByAge = (float **) malloc(_max_iterations * sizeof(float*));
    for(int i =0; i < _max_iterations; i++) {
        _timelineHospByAge[i] = (float *) malloc (8 * sizeof(float));
    }
    compteur = 0;
    for(int nb = 0; nb < num_variants; nb++) {
        int end_day_variant = (nb < num_variants && static_cast<size_t>(nb) < num_variants) ? dureeVariantsCumulative[nb] : _max_iterations;
        for(compteur = (nb==0?0:dureeVariantsCumulative[nb-1]); compteur < end_day_variant && compteur < _max_iterations; compteur++) {
            for(int j = 0; j<8 ; j++) {
                if (static_cast<size_t>(j + 8*nb) < table_hosp_by_age_variants_vec.size())
                    _timelineHospByAge[compteur][j] = table_hosp_by_age_variants_vec[j + 8*nb];
                else if (static_cast<size_t>(j) < table_hosp_by_age_default_vec.size())
                    _timelineHospByAge[compteur][j] = table_hosp_by_age_default_vec[j];
                else _timelineHospByAge[compteur][j] = 0;
            }
        }
    }
    for(; compteur < _max_iterations ;compteur++) {
        for(int j = 0; j<8 ; j++) {
            if (static_cast<size_t>(j) < table_hosp_by_age_default_vec.size())
                _timelineHospByAge[compteur][j] = table_hosp_by_age_default_vec[j];
            else _timelineHospByAge[compteur][j] = 0;
        }
    }
   delete[] dureeVariantsCumulative;
}

void World::addAgent(SimulationParams * inSimulationParams, float inWorldMax ) {
   if (!inSimulationParams) {
       std::cerr << "Erreur critique: inSimulationParams est nul dans World::addAgent" << std::endl;
       throw std::runtime_error("SimulationParams est nul dans World::addAgent");
   }
    std::string modeSim = inSimulationParams->getModeSimulation();
   int current_nb_humain = inSimulationParams->getNbHumain();
   int current_nb_malade = inSimulationParams->getNbMalade();
   int current_grid_size = inSimulationParams->getSize();

   if(current_nb_malade > current_nb_humain) {
      std::cerr << "Erreur de configuration: nbMalade (" << current_nb_malade << ") > nbHumain (" << current_nb_humain << ")" << std::endl;
      throw std::runtime_error("Configuration invalide: nbMalade > nbHumain");
   }

   int tempComptSuperContaminateur = inSimulationParams->getNbSuperContaminateur();
   int agentsPlaces = 0;
   int nbMaladeRestantAInitialiser = current_nb_malade;

   if (!_listVille.empty()) {
        for(City* c : _listVille) {
            if (!c) continue;
            int nbHabVille = c->getNbHab();
            if (agentsPlaces + nbHabVille > current_nb_humain) {
                nbHabVille = current_nb_humain - agentsPlaces;
                if (nbHabVille < 0) nbHabVille = 0;
            }
            for(int i = 0; i < nbHabVille && agentsPlaces < current_nb_humain; ++i) {
                bool varEmpty = false; int row = 0; int column = 0; int tentatives_placement = 0;
                const int max_tentatives = (current_grid_size > 0 && c->getTaille() > 0) ? (current_grid_size * current_grid_size * 2) : 1000;
                while(!varEmpty && tentatives_placement < max_tentatives) {
                    if (c->getTaille() > 0) {
                        row = (randmt->genrand_int32() % c->getTaille()) + c->getCoordX();
                        column = (randmt->genrand_int32() % c->getTaille()) + c->getCoordY();
                        row = std::max(0, std::min(row, current_grid_size - 1));
                        column = std::max(0, std::min(column, current_grid_size - 1));
                    } else {
                        row = randmt->genrand_int32() % current_grid_size ; column = randmt->genrand_int32() % current_grid_size ;
                    }
                    varEmpty = isEmpty(row,column); tentatives_placement++;
                }
                if (!varEmpty) {
                    tentatives_placement = 0;
                    while(!varEmpty && tentatives_placement < max_tentatives) {
                        row = randmt->genrand_int32() % current_grid_size ; column = randmt->genrand_int32() % current_grid_size ;
                        varEmpty = isEmpty(row, column); tentatives_placement++;
                    }
                    if (!varEmpty) {std::cerr << "Avertissement: Impossible de placer l'agent " << agentsPlaces << " après de nombreuses tentatives." << std::endl; continue;}
                }
                _carte[row][column] = new Human(inSimulationParams, row, column);
                _carte[row][column]->setVille(c); agentsPlaces++;
                if (nbMaladeRestantAInitialiser > 0) {
                    if (modeSim == "GENERIQUE_SEIRS") {
                        _carte[row][column]->_statut_generique_actuel = StatutGenerique::I;
                        _carte[row][column]->_temps_en_statut_generique = 0;
                    } else { _carte[row][column]->contamine(); }
                    nbMaladeRestantAInitialiser--;
                } else {
                     if (modeSim == "GENERIQUE_SEIRS") {
                        _carte[row][column]->_statut_generique_actuel = StatutGenerique::S;
                        _carte[row][column]->_temps_en_statut_generique = 0;
                    }
                }
                if (modeSim != "GENERIQUE_SEIRS") {
                    if(tempComptSuperContaminateur > 0) {
                        _carte[row][column]->setIsSuperContaminateur(true); tempComptSuperContaminateur--;
                    }
                    if(randmt->genrand_real1() < inSimulationParams->getTauxObesitePopulation()) _carte[row][column]->setIsObese();
                    if(randmt->genrand_real1() < inSimulationParams->getTauxDiabetePopulation()) _carte[row][column]->setIsDiabete();
                }
            }
        }
   }
   for(int i = agentsPlaces; i < current_nb_humain; i++) {
      bool  varEmpty = false; int row = 0; int column = 0; int tentatives_placement = 0;
      const int max_tentatives = (current_grid_size * current_grid_size) > 0 ? (current_grid_size * current_grid_size * 2) : 1000;
      while(!varEmpty && tentatives_placement < max_tentatives) {
         row = randmt->genrand_int32() % current_grid_size ; column = randmt->genrand_int32() % current_grid_size ;
         varEmpty = isEmpty(row,column); tentatives_placement++;
      }
      if (!varEmpty) {std::cerr << "Avertissement: Impossible de placer l'agent " << i << " (hors ville) après de nombreuses tentatives." << std::endl; continue;}
      _carte[row][column] = new Human(inSimulationParams,row,column);
      if (nbMaladeRestantAInitialiser > 0) {
          if (modeSim == "GENERIQUE_SEIRS") {
              _carte[row][column]->_statut_generique_actuel = StatutGenerique::I;
              _carte[row][column]->_temps_en_statut_generique = 0;
          } else { _carte[row][column]->contamine(); }
          nbMaladeRestantAInitialiser--;
      } else {
          if (modeSim == "GENERIQUE_SEIRS") {
            _carte[row][column]->_statut_generique_actuel = StatutGenerique::S;
            _carte[row][column]->_temps_en_statut_generique = 0;
          }
      }
      if (modeSim != "GENERIQUE_SEIRS") {
            if(inSimulationParams->getIsVaccin() == 1) {
                if(randmt->genrand_real1() < inSimulationParams->getTauxVaccination()) {
                    _carte[row][column]->vaccine();
                    if(randmt->genrand_real1() < inSimulationParams->getTauxVaccinationRappel()) _carte[row][column]->vaccineRappel();
                }
            }
            if(randmt->genrand_real1() < inSimulationParams->getTauxObesitePopulation()) _carte[row][column]->setIsObese();
            if(randmt->genrand_real1() < inSimulationParams->getTauxDiabetePopulation()) _carte[row][column]->setIsDiabete();
      }
      agentsPlaces++;
   }
   _stats["safe"] = current_nb_humain - current_nb_malade;
   _stats["contamined"] = current_nb_malade;
   _stats["recovered"] = 0; _stats["dead"] = 0;
   for(int qtyShop = 0; qtyShop < _paramsInstance->getNbMagasin(); qtyShop++) {
      int r, cl, taille = 20; r = randmt->genrand_int32()%current_grid_size ; cl = randmt->genrand_int32()%current_grid_size ;
      for(int k = r; k < r+taille; k++) for(int l = cl; l < cl+taille; l++) if(isValid(k,l)) _carteLieu[k][l] = 'S';
   }
   for(int qtyRestaurant = 0; qtyRestaurant < _paramsInstance->getNbRestaurant(); qtyRestaurant++) {
      int r, cl, taille = 20; r = randmt->genrand_int32()%current_grid_size ; cl = randmt->genrand_int32()%current_grid_size ;
      for(int k = r; k < r+taille; k++) for(int l = cl; l < cl+taille; l++) if(isValid(k,l)) _carteLieu[k][l] = 'R';
   }
   for(int qtyGym = 0; qtyGym < _paramsInstance->getNbSalleDeSport(); qtyGym++) {
      int r, cl, taille = 20; r = randmt->genrand_int32()%current_grid_size ; cl = randmt->genrand_int32()%current_grid_size ;
      for(int k = r; k < r+taille; k++) for(int l = cl; l < cl+taille; l++) if(isValid(k,l)) _carteLieu[k][l] = 'G';
   }
   for(int qtyClub = 0; qtyClub < _paramsInstance->getNbBoiteDeNuit(); qtyClub++) {
      int r, cl, taille = 20; r = randmt->genrand_int32()%current_grid_size ; cl = randmt->genrand_int32()%current_grid_size ;
      for(int k = r; k < r+taille; k++) for(int l = cl; l < cl+taille; l++) if(isValid(k,l)) _carteLieu[k][l] = 'C';
   }
   for(int qtyHopitaux = 0; qtyHopitaux < _paramsInstance->getNbHopitaux(); qtyHopitaux++) {
      int r, cl; r = randmt->genrand_int32()%current_grid_size ; cl = randmt->genrand_int32()%current_grid_size ;
      const std::vector<int>& tableTailleHop = _paramsInstance->getTableTailleHopitaux();
      int taille = (tableTailleHop.size() > static_cast<size_t>(qtyHopitaux) && tableTailleHop[qtyHopitaux] > 0) ? static_cast<int>(sqrt(tableTailleHop[qtyHopitaux])) : 0;
      taille = static_cast<int>(taille * _paramsInstance->getFacteurTailleHopitaux());
      for(int k = r; k < r+taille; k++) for(int l = cl; l < cl+taille; l++) if(isValid(k,l)) _carteLieu[k][l] = 'H';
   }
}

void World::initialize(SimulationParams * inSimulationParams) {
    if (!inSimulationParams) throw std::runtime_error("SimulationParams is null in World::initialize");
    if (!_paramsInstance) _paramsInstance = inSimulationParams;
    addAgent(inSimulationParams, World::MAX_HUMANS);
}

std::map<std::string,std::vector<Position>> World::vision(int inLength, int inRow, int inColumn) const {
   Position maPositionTest; std::map<std::string, std::vector<Position>> neighborhood;
   neighborhood["empty"]; neighborhood["human"];
   int current_grid_size = _paramsInstance->getSize();
   for(int elt_1 = -inLength; elt_1 <= inLength; elt_1++) {
      for(int elt_2 = -inLength; elt_2 <= inLength; elt_2++) {
         if (elt_1 == 0 && elt_2 == 0) continue;
         int current_r = inRow + elt_1; int current_c = inColumn + elt_2;
         current_r = (current_r + current_grid_size) % current_grid_size;
         current_c = (current_c + current_grid_size) % current_grid_size;

         if(isValid(current_r, current_c)) {
            maPositionTest.setPosX(current_r); maPositionTest.setPosY(current_c);
            if(isHuman(current_r, current_c)) neighborhood["human"].push_back(_carte[current_r][current_c]->getPosition());
            else if(isEmpty(current_r, current_c)) neighborhood["empty"].push_back(maPositionTest);
         }
      }
   }
   return neighborhood;
}

void World::contamination(int targetRow, int targetCol, int sourceRow, int sourceCol) {
    if (!_carte[sourceRow][sourceCol] || !_paramsInstance) return;
    Human* infecting_human = _carte[sourceRow][sourceCol];
    map<string, vector<Position>> target_v1 = vision(infecting_human->getIsSuperContaminateur() ? _paramsInstance->getRayonSuperContaminateur() : 2, targetRow, targetCol);
    for(Position pos : target_v1["human"]) {
        Human* targetHuman = _carte[pos.getPosX()][pos.getPosY()];
        if(targetHuman && !targetHuman->isSick(_modeSimulationInternal)) {
            float randomValue = randmt->genrand_real1();
            int distance = std::max(abs(targetRow - pos.getPosX()), abs(targetCol - pos.getPosY()));
            if(infecting_human->getIsSuperContaminateur()) distance = 1;

            float probaContaBase = 0.0f;
            int current_iteration_timeline = std::min(_iteration, static_cast<int>(_max_iterations) -1 );
             current_iteration_timeline = std::max(0, current_iteration_timeline);

            if (_timelineContamination && static_cast<size_t>(current_iteration_timeline) < _max_iterations &&
                infecting_human->getState()-1 >=0 &&
                static_cast<size_t>(infecting_human->getState()-1) < 11 ) {
                 if (_timelineContamination[current_iteration_timeline] != nullptr)
                    probaContaBase = _timelineContamination[current_iteration_timeline][infecting_human->getState()-1];
            }

            if(randomValue < probaContaBase *
            (1.0f - targetHuman->getTauxDeProtectionInfection()) *
            (1.0f - (_paramsInstance->getTauxProtectionMasqueTissu() * _paramsInstance->getIsMasqueTissu())) *
            (1.0f - (_paramsInstance->getTauxProtectionMasqueChir() * _paramsInstance->getIsMasqueChir())) *
            (1.0f - (_paramsInstance->getTauxProtectionMasqueFFP2() * _paramsInstance->getIsMasqueFFP2())) *
            (1.0f - (_paramsInstance->getTauxProtectionTransmissionGelHydro() * _paramsInstance->getIsGelHydroalcolique())) *
            (1.0f - ((1.0f - _paramsInstance->getTauxContaDistanceDeux()) * (distance - 1))) *
            (1.0f + _tabAugmentionContaminationParLieu[_carteLieu[pos.getPosX()][pos.getPosY()]]))
            {
                targetHuman->contamine();
                if(targetHuman->getNumberOfInfections() > 1) _nbHumainContaminePlusieursFois++;
                _nbNouveauxCas++;
            }
        }
    }
}

void World::humanGoFromTo(int fromRow, int fromCol, int toRow, int toCol, bool die) {
   if (!isValid(fromRow, fromCol) || !_carte[fromRow][fromCol]) return;
   Human* agent = _carte[fromRow][fromCol];
   if(!die) {
      if (!isValid(toRow, toCol)) return;
      if (_carte[toRow][toCol] != nullptr && _carte[toRow][toCol] != agent) return;
      _carte[toRow][toCol] = agent;
      agent->setPosition(toRow,toCol);
      if (fromRow != toRow || fromCol != toCol) _carte[fromRow][fromCol] = nullptr;
   } else {
      delete agent;
      _carte[fromRow][fromCol] = nullptr;
   }
}

int World::getCurrentContaminationDuration(Human* agent, SimulationParams* params, int current_iteration_day) {
    if (!agent || !params) return 11;

    const std::vector<float>* ptr_histoConta = nullptr;
    if (params->getNbVariants() > 0 && !params->getDefaultVariantHistoConta().empty()) {
        ptr_histoConta = &params->getDefaultVariantHistoConta();
    }
    if (!ptr_histoConta && !params->getHistogrammeContamination().empty()) {
        ptr_histoConta = &params->getHistogrammeContamination();
    }
    if (ptr_histoConta && !ptr_histoConta->empty()) {
        return static_cast<int>(ptr_histoConta->size());
    }
    return 11;
}

void World::moveHumanSafe(int r, int c){
    if(!_carte[r][c] || !_paramsInstance) return;
    Human* agent = _carte[r][c];
    // updateHumanProtection and decreaseResistance are called in Human::evoluerQuotidiennement

    // Simplified displacement logic: find a random empty neighbor or stay
    int current_grid_size = _paramsInstance->getSize();
    int new_r = r; int new_c = c;
    std::vector<Position> emptyNeighbors;
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;
            int check_r = (r + dx + current_grid_size) % current_grid_size;
            int check_c = (c + dy + current_grid_size) % current_grid_size;
            if (isEmpty(check_r, check_c)) {
                emptyNeighbors.push_back(Position(check_r, check_c));
            }
        }
    }
    if (!emptyNeighbors.empty()) {
        Position& chosen_pos = emptyNeighbors[randmt->genrand_int32() % emptyNeighbors.size()];
        new_r = chosen_pos.getPosX();
        new_c = chosen_pos.getPosY();
    }

    if (new_r != r || new_c != c) {
        humanGoFromTo(r, c, new_r, new_c, false);
    }
    _newHumanSafePositions.push_back(_carte[new_r][new_c]->getPosition());
}

void World::moveHumanAsymptomatique(int r, int c, int rD, int cD){
    if(!_carte[r][c] || !_paramsInstance) return;
    Human* agent = _carte[r][c];
    // Recovery and becoming symptomatic logic is now in nextIteration post-evoluerQuotidiennement
    // This method should primarily handle displacement for asymptomatics if needed,
    // or be restructured if its unique logic is fully covered by nextIteration + Human::evoluer.
    // For now, it updates the new position list.
    _newCurrentHumanAsymptomatiquePositions.push_back(agent->getPosition());
}

void World::moveHumanConfined(int r, int c){
    if(!_carte[r][c] || !_paramsInstance) return;
    Human* agent = _carte[r][c];
    // Decisions for hospital, recovery are now in nextIteration post-evoluerQuotidiennement
    // This method primarily just ensures the agent is added to the correct list for the next state.
    _newHumanConfinedPositions.push_back(agent->getPosition());
}

void World::moveHumanHospital(int r, int c){
    if(!_carte[r][c] || !_paramsInstance) return;
    Human* agent = _carte[r][c];
    // Decisions for reanimation, recovery are now in nextIteration post-evoluerQuotidiennement
    _newHumanHospitalPositions.push_back(agent->getPosition());
}

void World::moveHumanReanimation(int r, int c){
    if(!_carte[r][c] || !_paramsInstance) return;
    Human* agent = _carte[r][c];
    // Decisions for death or recovery are now in nextIteration post-evoluerQuotidiennement
    _newHumanReanimationPositions.push_back(agent->getPosition());
}

void World::gestionVaccination(){
    if (!_paramsInstance) return;
    const std::deque<int>& timelineVac = _paramsInstance->getTimelineVaccination();
    if(!timelineVac.empty() && _iteration >= timelineVac[0] && timelineVac.size() >=2) { // Check _iteration and size
      // Mutable copy for pop_front
      std::deque<int> mutableTimeline = timelineVac; // This doesn't modify the static one from params
      // This logic needs to be re-thought if it's to modify the actual deque in params or a World copy
      // For now, this will only work for the first campaign if it modifies a copy.
      // World::_timelineVaccination.pop_front(); // This would modify static
      // ...
    }
}
void World::gestionMesuresBarrieres(){
    if (!_paramsInstance) return;
    // Similar to gestionVaccination, this modifies static World::_isMasqueTissu etc.
    // It should ideally work with instance members or a clearer state management for measures.
    // ...
}

void World::logIterationSummary() {
    if (!_log || !_paramsInstance || !_logfile.is_open()) return;
    std::string modeSim = _modeSimulationInternal;

    if (modeSim == "GENERIQUE_SEIRS") {
        int s_gen = 0; int e_gen = 0; int i_gen = 0; int r_gen = 0;
        int vivants_compteur = 0;
        int current_grid_size = _paramsInstance->getSize();
        for (int r_idx = 0; r_idx < current_grid_size; ++r_idx) {
            for (int c_idx = 0; c_idx < current_grid_size; ++c_idx) {
                if (_carte[r_idx][c_idx] != nullptr) {
                    vivants_compteur++; Human* agent = _carte[r_idx][c_idx];
                    switch (agent->getStatutGenerique()) {
                        case StatutGenerique::S: s_gen++; break;
                        case StatutGenerique::E: e_gen++; break;
                        case StatutGenerique::I: i_gen++; break;
                        case StatutGenerique::R: r_gen++; break;
                    }
                }
            }
        }
        _logfile << _iteration << "," << s_gen << "," << e_gen << "," << i_gen << "," << r_gen;
        _logfile << "," << _nbNouveauxCas << "\n";

        _stats["safe"] = s_gen + r_gen;
        _stats["contamined"] = i_gen + e_gen;
        _stats["recovered"] = r_gen;
        _stats["dead"] = _paramsInstance->getNbHumain() - vivants_compteur;
    } else {
        _logfile << _iteration << ",";
        _logfile << _stats["safe"] << ",";
        _logfile << (_stats["contamined"] - _nbPersonneHospital - _nbPersonneReanimation) << ",";
        _logfile << _nbPersonneHospital << ",";
        _logfile << _nbPersonneReanimation << ",";
        _logfile << _stats["recovered"] << ",";
        _logfile << _stats["dead"] << ",";
        _logfile << _nbNouveauxCas << ",";
        _logfile << _nbNouveauxHospitalisation << ",";
        _logfile << _nbNouveauxReanimation << ",";
        _logfile << _nbMorts << ",";
        _logfile << _nbCasCovidConnuTotal << ",";
        float totalMaladeActif = 0;
        // Recalculate based on current lists, ensure agents in lists are still valid if lists are rebuilt from _carte
        for (const Position& p : _humanAsymptomatiquePositions) if(_carte[p.getPosX()][p.getPosY()]) totalMaladeActif++;
        for (const Position& p : _humanConfinedPositions) if(_carte[p.getPosX()][p.getPosY()]) totalMaladeActif++;
        for (const Position& p : _humanHospitalPositions) if(_carte[p.getPosX()][p.getPosY()]) totalMaladeActif++;
        for (const Position& p : _humanReanimationPositions) if(_carte[p.getPosX()][p.getPosY()]) totalMaladeActif++;

        float reffectifJour = (totalMaladeActif > 0.0001f) ? (static_cast<float>(_nbNouveauxCas) / totalMaladeActif) : 0.0f;
        int dureeContaCovid = 11;
        if (_paramsInstance && !_paramsInstance->getHistogrammeContamination().empty()) {
            dureeContaCovid = _paramsInstance->getHistogrammeContamination().size();
        } else if (_paramsInstance && !_paramsInstance->getDefaultVariantHistoConta().empty()) {
            dureeContaCovid = _paramsInstance->getDefaultVariantHistoConta().size();
        }
        _logfile << (reffectifJour * dureeContaCovid) << ",";
        int nbNouveauxCasSemaineSomme = 0; for(int val : _nbNouveauxCasSemaine) nbNouveauxCasSemaineSomme += val;
        double tauxIncidence = (_paramsInstance->getNbHumain() > 0) ? (100000.0 * static_cast<double>(nbNouveauxCasSemaineSomme) / static_cast<double>(_paramsInstance->getNbHumain())) : 0.0;
        _logfile << tauxIncidence << "\n";
        for (int age: _ageOfSymptomaticDailyHuman) _logfile << "AgeC:" << age << "\n";
        for (int age: _ageOfDeadHumansDaily) _logfile << "AgeD:" << age << "\n";
        _logfile << "##########\n";
    }
}

void World::nextIteration() {
    if (!_paramsInstance) throw std::runtime_error("FATAL: _paramsInstance is null in nextIteration. World not properly initialized.");
    std::string currentModeSim = this->_modeSimulationInternal;
    int current_grid_size = _paramsInstance->getSize();

    gestionVaccination();
    gestionMesuresBarrieres();

    _nbNouveauxCas = 0; _nbNouveauxHospitalisation = 0; _nbNouveauxReanimation = 0;
    _nbMorts = 0;
    _ageOfSymptomaticDailyHuman.clear(); _ageOfDeadHumansDaily.clear();

    _newHumanSafePositions.clear();
    _newCurrentHumanAsymptomatiquePositions.clear();
    _newNextHumanAsymptomatiquePositions.clear();
    _newHumanConfinedPositions.clear();
    _newHumanHospitalPositions.clear();
    _newHumanReanimationPositions.clear();

    std::vector<Human*> agentsActifs;
    agentsActifs.reserve(_paramsInstance->getNbHumain());
    for (int r = 0; r < current_grid_size; ++r) {
        for (int c = 0; c < current_grid_size; ++c) {
            if (_carte[r][c] != nullptr) {
                agentsActifs.push_back(_carte[r][c]);
            }
        }
    }
    if (randmt) std::shuffle(agentsActifs.begin(), agentsActifs.end(), *randmt);

    for (Human* human_ptr : agentsActifs) {
        if (!human_ptr) continue;
        Position old_pos = human_ptr->getPosition();
        human_ptr->setPositionDebutTour(old_pos.getPosX(), old_pos.getPosY());

        bool agent_died_or_moved_critically = false;
        if (currentModeSim == "GENERIQUE_SEIRS") {
            int new_x = randmt->genrand_int32() % current_grid_size;
            int new_y = randmt->genrand_int32() % current_grid_size;
            if (_carte[old_pos.getPosX()][old_pos.getPosY()] == human_ptr) _carte[old_pos.getPosX()][old_pos.getPosY()] = nullptr;
            if (_carte[new_x][new_y] == nullptr) { human_ptr->setPosition(new_x, new_y); _carte[new_x][new_y] = human_ptr; }
            else { human_ptr->setPosition(old_pos.getPosX(), old_pos.getPosY()); if (!_carte[old_pos.getPosX()][old_pos.getPosY()]) _carte[old_pos.getPosX()][old_pos.getPosY()] = human_ptr; }
        } else {
            Position original_pos_for_turn = human_ptr->getPosition(); // Position avant les moveHumanXXX
            if (human_ptr->getIsReanimation()) moveHumanReanimation(original_pos_for_turn.getPosX(), original_pos_for_turn.getPosY());
            else if (human_ptr->getIsHospital()) moveHumanHospital(original_pos_for_turn.getPosX(), original_pos_for_turn.getPosY());
            else if (human_ptr->getIsConfined()) moveHumanConfined(original_pos_for_turn.getPosX(), original_pos_for_turn.getPosY());
            else if (human_ptr->getState() > 0) {
                int nbDeplacementsEffectifs = _paramsInstance->getNbDeplacementJour();
                 if (_paramsInstance->getIsConfinement()) nbDeplacementsEffectifs -= _paramsInstance->getNbDeplacementReductionConfinement();
                 if (_paramsInstance->getIsCouvreFeu()) nbDeplacementsEffectifs -= _paramsInstance->getNbDeplacementReductionCouvreFeu();
                 nbDeplacementsEffectifs = std::max(0, nbDeplacementsEffectifs);
                for (int d = 0; d < nbDeplacementsEffectifs; ++d) {
                    Position pos_avant_micro = human_ptr->getPosition();
                    moveHumanAsymptomatique(pos_avant_micro.getPosX(), pos_avant_micro.getPosY(),
                                           human_ptr->getPositionDebutTour().getPosX(), human_ptr->getPositionDebutTour().getPosY());
                    if (!human_ptr || !_carte[human_ptr->getPosition().getPosX()][human_ptr->getPosition().getPosY()] ||
                        _carte[human_ptr->getPosition().getPosX()][human_ptr->getPosition().getPosY()] != human_ptr) {
                        agent_died_or_moved_critically = true; break;
                    }
                }
            } else moveHumanSafe(original_pos_for_turn.getPosX(), original_pos_for_turn.getPosY());

            if (agent_died_or_moved_critically || !_carte[human_ptr->getPosition().getPosX()][human_ptr->getPosition().getPosY()] ||
                _carte[human_ptr->getPosition().getPosX()][human_ptr->getPosition().getPosY()] != human_ptr) {
                 continue;
            }
        }

        human_ptr->evoluerQuotidiennement(_paramsInstance, _carte, current_grid_size, current_grid_size, randmt);

        if (!_carte[human_ptr->getPosition().getPosX()][human_ptr->getPosition().getPosY()] ||
             _carte[human_ptr->getPosition().getPosX()][human_ptr->getPosition().getPosY()] != human_ptr) {
            continue; // Agent might have died during its own evolution (e.g. generic SEIRS model adds death)
        }

        if (currentModeSim == "COVID") {
            // Post-evolution state changes for COVID, like going to hospital/rea after symptoms worsen
            // This logic is now more complex because evoluerQuotidiennement handles internal COVID updates like incrementState
            Human* agent = human_ptr; Position current_pos = agent->getPosition();
            int dureeContaEff = getCurrentContaminationDuration(agent, _paramsInstance, _iteration);

            if (agent->getIsConfined() && !agent->getIsHospital() && !agent->getIsReanimation()){
                 if(agent->getState() > dureeContaEff) { agent->resetState(); /* updateStats("recovered"); handled by logIterationSummary */ }
                 else if (agent->getState() == 4) { /* Check for hospital */
                    float probaToHosp = (_timelineHospByAge[_iteration][agent->getAge()]/100.0f) * (1.0f - agent->getTauxDeProtectionHospitalisation()) * (1.0f + agent->getTauxComorbiditeHosp());
                    if (randmt->genrand_real1() < probaToHosp) {
                        if (_nbPersonneHospital < _paramsInstance->getNbPlaceHospital()) {
                            agent->goToHospital(); _nbNouveauxHospitalisation++; _nbPersonneHospital++;
                        } else { if (randmt->genrand_real1() < _paramsInstance->getTauxDeChanceDeMourirHospitalFull()) {
                            _ageOfDeadHumansDaily.push_back(agent->getAge()); _nbMorts++; delete _carte[current_pos.getPosX()][current_pos.getPosY()]; _carte[current_pos.getPosX()][current_pos.getPosY()] = nullptr; continue;
                        }}
                    }
                 }
            } else if (agent->getIsHospital() && !agent->getIsReanimation()){
                if(agent->getState() > dureeContaEff) { _nbPersonneHospital--; agent->resetState(); /* updateStats("recovered"); */ }
                else if (agent->getState() == 5) { /* Check for reanimation */
                    float tauxReaIfHosp = (_iteration < 633) ? _ratioHospRea[_iteration] : 0.15f;
                    float tauxReaIfHospByAgeVal = tauxReaIfHosp * ((agent->getAge()+1.0f)/2.0f);
                    float probaToRea = tauxReaIfHospByAgeVal * (1.0f - agent->getTauxDeProtectionReanimation()) * (1.0f + agent->getTauxComorbiditeRea());
                    if(_paramsInstance->getIsMedicament()) probaToRea *= (1.0f - _paramsInstance->getTauxProtectionReaMedicament());
                    if (randmt->genrand_real1() < probaToRea) {
                        if (_nbPersonneReanimation < _paramsInstance->getNbPlaceReanimation()) {
                            _nbPersonneHospital--; agent->goToReanimation(); _nbNouveauxReanimation++; _nbPersonneReanimation++;
                        } else { _nbPersonneHospital--; _ageOfDeadHumansDaily.push_back(agent->getAge()); _nbMorts++; delete _carte[current_pos.getPosX()][current_pos.getPosY()]; _carte[current_pos.getPosX()][current_pos.getPosY()] = nullptr; continue; }
                    }
                }
            } else if (agent->getIsReanimation()){
                 if (agent->getState() > agent->getDureeReanimation()) { _nbPersonneReanimation--; agent->resetState(); /* updateStats("recovered"); */ }
                 else if (agent->getState() == 6 && randmt->genrand_real1() < _paramsInstance->getTauxMortRea()) {
                     _nbPersonneReanimation--; _ageOfDeadHumansDaily.push_back(agent->getAge()); _nbMorts++; delete _carte[current_pos.getPosX()][current_pos.getPosY()]; _carte[current_pos.getPosX()][current_pos.getPosY()] = nullptr; continue;
                 }
            } else if (agent->getState() > 0) { // Not confined, not hospital, not rea, but sick
                if (agent->getState() > dureeContaEff) { agent->resetState(); /* updateStats("recovered"); */ }
                else if (agent->getState() == (_paramsInstance->getDureeAsymptomatiqueContagieuse() + 1) ) { // MODIFIED LINE
                     float chanceDevenirSymptomatique = 1.0f - _timelineAsymptomatique[_iteration];
                     if (randmt->genrand_real1() < chanceDevenirSymptomatique) {
                        _nbCasCovidConnuTotal++; _ageOfSymptomaticDailyHuman.push_back(agent->getAge()); agent->toConfined();
                     }
                }
            }

            // Contamination call for COVID mode, if agent is still around and contagious
            if (_carte[current_pos.getPosX()][current_pos.getPosY()] == agent &&
                agent->isContagieux(currentModeSim, _paramsInstance) &&
                !agent->getIsConfined() && !agent->getIsHospital() && !agent->getIsReanimation() ) {
                contamination(current_pos.getPosX(), current_pos.getPosY(), current_pos.getPosX(), current_pos.getPosY());
            }
        }
    }

    // Rebuild dynamic lists from _carte state (especially for COVID mode stats/display)
    // This is simplified as the original moveHumanXXX methods would update _newHuman... lists.
    // If those methods are fully replaced by logic in nextIteration, these lists need careful handling.
    // For now, assuming _newHuman... lists are populated by the moveHumanXXX calls if in COVID mode.
    if (currentModeSim == "COVID") {
        _humanSafePositions = _newHumanSafePositions;
        _humanAsymptomatiquePositions.assign(_newCurrentHumanAsymptomatiquePositions.begin(), _newCurrentHumanAsymptomatiquePositions.end());
        _humanAsymptomatiquePositions.insert(_humanAsymptomatiquePositions.end(), _newNextHumanAsymptomatiquePositions.begin(), _newNextHumanAsymptomatiquePositions.end());
        _humanConfinedPositions = _newHumanConfinedPositions;
        _humanHospitalPositions = _newHumanHospitalPositions;
        _humanReanimationPositions = _newHumanReanimationPositions;
    } else {
        _humanSafePositions.clear(); _humanAsymptomatiquePositions.clear();
        _humanConfinedPositions.clear(); _humanHospitalPositions.clear(); _humanReanimationPositions.clear();
        for (int r_idx = 0; r_idx < current_grid_size; ++r_idx) {
            for (int c_idx = 0; c_idx < current_grid_size; ++c_idx) {
                if (_carte[r_idx][c_idx] != nullptr) {
                    Human* agent = _carte[r_idx][c_idx];
                    if (agent->getStatutGenerique() == StatutGenerique::S || agent->getStatutGenerique() == StatutGenerique::R)
                        _humanSafePositions.push_back(agent->getPosition()); // R also considered "safe" for this list
                    else if (agent->getStatutGenerique() == StatutGenerique::I || agent->getStatutGenerique() == StatutGenerique::E)
                        _humanAsymptomatiquePositions.push_back(agent->getPosition());
                }
            }
        }
    }

    if (currentModeSim == "COVID") {
        if(_nbNouveauxCasSemaine.size() >= 7) _nbNouveauxCasSemaine.pop_front();
        _nbNouveauxCasSemaine.push_back(_nbNouveauxCas);
    }

    logIterationSummary();
    _iteration++;
}

void World::startSimulation(SimulationParams * inSimulationParams) {
   if (!inSimulationParams) throw std::runtime_error("SimulationParams is null in startSimulation");
   if (!_paramsInstance) _paramsInstance = inSimulationParams;
   _modeSimulationInternal = _paramsInstance->getModeSimulation();
   _max_iterations = _paramsInstance->getNbIteration();
   _iteration = 0;

   initialize(inSimulationParams);

   for(int i = 0; i < _max_iterations; ++i) {
      if (((_iteration % 10 == 0 || _iteration == 0) && _iteration < 50) || _iteration % 50 == 0 || _iteration == _max_iterations -1 ) {
        std::cout << "World Sim Instance: Iteration " << _iteration + 1 << "/" << _max_iterations << " (Mode: " << _modeSimulationInternal << ")" << std::endl;
      }
      nextIteration();
   }
   if (_log && _logfile.is_open()) {
       _logfile << "CompteurRand:" << std::to_string(randmt->getCompteur()) << std::endl;
       _logfile << "NbHumainSimuDepart:" << std::to_string(_paramsInstance->getNbHumain()) << std::endl;
   }
   displayStats();
}

void World::initWorldParams(SimulationParams * inSimulationParams) {
    if (!inSimulationParams) throw std::runtime_error("Cannot initWorldParams with null SimulationParams");
    World::_paramsInstance = inSimulationParams;
    World::_modeSimulationInternalStatic = inSimulationParams->getModeSimulation();

   _size = inSimulationParams->getSize();
   // World::_nbIteration = inSimulationParams->getNbIteration(); // Static _nbIteration
   _tableTauxHospitalisationByAge = inSimulationParams->getTableTauxHospitalisationByAge();
   _tableTailleHopitaux = inSimulationParams->getTableTailleHopitaux();
   _facteurTailleHopitaux = inSimulationParams->getFacteurTailleHopitaux();
   _tauxMortRea = inSimulationParams->getTauxMortRea();
   _r0 = inSimulationParams->getR0();
   _nbPlaceHospital = inSimulationParams->getNbPlaceHospital();
   _nbPlaceReanimation = inSimulationParams->getNbPlaceReanimation();
   _pourcentAsymptomatique = inSimulationParams->getPourcentAsymptomatique();
   _tauxVaccination = inSimulationParams->getTauxVaccination();
   _timelineVaccination = inSimulationParams->getTimelineVaccination();
   _tauxDeChanceDeMourirHospitalFull = inSimulationParams->getTauxDeChanceDeMourirHospitalFull();
   _histogrammeContamination = inSimulationParams->getHistogrammeContamination();
   _tauxContaDistanceDeux = inSimulationParams->getTauxContaDistanceDeux();
   _tauxVaccinationRappel = inSimulationParams->getTauxVaccinationRappel();
   _timelineVaccinationRappel = inSimulationParams->getTimelineVaccinationRappel();
   _isVaccin = inSimulationParams->getIsVaccin();
   _nbHumain = inSimulationParams->getNbHumain();
   _nbMalade = inSimulationParams->getNbMalade();
   _nbHopitaux = inSimulationParams->getNbHopitaux();
   _tauxAugmentationContaminationHopitaux = inSimulationParams->getTauxAugmentationContaminationHopitaux();
   _tauxContaminationHopitaux = inSimulationParams->getTauxContaminationHopitaux();
   _nbSalleDeSport = inSimulationParams->getNbSalleDeSport();
   _tauxAugmentationContaminationGym = inSimulationParams->getTauxAugmentationContaminationGym();
   _tauxContaminationGym = inSimulationParams->getTauxContaminationGym();
   _nbBoiteDeNuit = inSimulationParams->getNbBoiteDeNuit();
   _tauxAugmentationContaminationClub = inSimulationParams->getTauxAugmentationContaminationClub();
   _tauxContaminationClub = inSimulationParams->getTauxContaminationClub();
   _nbMagasin = inSimulationParams->getNbMagasin();
   _tauxAugmentationContaminationShop = inSimulationParams->getTauxAugmentationContaminationShop();
   _tauxContaminationShop = inSimulationParams->getTauxContaminationShop();
   _nbRestaurant = inSimulationParams->getNbRestaurant();
   _tauxAugmentationContaminationRestaurant = inSimulationParams->getTauxAugmentationContaminationRestaurant();
   _tauxContaminationRestaurant = inSimulationParams->getTauxContaminationRestaurant();
   _isMedicament = inSimulationParams->getIsMedicament();
   _tauxProtectionReaMedicament = inSimulationParams->getTauxProtectionReaMedicament();
   _isMasqueTissu = inSimulationParams->getIsMasqueTissu();
   _tauxProtectionMasqueTissu = inSimulationParams->getTauxProtectionMasqueTissu();
   _isMasqueChir = inSimulationParams->getIsMasqueChir();
   _tauxProtectionMasqueChir = inSimulationParams->getTauxProtectionMasqueChir();
   _isMasqueFFP2 = inSimulationParams->getIsMasqueFFP2();
   _tauxProtectionMasqueFFP2 = inSimulationParams->getTauxProtectionMasqueFFP2();
   _isConfinement = inSimulationParams->getIsConfinement();
   _nbDeplacementReductionConfinement = inSimulationParams->getNbDeplacementReductionConfinement();
   _nbLimiteDistanceMaxConfinement = inSimulationParams->getNbLimiteDistanceMaxConfinement();
   _isDeplacementLimites = inSimulationParams->getIsDeplacementLimites();
   _nbDistanceMax = inSimulationParams->getNbDistanceMax();
   _isGelHydroalcolique = inSimulationParams->getIsGelHydroalcolique();
   _tauxProtectionTransmissionGelHydro = inSimulationParams->getTauxProtectionTransmissionGelHydro();
   _isTestCovid = inSimulationParams->getIsTestCovid();
   _tauxDeDivisionAsymptomatique = inSimulationParams->getTauxDeDivisionAsymptomatique();
   _isCouvreFeu = inSimulationParams->getIsCouvreFeu();
   _nbDeplacementReductionCouvreFeu = inSimulationParams->getNbDeplacementReductionCouvreFeu();
   _nbSuperContaminateur = inSimulationParams->getNbSuperContaminateur();
   _rayonSuperContaminateur = inSimulationParams->getRayonSuperContaminateur();
   _nbDeplacementJour = inSimulationParams->getNbDeplacementJour();
   _nbVariants = inSimulationParams->getNbVariants();
   _defaultVariantHistoConta = inSimulationParams->getDefaultVariantHistoConta();
   _variantsHistoConta = inSimulationParams->getVariantsHistoConta();
   _dureeVariants = inSimulationParams->getDureeVariants();
   _pourcentAsymptomatiqueVariants = inSimulationParams->getPourcentAsymptomatiqueVariants();
   _tableTauxHospitalisationByAgeVariants = inSimulationParams->getTableTauxHospitalisationByAgeVariants();
   _tailleVilles = inSimulationParams->getTailleVilles();
   _nbHabitants = inSimulationParams->getNbHabitants();
   _coordonneesX = inSimulationParams->getCoordonneesX();
   _coordonneesY = inSimulationParams->getCoordonneesY();
   _timelineMasqueTissu = inSimulationParams->getTimelineMasqueTissu();
   _timelineMasqueChir = inSimulationParams->getTimelineMasqueChir();
   _timelineMasqueFFP2 = inSimulationParams->getTimelineMasqueFFP2();
   _timelineGelHydroalcolique = inSimulationParams->getTimelineGelHydroalcolique();
   _timelineConfinement = inSimulationParams->getTimelineConfinement();
   _timelineCouvreFeu = inSimulationParams->getTimelineCouvreFeu();
   _timelineMedicament = inSimulationParams->getTimelineMedicament();
   _timelineDeplacementLimites = inSimulationParams->getTimelineDeplacementLimites();
   _tauxObesitePopulation = inSimulationParams->getTauxObesitePopulation();
   _tauxDiabetePopulation = inSimulationParams->getTauxDiabetePopulation();
}

void World::writeLog(const std::string& inMsg) {
   if (_log && _logfile.is_open()) {
       _logfile << inMsg << std::endl;
   }
}
void World::pause() { std::cout << "Hit <Return to continue" << std::endl; std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); }

void World::display() {
   std::cout << std::endl << std::endl << "Iteration: " << _iteration << "/" << _max_iterations << std::endl;
   std::cout << "*****************************************************************";
   int current_grid_size = _paramsInstance ? _paramsInstance->getSize() : World::_size;
   for(int row = 0; row < current_grid_size; row++) {
      std::cout << "   " << std::endl;
      for(int column = 0; column < current_grid_size; column++ ) {
         if(_carte[row][column] == nullptr) {
            std::cout << World::SYMBOL_EMPTY << "   " ;
         } else {
            if (_modeSimulationInternal == "GENERIQUE_SEIRS") {
                 Human* agent = _carte[row][column];
                 char symbol_to_print = '?';
                 switch(agent->getStatutGenerique()){
                    case StatutGenerique::S: symbol_to_print = 'S'; break;
                    case StatutGenerique::E: symbol_to_print = 'E'; break;
                    case StatutGenerique::I: symbol_to_print = 'I'; break;
                    case StatutGenerique::R: symbol_to_print = 'R'; break;
                 }
                 std::cout << symbol_to_print << "   ";
            } else {
                std::cout << _carte[row][column]->to_string_covid_symbol() << "   ";
            }
         }
      }
      std::cout << std::endl;
   }
}

void World::updateStats(const std::string& inState) {
   if (inState == "recovered") {
        if (_stats["contamined"] > 0) _stats["contamined"] -= 1;
        _stats["recovered"] += 1;
   } else if (inState == "dead") {
        // Note: _stats["contamined"] might not always be decremented if death can occur from other states.
        // For now, assume death comes from a contagious state or is handled by overall counts.
        _stats["dead"] += 1;
   } else if (inState == "contamined") {
        if (_stats["safe"] > 0) _stats["safe"] -= 1;
        _stats["contamined"] += 1;
   }
   // "safe" is better calculated as Total - (E+I+R+Dead) at the end of iteration if possible
}

void World::displayStats() const {
   std::cout << "***************STATISTICS (Iter: " << _iteration << "/" << _max_iterations << ")******************" << std::endl;
   long current_total_humain = _paramsInstance ? _paramsInstance->getNbHumain() : World::_nbHumain;

   if (_modeSimulationInternal == "GENERIQUE_SEIRS") {
        long s_gen = 0; long e_gen = 0; long i_gen = 0; long r_gen = 0;
        long vivants = 0;
        int current_grid_size = _paramsInstance ? _paramsInstance->getSize() : World::_size;
        for (int r_idx = 0; r_idx < current_grid_size; ++r_idx) {
            for (int c_idx = 0; c_idx < current_grid_size; ++c_idx) {
                if (_carte[r_idx][c_idx] != nullptr) {
                    vivants++; Human* agent = _carte[r_idx][c_idx];
                    switch (agent->getStatutGenerique()) {
                        case StatutGenerique::S: s_gen++; break;
                        case StatutGenerique::E: e_gen++; break;
                        case StatutGenerique::I: i_gen++; break;
                        case StatutGenerique::R: r_gen++; break;
                    }
                }
            }
        }
        std::cout << "--- GENERIC SEIRS Mode ---" << std::endl;
        std::cout << "S: " << s_gen << ", E: " << e_gen << ", I: " << i_gen << ", R: " << r_gen << std::endl;
        std::cout << "Nouveaux Cas (jour): " << _nbNouveauxCas << std::endl; // This needs to be updated for generic
        std::cout << "Total Vivants: " << vivants << std::endl;
        std::cout << "Total Morts (cumul, approx): " << (current_total_humain - vivants) << std::endl;
   } else {
       std::cout << "--- COVID Mode ---" << std::endl;
       std::cout <<"Safe : " << _stats.at("safe") << std::endl;
       std::cout <<"Contamined (total) : " << _stats.at("contamined") << std::endl;
       std::cout <<"  (Asymptomatique/Leger non-confine): " << _humanAsymptomatiquePositions.size() << std::endl;
       std::cout <<"  (Confine): " << _humanConfinedPositions.size() << std::endl;
       std::cout <<"  (Hospitalise): " << _nbPersonneHospital << std::endl;
       std::cout <<"  (Reanimation): " << _nbPersonneReanimation << std::endl;
       std::cout <<"Recovered : " << _stats.at("recovered") << std::endl;
       std::cout <<"Dead (total): " << _stats.at("dead") << std::endl;
       std::cout <<"Nouveaux Cas (jour): " << _nbNouveauxCas << std::endl;
       std::cout <<"Nouveaux Hospitalises (jour): " << _nbNouveauxHospitalisation << std::endl;
       std::cout <<"Nouveaux Reanimations (jour): " << _nbNouveauxReanimation << std::endl;
       std::cout <<"Nouveaux Morts (jour): " << _nbMorts << std::endl;
   }
   std::cout << "******************************************" << std::endl;
}

bool World::isValid(int r, int c) const {
    int current_grid_size = _paramsInstance ? _paramsInstance->getSize() : World::_size;
    return r >= 0 && c >= 0 && r < current_grid_size && c < current_grid_size;
}
bool World::isHuman(int r, int c) const {
   if(isValid(r,c) && _carte[r][c] != nullptr) {
      char symbol = _carte[r][c]->to_string_covid_symbol();
      return (symbol == Human::SYMBOL || symbol == Human::SYMBOL_SICK ||
              symbol == Human::SYMBOL_SICK_CONFINED || symbol == Human::SYMBOL_SICK_HOSPITAL ||
              symbol == Human::SYMBOL_SICK_REANIMATION);
   }
   return false;
}
bool World::isEmpty(int r, int c) const {
   return isValid(r,c) && _carte[r][c] == nullptr;
}

// Placeholder for static _paramsInstance definition if truly needed by other static methods.
// SimulationParams* World::_paramsInstance = nullptr;
// std::string World::_modeSimulationInternalStatic;
