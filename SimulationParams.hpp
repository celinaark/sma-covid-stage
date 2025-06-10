#ifndef _SIMU_EPIDEMIO_SIMULATIONPARAMS_
#define _SIMU_EPIDEMIO_SIMULATIONPARAMS_

#include <map>
#include <string>
#include <deque>
#include <vector>
#include <fstream>
#include <ostream>
#include <iostream>

class SimulationParams
{

private:
    // HUMAN
    std::vector<float> _resistanceInfectionValuesByAge;
    std::vector<float> _maxResistanceInjectionValuesByAge;
    std::vector<float> _minResistanceInjectionValuesByAge;
    std::vector<float> _tauxDeProtectionHospVaccinByAge;
    std::vector<float> _tauxDeProtectionReaVaccinByAge;
    std::vector<float> _tauxDeProtectionHospVaccinRappelByAge;
    std::vector<float> _tauxDeProtectionReaVaccinRappelByAge;
    std::vector<float> _tauxDeProtectionHospInfectionByAge;
    std::vector<float> _tauxDeProtectionReaInfectionByAge;
    std::vector<float> _probasCumulativesTrancheAge;
    float            _tauxAugmentationReaObesite;
    float            _tauxAugmentationHospObesite;
    float            _tauxAugmentationReaDiabete;
    float            _tauxAugmentationHospDiabete;

    //WORLD
    int              _nbVariants;
    float            _tauxObesitePopulation;
    float            _tauxDiabetePopulation;
    std::vector<float> _defaultVariantHistoConta;
    std::vector<float> _variantsHistoConta;
    std::vector<int>   _dureeVariants;
    std::vector<float> _pourcentAsymptomatiqueVariants;
    std::vector<float> _tableTauxHospitalisationByAgeVariants;
    std::vector<int>   _tableTailleHopitaux;
    float            _facteurTailleHopitaux;
    std::vector<float> _tableTauxHospitalisationByAge;
    int              _nbPlaceHospital;
    int              _nbPlaceReanimation;
    float            _r0;
    float            _tauxMortRea;
    int              _size;
    int              _nbMalade;
    int              _nbHumain;
    int              _nbHopitaux;
    float            _tauxAugmentationContaminationHopitaux;
    float            _tauxContaminationHopitaux;
    int              _nbSalleDeSport;
    float            _tauxAugmentationContaminationGym;
    float            _tauxContaminationGym;
    int              _nbBoiteDeNuit;
    float            _tauxAugmentationContaminationClub;
    float            _tauxContaminationClub;
    int              _nbMagasin;
    float            _tauxAugmentationContaminationShop;
    float            _tauxContaminationShop;
    int              _nbRestaurant;
    float            _tauxAugmentationContaminationRestaurant;
    float            _tauxContaminationRestaurant;
    int              _nbIteration;
    int              _isVaccin;
    float            _tauxVaccination;
    float            _tauxVaccinationRappel;
    std::deque<int>  _timelineVaccination;
    std::deque<int>  _timelineVaccinationRappel;
    std::vector<float> _histogrammeContamination;
    float            _pourcentAsymptomatique;
    float            _tauxDeChanceDeMourirHospitalFull;

    float            _tauxContaDistanceDeux;
    int              _nbDeplacementJour;

    int              _isMedicament;
    std::deque<int>  _timelineMedicament;
    float            _tauxProtectionReaMedicament;

    int              _isMasqueTissu;
    std::deque<int>  _timelineMasqueTissu;
    float            _tauxProtectionMasqueTissu;

    int              _isMasqueChir;
    std::deque<int>  _timelineMasqueChir;
    float            _tauxProtectionMasqueChir;

    int              _isMasqueFFP2;
    std::deque<int>  _timelineMasqueFFP2;
    float            _tauxProtectionMasqueFFP2;

    int              _isConfinement;
    std::deque<int>  _timelineConfinement;
    int              _nbDeplacementReductionConfinement;
    int              _nbLimiteDistanceMaxConfinement;

    int              _isDeplacementLimites;
    std::deque<int>  _timelineDeplacementLimites;
    int              _nbDistanceMax;

    int              _isGelHydroalcolique;
    std::deque<int>  _timelineGelHydroalcolique;
    float            _tauxProtectionTransmissionGelHydro;

    int              _isTestCovid;
    float            _tauxDeDivisionAsymptomatique;

    int              _isCouvreFeu;
    std::deque<int>  _timelineCouvreFeu;
    int              _nbDeplacementReductionCouvreFeu;

    int              _nbSuperContaminateur;
    int              _rayonSuperContaminateur;
    std::vector<int> _tailleVilles;
    std::vector<int> _nbHabitants;
    std::vector<int> _coordonneesX;
    std::vector<int> _coordonneesY;

    std::string      _modeSimulation;
    int              _dureeIncubation_generique;
    int              _dureeContamination_generique;
    int              _dureeImmunite_generique;
    float            _forceInfectionGenerique;
    int              _dureeAsymptomatiqueContagieuse;

    // NOUVEAU PARAMÈTRE
    int              _dureeIncubationNonContagieuse;

    std::map<std::string, int> _mapStringValues;

public:
    SimulationParams(char *, char *);
    ~SimulationParams();

