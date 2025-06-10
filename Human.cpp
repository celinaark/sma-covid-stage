#include "Human.hpp"
// SimulationParams.hpp est inclus via Human.hpp pour le type SimulationParams*
// RandMT.hpp est inclus via Human.hpp pour le type RandMT*
// City.hpp et Position.hpp sont inclus via Human.hpp
#include <cmath>     // Pour std::exp, std::log
#include <string>    // Pour std::string
#include <vector>    // Pour std::vector (utilisé indirectement)
#include <iostream>  // Pour std::cerr (débogage)
#include <stdexcept> // Pour std::runtime_error

// Définition des membres statiques (copié de la version utilisateur)
std::vector<float> Human::_resistanceInfectionValuesByAge;
std::vector<float> Human::_maxResistanceInjectionValuesByAge;
std::vector<float> Human::_minResistanceInjectionValuesByAge;
std::vector<float> Human::_tauxDeProtectionHospVaccinByAge;
std::vector<float> Human::_tauxDeProtectionReaVaccinByAge;
std::vector<float> Human::_tauxDeProtectionHospVaccinRappelByAge;
std::vector<float> Human::_tauxDeProtectionReaVaccinRappelByAge;
std::vector<float> Human::_tauxDeProtectionHospInfectionByAge;
std::vector<float> Human::_tauxDeProtectionReaInfectionByAge;
std::vector<float> Human::_probasCumulativesTrancheAge;
float Human::_tauxAugmentationReaObesite;
float Human::_tauxAugmentationHospObesite;
float Human::_tauxAugmentationReaDiabete;
float Human::_tauxAugmentationHospDiabete;

// extern RandMT * randmt; // Déjà déclaré dans Human.hpp, pas besoin de le redéfinir ici.

// --- Constructeur ---
// REFACTORING: Initialisation des nouveaux membres pour le mode générique.
Human::Human(SimulationParams* inSimulationParams, int inRow, int inColumn)
    : _pos(inRow, inColumn), // Initialisation de _pos
      _posDebutTour(inRow, inColumn), // Initialisation de _posDebutTour
      _state(0),             // État COVID: jours de maladie, initialisé à 0 (sain)
      _symbol(Human::SYMBOL),// Symbole par défaut pour un agent sain
      _age(0),               // Sera déterminé ci-dessous
      _sexe(false),            // Sera déterminé ci-dessous
      _isConfined(false),
      _isHospital(false),
      _isReanimation(false),
      _isSuperContaminateur(false),
      _tauxDeProtectionInfection(0.0f),
      _tauxDeProtectionHospitalisation(0.0f),
      _tauxDeProtectionReanimation(0.0f),
      _newTauxDeProtectionInfection(0.0f),
      _newTauxDeProtectionHospitalisation(0.0f),
      _newTauxDeProtectionReanimation(0.0f),
      _dureeReanimation(0),
      _daysSinceLastInfectionOrInjection(0),
      _numberOfInfections(0),
      _numberOfInjections(0),
      _isObese(false),
      _isDiabete(false),
      _tauxComorbiditeRea(0.0f),
      _tauxComorbiditeHosp(0.0f),
      _ville(nullptr), // Sera défini par setVille si nécessaire
      // REFACTORING: Initialisation des membres du mode générique.
      _statut_generique_actuel(StatutGenerique::S), // Par défaut Susceptible
      _temps_en_statut_generique(0)
{
   if (!inSimulationParams) { // Vérification ajoutée
        throw std::runtime_error("SimulationParams est nul dans le constructeur de Human.");
   }
   if (!randmt) { // randmt est extern, il doit être initialisé ailleurs
        throw std::runtime_error("randmt (PRNG global) est nul dans le constructeur de Human.");
   }

   _symbol = Human::SYMBOL; // Assurer l'initialisation du symbole
   float randAge = randmt->genrand_real1();

   // Logique pour _age (reprise de la version utilisateur)
   // Utiliser les getters de inSimulationParams pour _probasCumulativesTrancheAge
   const std::vector<float>& probasAge = Human::_probasCumulativesTrancheAge; // Utiliser le membre statique initialisé

   if (!probasAge.empty()) {
       for(size_t i = 0; i < probasAge.size(); ++i) { // Utiliser size_t pour la comparaison avec .size()
          if(randAge <= probasAge[i]) {
             _age = static_cast<int>(i);
             break;
          }
          // Si randAge est plus grand que toutes les probas (la dernière devrait être 1.0)
          if (i == probasAge.size() - 1) {
              _age = static_cast<int>(i);
          }
       }
   } else {
       // std::cerr << "Avertissement: _probasCumulativesTrancheAge (statique) est vide. Age assigné aléatoirement." << std::endl;
       _age = randmt->genrand_int31() % 8; // Fallback si le vecteur est vide
   }

   _sexe = (randmt->genrand_real1() < 0.5);
}

