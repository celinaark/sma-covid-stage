#ifndef _SIMU_EPIDEMIO_SIMULATIONPARAMS_
#define _SIMU_EPIDEMIO_SIMULATIONPARAMS_



#include <map>
#include <string>
#include <deque>
#include <vector>
#include <fstream>
#include <ostream>
#include <iostream>

using namespace std;

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
      deque<int>       _timelineVaccination;
      deque<int>       _timelineVaccinationRappel;
      std::vector<float> _histogrammeContamination;
      float            _pourcentAsymptomatique;
      float            _tauxDeChanceDeMourirHospitalFull;
      
      float            _tauxContaDistanceDeux;
      int              _nbDeplacementJour;
      
      int              _isMedicament;
      deque<int>       _timelineMedicament;
      float            _tauxProtectionReaMedicament;
      
      int              _isMasqueTissu;
      deque<int>       _timelineMasqueTissu;
      float            _tauxProtectionMasqueTissu;
      
      int              _isMasqueChir;
      deque<int>       _timelineMasqueChir;
      float            _tauxProtectionMasqueChir;
      
      int              _isMasqueFFP2;
      deque<int>       _timelineMasqueFFP2;
      float            _tauxProtectionMasqueFFP2;
      
      int              _isConfinement;
      deque<int>       _timelineConfinement;
      int              _nbDeplacementReductionConfinement;
      int              _nbLimiteDistanceMaxConfinement;
      
      int              _isDeplacementLimites;
      deque<int>       _timelineDeplacementLimites;
      int              _nbDistanceMax;
      
      int              _isGelHydroalcolique;
      deque<int>       _timelineGelHydroalcolique;
      float            _tauxProtectionTransmissionGelHydro;
      
      int              _isTestCovid;
      float            _tauxDeDivisionAsymptomatique;
      
      int              _isCouvreFeu;
      deque<int>       _timelineCouvreFeu;
      int              _nbDeplacementReductionCouvreFeu;
      
      int              _nbSuperContaminateur;
      int              _rayonSuperContaminateur;
      vector<int>      _tailleVilles;
      vector<int>      _nbHabitants;
      vector<int>      _coordonneesX;
      vector<int>      _coordonneesY;


      map<string, int> _mapStringValues;

   public:
      SimulationParams(char *, char *);
      ~SimulationParams();
      std::vector<float> getResistanceInfectionValuesByAge();
      std::vector<float> getMaxResistanceInjectionValuesByAge();
      std::vector<float> getMinResistanceInjectionValuesByAge();
      std::vector<float> getTableTauxHospitalisationByAge();
      std::vector<int> getTableTailleHopitaux();
      float       getFacteurTailleHopitaux();
      float       getTauxVaccination();
      int         getNbPlaceHospital();
      int         getNbPlaceReanimation();
      int         getR0();
      float       getTauxMortRea();
      int         getSize();
      int         getNbMalade();
      int         getNbHumain();
      int         getNbHopitaux();
      float       getTauxAugmentationContaminationHopitaux();
      float       getTauxContaminationHopitaux();
      int         getNbSalleDeSport();
      float       getTauxAugmentationContaminationGym();
      float       getTauxContaminationGym();
      int         getNbBoiteDeNuit();
      float       getTauxAugmentationContaminationClub();
      float       getTauxContaminationClub();
      int         getNbMagasin();
      float       getTauxAugmentationContaminationShop();
      float       getTauxContaminationShop();
      int         getNbRestaurant();
      float       getTauxAugmentationContaminationRestaurant();
      float       getTauxContaminationRestaurant();
      int         getNbIteration();
      int         getIsVaccin();
      std::vector<float> getHistogrammeContamination();
      float       getPourcentAsymptomatique();
      float       getTauxDeChanceDeMourirHospitalFull();
      float       getTauxContaDistanceDeux();
      float       getTauxVaccinationRappel();
      std::vector<float> getTauxDeProtectionHospVaccinByAge();
      std::vector<float> getTauxDeProtectionReaVaccinByAge();
      std::vector<float> getTauxDeProtectionHospVaccinRappelByAge();
      std::vector<float> getTauxDeProtectionReaVaccinRappelByAge();
      std::vector<float> getTauxDeProtectionHospInfectionByAge();
      std::vector<float> getTauxDeProtectionReaInfectionByAge();
      int         getIsMedicament();
      float       getTauxProtectionReaMedicament();
      int         getIsMasqueTissu();
      float       getTauxProtectionMasqueTissu();
      int         getIsMasqueChir();
      float       getTauxProtectionMasqueChir();
      int         getIsMasqueFFP2();
      float       getTauxProtectionMasqueFFP2();
      int         getIsConfinement();
      int         getNbDeplacementReductionConfinement();
      int         getNbLimiteDistanceMaxConfinement();
      int         getIsDeplacementLimites();
      int         getNbDistanceMax();
      int         getIsGelHydroalcolique();
      float       getTauxProtectionTransmissionGelHydro();
      int         getIsTestCovid();
      float       getTauxDeDivisionAsymptomatique();
      int         getIsCouvreFeu();
      int         getNbDeplacementReductionCouvreFeu();
      int         getNbSuperContaminateur();
      int         getRayonSuperContaminateur();
      std::vector<float> getProbasCumulativesTrancheAge();
      int         getNbDeplacementJour();
      int         getNbVariants();
      std::vector<float> getDefaultVariantHistoConta();
      std::vector<float> getVariantsHistoConta();
      std::vector<int>   getDureeVariants();
      std::vector<float> getPourcentAsymptomatiqueVariants();
      std::vector<float> getTableTauxHospitalisationByAgeVariants();
      deque<int>  getTimelineVaccination();
      deque<int>  getTimelineVaccinationRappel();
      deque<int>  getTimelineMasqueTissu();
      deque<int>  getTimelineMasqueChir();
      deque<int>  getTimelineMasqueFFP2();
      deque<int>  getTimelineGelHydroalcolique();
      deque<int>  getTimelineConfinement();
      deque<int>  getTimelineCouvreFeu();
      deque<int>  getTimelineMedicament();
      deque<int>  getTimelineDeplacementLimites();
      float       getTauxObesitePopulation();
      float       getTauxAugmentationReaObesite();
      float       getTauxAugmentationHospObesite();
      float       getTauxDiabetePopulation();
      float       getTauxAugmentationReaDiabete();
      float       getTauxAugmentationHospDiabete();
      vector<int> getTailleVilles();
      vector<int> getNbHabitants();
      vector<int> getCoordonneesX();
      vector<int> getCoordonneesY();

};

#endif