    std::vector<float> getResistanceInfectionValuesByAge() const;
    std::vector<float> getMaxResistanceInjectionValuesByAge() const;
    std::vector<float> getMinResistanceInjectionValuesByAge() const;
    std::vector<float> getTableTauxHospitalisationByAge() const;
    std::vector<int>   getTableTailleHopitaux() const;
    float       getFacteurTailleHopitaux() const;
    float       getTauxVaccination() const;
    int         getNbPlaceHospital() const;
    int         getNbPlaceReanimation() const;
    float       getR0() const;
    float       getTauxMortRea() const;
    int         getSize() const;
    int         getNbMalade() const;
    int         getNbHumain() const;
    int         getNbHopitaux() const;
    float       getTauxAugmentationContaminationHopitaux() const;
    float       getTauxContaminationHopitaux() const;
    int         getNbSalleDeSport() const;
    float       getTauxAugmentationContaminationGym() const;
    float       getTauxContaminationGym() const;
    int         getNbBoiteDeNuit() const;
    float       getTauxAugmentationContaminationClub() const;
    float       getTauxContaminationClub() const;
    int         getNbMagasin() const;
    float       getTauxAugmentationContaminationShop() const;
    float       getTauxContaminationShop() const;
    int         getNbRestaurant() const;
    float       getTauxAugmentationContaminationRestaurant() const;
    float       getTauxContaminationRestaurant() const;
    int         getNbIteration() const;
    int         getIsVaccin() const;
    std::vector<float> getHistogrammeContamination() const;
    float       getPourcentAsymptomatique() const;
    float       getTauxDeChanceDeMourirHospitalFull() const;
    float       getTauxContaDistanceDeux() const;
    float       getTauxVaccinationRappel() const;
    std::vector<float> getTauxDeProtectionHospVaccinByAge() const;
    std::vector<float> getTauxDeProtectionReaVaccinByAge() const;
    std::vector<float> getTauxDeProtectionHospVaccinRappelByAge() const;
    std::vector<float> getTauxDeProtectionReaVaccinRappelByAge() const;
    std::vector<float> getTauxDeProtectionHospInfectionByAge() const;
    std::vector<float> getTauxDeProtectionReaInfectionByAge() const;
    int         getIsMedicament() const;
    float       getTauxProtectionReaMedicament() const;
    int         getIsMasqueTissu() const;
    float       getTauxProtectionMasqueTissu() const;
    int         getIsMasqueChir() const;
    float       getTauxProtectionMasqueChir() const;
    int         getIsMasqueFFP2() const;
    float       getTauxProtectionMasqueFFP2() const;
    int         getIsConfinement() const;
    int         getNbDeplacementReductionConfinement() const;
    int         getNbLimiteDistanceMaxConfinement() const;
    int         getIsDeplacementLimites() const;
    int         getNbDistanceMax() const;
    int         getIsGelHydroalcolique() const;
    float       getTauxProtectionTransmissionGelHydro() const;
    int         getIsTestCovid() const;
    float       getTauxDeDivisionAsymptomatique() const;
    int         getIsCouvreFeu() const;
    int         getNbDeplacementReductionCouvreFeu() const;
    int         getNbSuperContaminateur() const;
    int         getRayonSuperContaminateur() const;
    std::vector<float> getProbasCumulativesTrancheAge() const;
    int         getNbDeplacementJour() const;
    int         getNbVariants() const;
    std::vector<float> getDefaultVariantHistoConta() const;
    std::vector<float> getVariantsHistoConta() const;
    std::vector<int>   getDureeVariants() const;
    std::vector<float> getPourcentAsymptomatiqueVariants() const;
    std::vector<float> getTableTauxHospitalisationByAgeVariants() const;
    std::deque<int>  getTimelineVaccination() const;
    std::deque<int>  getTimelineVaccinationRappel() const;
    std::deque<int>  getTimelineMasqueTissu() const;
    std::deque<int>  getTimelineMasqueChir() const;
    std::deque<int>  getTimelineMasqueFFP2() const;
    std::deque<int>  getTimelineGelHydroalcolique() const;
    std::deque<int>  getTimelineConfinement() const;
    std::deque<int>  getTimelineCouvreFeu() const;
    std::deque<int>  getTimelineMedicament() const;
    std::deque<int>  getTimelineDeplacementLimites() const;
    float       getTauxObesitePopulation() const;
    float       getTauxAugmentationReaObesite() const;
    float       getTauxAugmentationHospObesite() const;
    float       getTauxDiabetePopulation() const;
    float       getTauxAugmentationReaDiabete() const;
    float       getTauxAugmentationHospDiabete() const;
    std::vector<int> getTailleVilles() const;
    std::vector<int> getNbHabitants() const;
    std::vector<int> getCoordonneesX() const;
    std::vector<int> getCoordonneesY() const;

    std::string getModeSimulation() const;
    int getDureeIncubationGenerique() const;
    int getDureeContaminationGenerique() const;
    int getDureeImmuniteGenerique() const;
    float getForceInfectionGenerique() const;
    int getDureeAsymptomatiqueContagieuse() const;

    // NOUVEAU GETTER
    int getDureeIncubationNonContagieuse() const;
};

#endif // _SIMU_EPIDEMIO_SIMULATIONPARAMS_