// --- Destructeur ---
Human::~Human() {
    // Rien à libérer manuellement ici si Human ne possède pas directement de ressources allouées avec new/malloc.
    // _ville est un pointeur externe.
}

// --- Getters existants (inchangés par rapport à la version utilisateur) ---
int Human::getState() const { return _state; }
int Human::getAge() const { return _age; }
float Human::getTauxDeProtectionInfection() const { return _tauxDeProtectionInfection; }
bool Human::getIsConfined() const { return _isConfined; }
bool Human::getIsHospital() const { return _isHospital; }
bool Human::getIsReanimation() const { return _isReanimation; }
int Human::getDureeReanimation() const { return _dureeReanimation; }
Position Human::getPosition() const { return _pos; }
Position Human::getPositionDebutTour() const { return _posDebutTour; }
City* Human::getVille() const { return _ville; }
float Human::getTauxDeProtectionHospitalisation() const { return _tauxDeProtectionHospitalisation; }
float Human::getTauxDeProtectionReanimation() const { return _tauxDeProtectionReanimation; }
int Human::getNumberOfInfections() const { return _numberOfInfections; }
int Human::getNumberOfInjections() const { return _numberOfInjections; }
bool Human::getIsSuperContaminateur() const { return _isSuperContaminateur; }
float Human::getTauxComorbiditeRea() const { return _tauxComorbiditeRea; }
float Human::getTauxComorbiditeHosp() const { return _tauxComorbiditeHosp; }

// --- Setters existants (inchangés par rapport à la version utilisateur) ---
void Human::setIsObese() { _isObese = true; _tauxComorbiditeRea += _tauxAugmentationReaObesite; _tauxComorbiditeHosp += _tauxAugmentationHospObesite; }
void Human::setIsDiabete() { _isDiabete = true; _tauxComorbiditeRea += _tauxAugmentationReaDiabete; _tauxComorbiditeHosp += _tauxAugmentationHospDiabete; }
void Human::setIsSuperContaminateur(bool isSuperContaminateur) { _isSuperContaminateur = isSuperContaminateur; }
void Human::setVille(City* ville) { _ville = ville; }
void Human::setPosition(int inPosX,int inPosY) { _pos.setPosX(inPosX); _pos.setPosY(inPosY); }
void Human::setPositionDebutTour(int inPosX,int inPosY) { _posDebutTour.setPosX(inPosX); _posDebutTour.setPosY(inPosY); }

// --- Méthodes COVID spécifiques (inchangées dans leur logique interne, mais leur appel sera conditionné) ---
void Human::updateHumanProtection() { _daysSinceLastInfectionOrInjection++; if(_daysSinceLastInfectionOrInjection > 10){if(_newTauxDeProtectionInfection > 0){_tauxDeProtectionInfection = _newTauxDeProtectionInfection; _newTauxDeProtectionInfection = 0;} if(_newTauxDeProtectionHospitalisation > 0){_tauxDeProtectionHospitalisation = _newTauxDeProtectionHospitalisation; _newTauxDeProtectionHospitalisation = 0;} if(_newTauxDeProtectionReanimation > 0){_tauxDeProtectionReanimation = _newTauxDeProtectionReanimation; _newTauxDeProtectionReanimation = 0;}}}
void Human::goToHospital() { _isConfined = false; _isHospital = true; _isReanimation = false; }
void Human::goToReanimation() { _isConfined = false; _isHospital = false; _isReanimation = true; if(randmt) _dureeReanimation = (randmt->genrand_int32()%26) + 10;}
void Human::decreaseResistance() {  _daysSinceLastInfectionOrInjection++; if(_daysSinceLastInfectionOrInjection < 90) return; if(_daysSinceLastInfectionOrInjection >= 90 && _tauxDeProtectionInfection > 0) _tauxDeProtectionInfection -= 0.004f; if(_daysSinceLastInfectionOrInjection >= 90 && _tauxDeProtectionHospitalisation > 0) _tauxDeProtectionHospitalisation -= 0.002f; if(_daysSinceLastInfectionOrInjection >= 90 && _tauxDeProtectionReanimation > 0) _tauxDeProtectionReanimation -= 0.001f;}
void Human::resetState() { _state = 0; _isConfined = false; _isHospital = false; _isReanimation = false; }
void Human::incrementState() { _state += 1; }
void Human::contamine() { _state = 1; _numberOfInfections++; if(static_cast<size_t>(_age) < _resistanceInfectionValuesByAge.size() && _tauxDeProtectionInfection < _resistanceInfectionValuesByAge[_age]) _newTauxDeProtectionInfection = _resistanceInfectionValuesByAge[_age]; if(static_cast<size_t>(_age) < _tauxDeProtectionHospInfectionByAge.size() && _tauxDeProtectionHospitalisation < _tauxDeProtectionHospInfectionByAge[_age]) _newTauxDeProtectionHospitalisation = _tauxDeProtectionHospInfectionByAge[_age]; if(static_cast<size_t>(_age) < _tauxDeProtectionReaInfectionByAge.size() && _tauxDeProtectionReanimation < _tauxDeProtectionReaInfectionByAge[_age]) _newTauxDeProtectionReanimation = _tauxDeProtectionReaInfectionByAge[_age]; _daysSinceLastInfectionOrInjection = 0;}
void Human::vaccine() { _numberOfInjections++; double randomValue = 0.0; if (static_cast<size_t>(_age) < _minResistanceInjectionValuesByAge.size() && static_cast<size_t>(_age) < _maxResistanceInjectionValuesByAge.size()) randomValue = _minResistanceInjectionValuesByAge[_age] + (_maxResistanceInjectionValuesByAge[_age] - _minResistanceInjectionValuesByAge[_age]) * randmt->genrand_real1(); if(_tauxDeProtectionInfection < randomValue) _tauxDeProtectionInfection = randomValue; if(static_cast<size_t>(_age) < _tauxDeProtectionHospVaccinByAge.size() && _tauxDeProtectionHospitalisation < _tauxDeProtectionHospVaccinByAge[_age]) _tauxDeProtectionHospitalisation = _tauxDeProtectionHospVaccinByAge[_age]; if(static_cast<size_t>(_age) < _tauxDeProtectionReaVaccinByAge.size() && _tauxDeProtectionReanimation < _tauxDeProtectionReaVaccinByAge[_age]) _tauxDeProtectionReanimation = _tauxDeProtectionReaVaccinByAge[_age]; _daysSinceLastInfectionOrInjection = 0;}
void Human::vaccineRappel() { double randomValue = 0.0; if (static_cast<size_t>(_age) < _minResistanceInjectionValuesByAge.size() && static_cast<size_t>(_age) < _maxResistanceInjectionValuesByAge.size()) randomValue = _minResistanceInjectionValuesByAge[_age] + (_maxResistanceInjectionValuesByAge[_age] - _minResistanceInjectionValuesByAge[_age]) * randmt->genrand_real1(); if(_tauxDeProtectionInfection < randomValue) _newTauxDeProtectionInfection = randomValue; if(static_cast<size_t>(_age) < _tauxDeProtectionHospVaccinRappelByAge.size() && _tauxDeProtectionHospitalisation < _tauxDeProtectionHospVaccinRappelByAge[_age]) _newTauxDeProtectionHospitalisation = _tauxDeProtectionHospVaccinRappelByAge[_age]; if(static_cast<size_t>(_age) < _tauxDeProtectionReaVaccinRappelByAge.size() && _tauxDeProtectionReanimation < _tauxDeProtectionReaVaccinRappelByAge[_age]) _newTauxDeProtectionReanimation = _tauxDeProtectionReaVaccinRappelByAge[_age]; _daysSinceLastInfectionOrInjection = 0;}
void Human::toConfined() { _isConfined = true; _isHospital = false; _isReanimation = false; }

// REFACTORING: Renommage de to_string() en to_string_covid_symbol()
char Human::to_string_covid_symbol() {
   if(_isConfined) return Human::SYMBOL_SICK_CONFINED;
   if(_isHospital) return Human::SYMBOL_SICK_HOSPITAL;
   if(_isReanimation) return Human::SYMBOL_SICK_REANIMATION;
   if (this->isSick("COVID")) {
      return Human::SYMBOL_SICK;
   }
   return _symbol;
}

// REFACTORING: Implémentation de isSick(mode)
bool Human::isSick(const std::string& modeSimulation) const {
    if (modeSimulation == "GENERIQUE_SEIRS") {
        return (_statut_generique_actuel == StatutGenerique::E || _statut_generique_actuel == StatutGenerique::I);
    } else { // Mode COVID (ou par défaut)
        return _state > 0;
    }
}

// REFACTORING: Implémentation de isContagieux(mode, params)
bool Human::isContagieux(const std::string& modeSimulation, const SimulationParams* params) const {
    if (!params) {
        return false;
    }
    if (modeSimulation == "GENERIQUE_SEIRS") {
        return (_statut_generique_actuel == StatutGenerique::I);
    } else { // Mode COVID
        if (_state > 0) {
            const std::vector<float>* ptr_histoConta = nullptr;
            if (params->getNbVariants() > 0 && !params->getVariantsHistoConta().empty()) {
                 if(!params->getDefaultVariantHistoConta().empty() && params->getDefaultVariantHistoConta().size() > static_cast<size_t>(_state -1) && _state >0){
                    ptr_histoConta = &params->getDefaultVariantHistoConta();
                 }
            }
            if (!ptr_histoConta && !params->getHistogrammeContamination().empty() && params->getHistogrammeContamination().size() > static_cast<size_t>(_state-1) && _state > 0 ){
                 ptr_histoConta = &params->getHistogrammeContamination();
            }
            // Fallback to default if still null and default is available
             if (!ptr_histoConta && !params->getDefaultVariantHistoConta().empty() && params->getDefaultVariantHistoConta().size() > static_cast<size_t>(_state-1) && _state >0){
                 ptr_histoConta = &params->getDefaultVariantHistoConta();
            }

            if (ptr_histoConta && static_cast<size_t>(_state - 1) < ptr_histoConta->size() && _state > 0) { // Ensure _state-1 is valid index
                return ((*ptr_histoConta)[static_cast<size_t>(_state - 1)] > 0.000001f);
            }
        }
        return false;
    }
}

// REFACTORING: NOUVELLE MÉTHODE D'ÉVOLUTION QUOTIDIENNE PRINCIPALE
void Human::evoluerQuotidiennement(SimulationParams* params,
                                   Human*** carte, int taille_grille_x, int taille_grille_y,
                                   RandMT* prng) {
    if (!params) { throw std::runtime_error("SimulationParams est nul dans Human::evoluerQuotidiennement"); }
    if (!prng) { throw std::runtime_error("PRNG (randmt) est nul dans Human::evoluerQuotidiennement"); }

    if (params->getModeSimulation() == "GENERIQUE_SEIRS") {
        evoluerEtatGeneriqueSEIRS(params, carte, taille_grille_x, taille_grille_y, prng);
    } else {
        updateHumanProtection();
        if (getTauxDeProtectionInfection() > 0.0f || getTauxDeProtectionHospitalisation() > 0.0f || getTauxDeProtectionReanimation() > 0.0f) {
             decreaseResistance();
        }
        // En mode COVID, _state est incrémenté par World.cpp via les méthodes moveHumanXXX ou directement si besoin.
        // if (_state > 0) { // This was in the user's version, but incrementState is usually called by World
        //      Human::incrementState();
        // }
    }
}

// REFACTORING: NOUVELLES MÉTHODES PRIVÉES POUR LE MODE GÉNÉRIQUE SEIRS (avec getters renommés)
void Human::evoluerEtatGeneriqueSEIRS(SimulationParams* params,
                                      Human*** carte, int taille_grille_x, int taille_grille_y,
                                      RandMT* prng_pour_infection_check) {
    _temps_en_statut_generique++;
    StatutGenerique statut_avant_transition = _statut_generique_actuel;

    switch (statut_avant_transition) {
        case StatutGenerique::S: {
            float rand_val = prng_pour_infection_check->genrand_real1();
            verifierInfectionGeneriqueSEIRS(carte, taille_grille_x, taille_grille_y,
                                            params->getForceInfectionGenerique(), rand_val);
            break;
        }
        case StatutGenerique::E: {
            if (_temps_en_statut_generique >= params->getDureeIncubationGenerique()) { // RENAMED GETTER
                _statut_generique_actuel = StatutGenerique::I;
                _temps_en_statut_generique = 0;
            }
            break;
        }
        case StatutGenerique::I: {
            if (_temps_en_statut_generique >= params->getDureeContaminationGenerique()) { // RENAMED GETTER
                _statut_generique_actuel = StatutGenerique::R;
                _temps_en_statut_generique = 0;
            }
            break;
        }
        case StatutGenerique::R: {
            int dureeR = params->getDureeImmuniteGenerique(); // RENAMED GETTER
            if (dureeR > 0) {
                if (_temps_en_statut_generique >= dureeR) {
                    _statut_generique_actuel = StatutGenerique::S;
                    _temps_en_statut_generique = 0;
                }
            }
            break;
        }
    }
}

// REFACTORING: verifierInfectionGeneriqueSEIRS (logique interne inchangée, utilise Human*** carte)
void Human::verifierInfectionGeneriqueSEIRS(Human*** carte, int taille_grille_x, int taille_grille_y,
                                            float forceInfectionGenerique, float rand_val) {
    if (_statut_generique_actuel != StatutGenerique::S) return;

    int infectieux_voisins = 0;
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            int voisin_x = (_pos.getPosX() + dx + taille_grille_x) % taille_grille_x;
            int voisin_y = (_pos.getPosY() + dy + taille_grille_y) % taille_grille_y;

            Human* autre_agent_ptr = carte[voisin_x][voisin_y];

            if (autre_agent_ptr != nullptr && autre_agent_ptr != this) {
                if (autre_agent_ptr->getStatutGenerique() == StatutGenerique::I) {
                    infectieux_voisins++;
                }
            }
        }
    }

    if (infectieux_voisins > 0) {
        double probabilite_infection = 1.0 - std::exp(-static_cast<double>(forceInfectionGenerique) * infectieux_voisins);
        if (rand_val < probabilite_infection) {
            _statut_generique_actuel = StatutGenerique::E;
            _temps_en_statut_generique = 0;
        }
    }
}

// REFACTORING: NOUVEAUX GETTERS ET MÉTHODES POUR LE MODE GÉNÉRIQUE
StatutGenerique Human::getStatutGenerique() const {
    return _statut_generique_actuel;
}

std::string Human::statutGeneriqueToString() const {
    switch (_statut_generique_actuel) {
        case StatutGenerique::S: return "S_Gen";
        case StatutGenerique::E: return "E_Gen";
        case StatutGenerique::I: return "I_Gen";
        case StatutGenerique::R: return "R_Gen";
        default: return "?_Gen";
    }
}

// --- initHumanParams (statique) ---
// REFACTORING: S'assurer qu'il utilise bien les getters de l'objet params passé.
void Human::initHumanParams(SimulationParams * inSimulationParams) {
    if (!inSimulationParams) {
        throw std::runtime_error("Attempted to initialize Human static params with null SimulationParams in Human::initHumanParams");
    }
    _resistanceInfectionValuesByAge = inSimulationParams->getResistanceInfectionValuesByAge();
    _maxResistanceInjectionValuesByAge = inSimulationParams->getMaxResistanceInjectionValuesByAge();
    _minResistanceInjectionValuesByAge = inSimulationParams->getMinResistanceInjectionValuesByAge();
    _tauxDeProtectionHospVaccinByAge = inSimulationParams->getTauxDeProtectionHospVaccinByAge();
    _tauxDeProtectionReaVaccinByAge = inSimulationParams->getTauxDeProtectionReaVaccinByAge();
    _tauxDeProtectionHospVaccinRappelByAge = inSimulationParams->getTauxDeProtectionHospVaccinRappelByAge();
    _tauxDeProtectionReaVaccinRappelByAge = inSimulationParams->getTauxDeProtectionReaVaccinRappelByAge();
    _tauxDeProtectionHospInfectionByAge = inSimulationParams->getTauxDeProtectionHospInfectionByAge();
    _tauxDeProtectionReaInfectionByAge = inSimulationParams->getTauxDeProtectionReaInfectionByAge();
    _probasCumulativesTrancheAge = inSimulationParams->getProbasCumulativesTrancheAge();
    _tauxAugmentationReaObesite = inSimulationParams->getTauxAugmentationReaObesite();
    _tauxAugmentationHospObesite = inSimulationParams->getTauxAugmentationHospObesite();
    _tauxAugmentationReaDiabete = inSimulationParams->getTauxAugmentationReaDiabete();
    _tauxAugmentationHospDiabete = inSimulationParams->getTauxAugmentationHospDiabete();
}
