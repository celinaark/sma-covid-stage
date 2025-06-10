#include "SimulationParams.hpp"
#include "World.hpp"
#include "Human.hpp"
#include <stdexcept>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <fstream>
#include <iostream>

SimulationParams::SimulationParams(char * inFilenameCity, char * inFilenameConfigParams){

   // Default initializations
   _modeSimulation = "COVID";
   _dureeIncubation_generique = 3;
   _dureeContamination_generique = 7;
   _dureeImmunite_generique = 90;
   _forceInfectionGenerique = 0.1f;
   _dureeAsymptomatiqueContagieuse = 2;
   // NOUVEAU: Initialisation par défaut pour _dureeIncubationNonContagieuse
   _dureeIncubationNonContagieuse = 0; // Défaut à 0 (pas de phase d'incubation non contagieuse distincte)

   // --- _mapStringValues Initialization ---
   _mapStringValues["resistanceInfectionValuesByAge"] = 0;
   _mapStringValues["maxResistanceInjectionValuesByAge"] = 1;
   _mapStringValues["minResistanceInjectionValuesByAge"] = 2;
   _mapStringValues["tableTauxHospitalisationByAge"] = 3;
   _mapStringValues["r0"] = 7;
   _mapStringValues["tauxMortRea"] = 8;
   _mapStringValues["isVaccin"] = 14;
   _mapStringValues["histogrammeContamination"] = 15;
   _mapStringValues["pourcentAsymptomatique"] = 16;
   _mapStringValues["tauxDeChanceDeMourirHospitalFull"] = 17;
   _mapStringValues["tauxVaccination"] = 18;
   _mapStringValues["tauxContaDistanceDeux"] = 19;
   _mapStringValues["tauxVaccinationRappel"] = 20;
   _mapStringValues["tauxDeProtectionHospVaccinByAge"] = 21;
   _mapStringValues["tauxDeProtectionReaVaccinByAge"] = 22;
   _mapStringValues["tauxDeProtectionHospVaccinRappelByAge"] = 23;
   _mapStringValues["tauxDeProtectionReaVaccinRappelByAge"] = 24;
   _mapStringValues["tauxDeProtectionHospInfectionByAge"] = 25;
   _mapStringValues["tauxDeProtectionReaInfectionByAge"] = 26;
   _mapStringValues["isMedicament"] = 27;
   _mapStringValues["tauxProtectionReaMedicament"] = 28;
   _mapStringValues["isMasqueTissu"] = 29;
   _mapStringValues["tauxProtectionMasqueTissu"] = 30;
   _mapStringValues["isMasqueChir"] = 31;
   _mapStringValues["tauxProtectionMasqueChir"] = 32;
   _mapStringValues["isMasqueFFP2"] = 33;
   _mapStringValues["tauxProtectionMasqueFFP2"] = 34;
   _mapStringValues["isConfinement"] = 35;
   _mapStringValues["nbDeplacementReductionConfinement"] = 36;
   _mapStringValues["isDeplacementLimites"] = 37;
   _mapStringValues["nbDistanceMax"] = 38;
   _mapStringValues["isGelHydroalcolique"] = 39;
   _mapStringValues["tauxProtectionTransmissionGelHydro"] = 40;
   _mapStringValues["isTestCovid"] = 41;
   _mapStringValues["tauxDeDivisionAsymptomatique"] = 42;
   _mapStringValues["isCouvreFeu"] = 43;
   _mapStringValues["nbDeplacementReductionCouvreFeu"] = 44;
   _mapStringValues["nbSuperContaminateur"] = 45;
   _mapStringValues["rayonSuperContaminateur"] = 46;
   _mapStringValues["nbLimiteDistanceMaxConfinement"] = 48;
   _mapStringValues["tauxAugmentationContaminationGym"] = 51;
   _mapStringValues["tauxContaminationGym"] = 52;
   _mapStringValues["tauxAugmentationContaminationClub"] = 54;
   _mapStringValues["tauxContaminationClub"] = 55;
   _mapStringValues["tauxAugmentationContaminationShop"] = 57;
   _mapStringValues["tauxContaminationShop"] = 58;
   _mapStringValues["tauxAugmentationContaminationRestaurant"] = 60;
   _mapStringValues["tauxContaminationRestaurant"] = 61;
   _mapStringValues["tauxAugmentationContaminationHopitaux"] = 63;
   _mapStringValues["tauxContaminationHopitaux"] = 64;
   _mapStringValues["nbVariants"] = 67;
   _mapStringValues["defaultVariantHistoConta"] = 68;
   _mapStringValues["variantsHistoConta"] = 69;
   _mapStringValues["dureeVariants"] = 70;
   _mapStringValues["pourcentAsymptomatiqueVariants"] = 71;
   _mapStringValues["tableTauxHospitalisationByAgeVariants"] = 72;
   _mapStringValues["timelineVaccination"] = 73;
   _mapStringValues["timelineVaccinationRappel"] = 74;
   _mapStringValues["timelineMasqueTissu"] = 75;
   _mapStringValues["timelineMasqueChir"] = 76;
   _mapStringValues["timelineMasqueFFP2"] = 77;
   _mapStringValues["timelineGelHydroalcolique"] = 78;
   _mapStringValues["timelineConfinement"] = 79;
   _mapStringValues["timelineCouvreFeu"] = 80;
   _mapStringValues["timelineMedicament"] = 81;
   _mapStringValues["timelineDeplacementLimites"] = 82;
   _mapStringValues["tauxObesitePopulation"] = 83;
   _mapStringValues["tauxAugmentationReaObesite"] = 84;
   _mapStringValues["tauxAugmentationHospObesite"] = 85;
   _mapStringValues["tauxDiabetePopulation"] = 86;
   _mapStringValues["tauxAugmentationReaDiabete"] = 87;
   _mapStringValues["tauxAugmentationHospDiabete"] = 88;
   
   _mapStringValues["size"] = 1000;
   _mapStringValues["nbHumain"] = 1001;
   _mapStringValues["nbMalade"] = 1002;
   _mapStringValues["nbIteration"] = 1003;
   _mapStringValues["nbPlaceHospital"] = 1004;
   _mapStringValues["nbPlaceReanimation"] = 1005;
   _mapStringValues["nbDeplacementJour"] = 1006;
   _mapStringValues["probasCumulativesTrancheAge"] = 1007;
   _mapStringValues["nbHopitaux"] = 1008;
   _mapStringValues["tailleHopitauxMetreCarre"] = 1009;
   _mapStringValues["facteurTailleHopitaux"] = 1010;
   _mapStringValues["nbSalleDeSport"] = 1011;
   _mapStringValues["nbBoiteDeNuit"] = 1012;
   _mapStringValues["nbMagasin"] = 1013;
   _mapStringValues["nbRestaurant"] = 1014;
   _mapStringValues["tailleVille"] = 1015;
   _mapStringValues["nbHabitant"] = 1016;
   _mapStringValues["coordonneeX"] = 1017;
   _mapStringValues["coordonneeY"] = 1018;

   _mapStringValues["modeSimulation"] = 2000;
   _mapStringValues["dureeIncubationGenerique"] = 2001;
   _mapStringValues["dureeContaminationGenerique"] = 2002;
   _mapStringValues["dureeImmuniteGenerique"] = 2003;
   _mapStringValues["forceInfectionGenerique"] = 2004;
   _mapStringValues["dureeAsymptomatiqueContagieuse"] = 2005;
   _mapStringValues["dureeIncubationNonContagieuse"] = 2006;

   std::string   line;
   std::ifstream readFile(inFilenameConfigParams);
   size_t   pos = 0;
   std::string   delimiterName = "||";
   std::string   delimiterValues = ",";
   std::string   value_str;
   
   if (!readFile.is_open()) {
       std::cerr << "ERREUR CRITIQUE: Impossible d'ouvrir le fichier de configuration des paramètres: " << inFilenameConfigParams << std::endl;
       throw std::runtime_error(std::string("Impossible d'ouvrir le fichier de configuration (params): ") + inFilenameConfigParams);
   }

   while (getline(readFile, line)) {
      if(line.empty() || line[0] == '#') continue;
      size_t delimiterPos = line.find(delimiterName);
      if (delimiterPos == std::string::npos) continue;
      std::string paramName = line.substr(0, delimiterPos);
      std::string paramValues = line.substr(delimiterPos + delimiterName.length());
      paramName.erase(0, paramName.find_first_not_of(" 	\n\r\f\v"));
      paramName.erase(paramName.find_last_not_of(" 	\n\r\f\v") + 1);
      paramValues.erase(0, paramValues.find_first_not_of(" 	\n\r\f\v"));
      paramValues.erase(paramValues.find_last_not_of(" 	\n\r\f\v") + 1);
      if (_mapStringValues.find(paramName) == _mapStringValues.end()) continue;
      int paramId = _mapStringValues[paramName];
      if ((paramId >= 1000 && paramId < 2000)) continue;
      try {
          switch (paramId) {
            case 0: _resistanceInfectionValuesByAge.clear(); while ((pos = paramValues.find(delimiterValues)) != std::string::npos) { value_str = paramValues.substr(0, pos); if (!value_str.empty()) _resistanceInfectionValuesByAge.push_back(std::stof(value_str)); paramValues.erase(0, pos + delimiterValues.length()); } if (!paramValues.empty()) _resistanceInfectionValuesByAge.push_back(std::stof(paramValues)); break;
            case 1: _maxResistanceInjectionValuesByAge.clear(); while ((pos = paramValues.find(delimiterValues)) != std::string::npos) { value_str = paramValues.substr(0, pos); if (!value_str.empty()) _maxResistanceInjectionValuesByAge.push_back(std::stof(value_str)); paramValues.erase(0, pos + delimiterValues.length()); } if (!paramValues.empty()) _maxResistanceInjectionValuesByAge.push_back(std::stof(paramValues)); break;
            case 2: _minResistanceInjectionValuesByAge.clear(); while ((pos = paramValues.find(delimiterValues)) != std::string::npos) { value_str = paramValues.substr(0, pos); if (!value_str.empty()) _minResistanceInjectionValuesByAge.push_back(std::stof(value_str)); paramValues.erase(0, pos + delimiterValues.length()); } if (!paramValues.empty()) _minResistanceInjectionValuesByAge.push_back(std::stof(paramValues)); break;
            case 3: _tableTauxHospitalisationByAge.clear(); while ((pos = paramValues.find(delimiterValues)) != std::string::npos) { value_str = paramValues.substr(0, pos); if (!value_str.empty()) _tableTauxHospitalisationByAge.push_back(std::stof(value_str)); paramValues.erase(0, pos + delimiterValues.length()); } if (!paramValues.empty()) _tableTauxHospitalisationByAge.push_back(std::stof(paramValues)); break;
            case 7: _r0 = std::stof(paramValues); break;
            case 8: _tauxMortRea = std::stof(paramValues); break;
            case 14: _isVaccin = std::stoi(paramValues); break;
            case 15: _histogrammeContamination.clear(); while ((pos = paramValues.find(delimiterValues)) != std::string::npos) { value_str = paramValues.substr(0, pos); if (!value_str.empty()) _histogrammeContamination.push_back(std::stof(value_str)); paramValues.erase(0, pos + delimiterValues.length()); } if (!paramValues.empty()) _histogrammeContamination.push_back(std::stof(paramValues)); break;
            case 16: _pourcentAsymptomatique = std::stof(paramValues); break;
            case 17: _tauxDeChanceDeMourirHospitalFull = std::stof(paramValues); break;
            case 18: _tauxVaccination = std::stof(paramValues); break;
            case 19: _tauxContaDistanceDeux = std::stof(paramValues); break;
            case 20: _tauxVaccinationRappel = std::stof(paramValues); break;
            case 21: _tauxDeProtectionHospVaccinByAge.clear(); while ((pos = paramValues.find(delimiterValues)) != std::string::npos) { value_str = paramValues.substr(0, pos); if (!value_str.empty()) _tauxDeProtectionHospVaccinByAge.push_back(std::stof(value_str)); paramValues.erase(0, pos + delimiterValues.length()); } if (!paramValues.empty()) _tauxDeProtectionHospVaccinByAge.push_back(std::stof(paramValues)); break;
            case 22: _tauxDeProtectionReaVaccinByAge.clear(); while ((pos = paramValues.find(delimiterValues)) != std::string::npos) { value_str = paramValues.substr(0, pos); if (!value_str.empty()) _tauxDeProtectionReaVaccinByAge.push_back(std::stof(value_str)); paramValues.erase(0, pos + delimiterValues.length()); } if (!paramValues.empty()) _tauxDeProtectionReaVaccinByAge.push_back(std::stof(paramValues)); break;
            case 23: _tauxDeProtectionHospVaccinRappelByAge.clear(); while ((pos = paramValues.find(delimiterValues)) != std::string::npos) { value_str = paramValues.substr(0, pos); if (!value_str.empty()) _tauxDeProtectionHospVaccinRappelByAge.push_back(std::stof(value_str)); paramValues.erase(0, pos + delimiterValues.length()); } if (!paramValues.empty()) _tauxDeProtectionHospVaccinRappelByAge.push_back(std::stof(paramValues)); break;
            case 24: _tauxDeProtectionReaVaccinRappelByAge.clear(); while ((pos = paramValues.find(delimiterValues)) != std::string::npos) { value_str = paramValues.substr(0, pos); if (!value_str.empty()) _tauxDeProtectionReaVaccinRappelByAge.push_back(std::stof(value_str)); paramValues.erase(0, pos + delimiterValues.length()); } if (!paramValues.empty()) _tauxDeProtectionReaVaccinRappelByAge.push_back(std::stof(paramValues)); break;
            case 25: _tauxDeProtectionHospInfectionByAge.clear(); while ((pos = paramValues.find(delimiterValues)) != std::string::npos) { value_str = paramValues.substr(0, pos); if (!value_str.empty()) _tauxDeProtectionHospInfectionByAge.push_back(std::stof(value_str)); paramValues.erase(0, pos + delimiterValues.length()); } if (!paramValues.empty()) _tauxDeProtectionHospInfectionByAge.push_back(std::stof(paramValues)); break;
            case 26: _tauxDeProtectionReaInfectionByAge.clear(); while ((pos = paramValues.find(delimiterValues)) != std::string::npos) { value_str = paramValues.substr(0, pos); if (!value_str.empty()) _tauxDeProtectionReaInfectionByAge.push_back(std::stof(value_str)); paramValues.erase(0, pos + delimiterValues.length()); } if (!paramValues.empty()) _tauxDeProtectionReaInfectionByAge.push_back(std::stof(paramValues)); break;
            case 27: _isMedicament = std::stoi(paramValues); break;
            case 28: _tauxProtectionReaMedicament = std::stof(paramValues); break;
            case 29: _isMasqueTissu = std::stoi(paramValues); break;
            case 30: _tauxProtectionMasqueTissu = std::stof(paramValues); break;
            case 31: _isMasqueChir = std::stoi(paramValues); break;
            case 32: _tauxProtectionMasqueChir = std::stof(paramValues); break;
            case 33: _isMasqueFFP2 = std::stoi(paramValues); break;
            case 34: _tauxProtectionMasqueFFP2 = std::stof(paramValues); break;
            case 35: _isConfinement = std::stoi(paramValues); break;
            case 36: _nbDeplacementReductionConfinement = std::stoi(paramValues); break;
            case 37: _isDeplacementLimites = std::stoi(paramValues); break;
            case 38: _nbDistanceMax = std::stoi(paramValues); break;
            case 39: _isGelHydroalcolique = std::stoi(paramValues); break;
            case 40: _tauxProtectionTransmissionGelHydro = std::stof(paramValues); break;
            case 41: _isTestCovid = std::stoi(paramValues); break;
            case 42: _tauxDeDivisionAsymptomatique = std::stof(paramValues); break;
            case 43: _isCouvreFeu = std::stoi(paramValues); break;
            case 44: _nbDeplacementReductionCouvreFeu = std::stoi(paramValues); break;
            case 45: _nbSuperContaminateur = std::stoi(paramValues); break;
            case 46: _rayonSuperContaminateur = std::stoi(paramValues); break;
            case 48: _nbLimiteDistanceMaxConfinement = std::stoi(paramValues); break;
            case 51: _tauxAugmentationContaminationGym = std::stof(paramValues); break;
            case 52: _tauxContaminationGym = std::stof(paramValues); break;
            case 54: _tauxAugmentationContaminationClub = std::stof(paramValues); break;
            case 55: _tauxContaminationClub = std::stof(paramValues); break;
            case 57: _tauxAugmentationContaminationShop = std::stof(paramValues); break;
            case 58: _tauxContaminationShop = std::stof(paramValues); break;
            case 60: _tauxAugmentationContaminationRestaurant = std::stof(paramValues); break;
            case 61: _tauxContaminationRestaurant = std::stof(paramValues); break;
            case 63: _tauxAugmentationContaminationHopitaux = std::stof(paramValues); break;
            case 64: _tauxContaminationHopitaux = std::stof(paramValues); break;
            case 67: _nbVariants = std::stoi(paramValues); break;
            case 68: _defaultVariantHistoConta.clear(); while ((pos = paramValues.find(delimiterValues)) != std::string::npos) { value_str = paramValues.substr(0, pos); if (!value_str.empty()) _defaultVariantHistoConta.push_back(std::stof(value_str)); paramValues.erase(0, pos + delimiterValues.length()); } if (!paramValues.empty()) _defaultVariantHistoConta.push_back(std::stof(paramValues)); break;
            case 69: _variantsHistoConta.clear(); while ((pos = paramValues.find(delimiterValues)) != std::string::npos) { value_str = paramValues.substr(0, pos); if (!value_str.empty()) _variantsHistoConta.push_back(std::stof(value_str)); paramValues.erase(0, pos + delimiterValues.length()); } if (!paramValues.empty()) _variantsHistoConta.push_back(std::stof(paramValues)); break;
            case 70: _dureeVariants.clear(); while ((pos = paramValues.find(delimiterValues)) != std::string::npos) { value_str = paramValues.substr(0, pos); if (!value_str.empty()) _dureeVariants.push_back(std::stoi(value_str)); paramValues.erase(0, pos + delimiterValues.length()); } if (!paramValues.empty()) _dureeVariants.push_back(std::stoi(paramValues)); break;
            case 71: _pourcentAsymptomatiqueVariants.clear(); while ((pos = paramValues.find(delimiterValues)) != std::string::npos) { value_str = paramValues.substr(0, pos); if (!value_str.empty()) _pourcentAsymptomatiqueVariants.push_back(std::stof(value_str)); paramValues.erase(0, pos + delimiterValues.length()); } if (!paramValues.empty()) _pourcentAsymptomatiqueVariants.push_back(std::stof(paramValues)); break;
            case 72: _tableTauxHospitalisationByAgeVariants.clear(); while ((pos = paramValues.find(delimiterValues)) != std::string::npos) { value_str = paramValues.substr(0, pos); if (!value_str.empty()) _tableTauxHospitalisationByAgeVariants.push_back(std::stof(value_str)); paramValues.erase(0, pos + delimiterValues.length()); } if (!paramValues.empty()) _tableTauxHospitalisationByAgeVariants.push_back(std::stof(paramValues)); break;
            case 73: _timelineVaccination.clear(); while((pos = paramValues.find(delimiterValues)) != std::string::npos){ value_str = paramValues.substr(0,pos); if(!value_str.empty()) _timelineVaccination.push_back(std::stoi(value_str)); paramValues.erase(0, pos + delimiterValues.length());} if(!paramValues.empty()) _timelineVaccination.push_back(std::stoi(paramValues)); break;
            case 74: _timelineVaccinationRappel.clear(); while((pos = paramValues.find(delimiterValues)) != std::string::npos){ value_str = paramValues.substr(0,pos); if(!value_str.empty()) _timelineVaccinationRappel.push_back(std::stoi(value_str)); paramValues.erase(0, pos + delimiterValues.length());} if(!paramValues.empty()) _timelineVaccinationRappel.push_back(std::stoi(paramValues)); break;
            case 75: _timelineMasqueTissu.clear(); while((pos = paramValues.find(delimiterValues)) != std::string::npos){ value_str = paramValues.substr(0,pos); if(!value_str.empty()) _timelineMasqueTissu.push_back(std::stoi(value_str)); paramValues.erase(0, pos + delimiterValues.length());} if(!paramValues.empty()) _timelineMasqueTissu.push_back(std::stoi(paramValues)); break;
            case 76: _timelineMasqueChir.clear(); while((pos = paramValues.find(delimiterValues)) != std::string::npos){ value_str = paramValues.substr(0,pos); if(!value_str.empty()) _timelineMasqueChir.push_back(std::stoi(value_str)); paramValues.erase(0, pos + delimiterValues.length());} if(!paramValues.empty()) _timelineMasqueChir.push_back(std::stoi(paramValues)); break;
            case 77: _timelineMasqueFFP2.clear(); while((pos = paramValues.find(delimiterValues)) != std::string::npos){ value_str = paramValues.substr(0,pos); if(!value_str.empty()) _timelineMasqueFFP2.push_back(std::stoi(value_str)); paramValues.erase(0, pos + delimiterValues.length());} if(!paramValues.empty()) _timelineMasqueFFP2.push_back(std::stoi(paramValues)); break;
            case 78: _timelineGelHydroalcolique.clear(); while((pos = paramValues.find(delimiterValues)) != std::string::npos){ value_str = paramValues.substr(0,pos); if(!value_str.empty()) _timelineGelHydroalcolique.push_back(std::stoi(value_str)); paramValues.erase(0, pos + delimiterValues.length());} if(!paramValues.empty()) _timelineGelHydroalcolique.push_back(std::stoi(paramValues)); break;
            case 79: _timelineConfinement.clear(); while((pos = paramValues.find(delimiterValues)) != std::string::npos){ value_str = paramValues.substr(0,pos); if(!value_str.empty()) _timelineConfinement.push_back(std::stoi(value_str)); paramValues.erase(0, pos + delimiterValues.length());} if(!paramValues.empty()) _timelineConfinement.push_back(std::stoi(paramValues)); break;
            case 80: _timelineCouvreFeu.clear(); while((pos = paramValues.find(delimiterValues)) != std::string::npos){ value_str = paramValues.substr(0,pos); if(!value_str.empty()) _timelineCouvreFeu.push_back(std::stoi(value_str)); paramValues.erase(0, pos + delimiterValues.length());} if(!paramValues.empty()) _timelineCouvreFeu.push_back(std::stoi(paramValues)); break;
            case 81: _timelineMedicament.clear(); while((pos = paramValues.find(delimiterValues)) != std::string::npos){ value_str = paramValues.substr(0,pos); if(!value_str.empty()) _timelineMedicament.push_back(std::stoi(value_str)); paramValues.erase(0, pos + delimiterValues.length());} if(!paramValues.empty()) _timelineMedicament.push_back(std::stoi(paramValues)); break;
            case 82: _timelineDeplacementLimites.clear(); while((pos = paramValues.find(delimiterValues)) != std::string::npos){ value_str = paramValues.substr(0,pos); if(!value_str.empty()) _timelineDeplacementLimites.push_back(std::stoi(value_str)); paramValues.erase(0, pos + delimiterValues.length());} if(!paramValues.empty()) _timelineDeplacementLimites.push_back(std::stoi(paramValues)); break;
            case 83: _tauxObesitePopulation = std::stof(paramValues); break;
            case 84: _tauxAugmentationReaObesite = std::stof(paramValues); break;
            case 85: _tauxAugmentationHospObesite = std::stof(paramValues); break;
            case 86: _tauxDiabetePopulation = std::stof(paramValues); break;
            case 87: _tauxAugmentationReaDiabete = std::stof(paramValues); break;
            case 88: _tauxAugmentationHospDiabete = std::stof(paramValues); break;
            
            case 2000: _modeSimulation = paramValues; break;
            case 2001: _dureeIncubation_generique = std::stoi(paramValues); break;
            case 2002: _dureeContamination_generique = std::stoi(paramValues); break;
            case 2003: _dureeImmunite_generique = std::stoi(paramValues); break;
            case 2004: _forceInfectionGenerique = std::stof(paramValues); break;
            case 2005: _dureeAsymptomatiqueContagieuse = std::stoi(paramValues); break;
            case 2006: _dureeIncubationNonContagieuse = std::stoi(paramValues); break;
            
            default: break;
          }
      } catch (const std::exception& e) {
          std::cerr << "AVERTISSEMENT: Exception lors du parsing du paramètre '" << paramName << "' avec valeur '" << paramValues << "': " << e.what() << std::endl;
      }
   }
   readFile.close();
   
   std::ifstream readFileCity(inFilenameCity);
   if (!readFileCity.is_open()) {
       std::cerr << "ERREUR CRITIQUE: Impossible d'ouvrir le fichier de configuration de la ville: " << inFilenameCity << std::endl;
       throw std::runtime_error(std::string("Impossible d'ouvrir le fichier de configuration (ville): ") + inFilenameCity);
   }
   pos = 0;
   while (getline(readFileCity, line)) {
      if(line.empty() || line[0] == '#') continue;
      size_t delimiterPos = line.find(delimiterName);
      if (delimiterPos == std::string::npos) continue;
      std::string paramName = line.substr(0, delimiterPos);
      std::string paramValues = line.substr(delimiterPos + delimiterName.length());
      paramName.erase(0, paramName.find_first_not_of(" 	\n\r\f\v"));
      paramName.erase(paramName.find_last_not_of(" 	\n\r\f\v") + 1);
      paramValues.erase(0, paramValues.find_first_not_of(" 	\n\r\f\v"));
      paramValues.erase(paramValues.find_last_not_of(" 	\n\r\f\v") + 1);
      if (_mapStringValues.find(paramName) == _mapStringValues.end()) continue;
      int paramId = _mapStringValues[paramName];
      if (paramId < 1000 || paramId >= 2000) continue;
      try {
          switch (paramId) {
            case 1000: _size = std::stoi(paramValues); break;
            case 1001: _nbHumain = std::stoi(paramValues); break;
            case 1002: _nbMalade = std::stoi(paramValues); break;
            case 1003: _nbIteration = std::stoi(paramValues); break;
            case 1004: _nbPlaceHospital = std::stoi(paramValues); break;
            case 1005: _nbPlaceReanimation = std::stoi(paramValues); break;
            case 1006: _nbDeplacementJour = std::stoi(paramValues); break;
            case 1007: _probasCumulativesTrancheAge.clear(); while ((pos = paramValues.find(delimiterValues)) != std::string::npos) { value_str = paramValues.substr(0, pos); if (!value_str.empty()) _probasCumulativesTrancheAge.push_back(std::stof(value_str)); paramValues.erase(0, pos + delimiterValues.length()); } if(!paramValues.empty()) _probasCumulativesTrancheAge.push_back(std::stof(paramValues)); break;
            case 1008: _nbHopitaux = std::stoi(paramValues); break;
            case 1009: _tableTailleHopitaux.clear(); while ((pos = paramValues.find(delimiterValues)) != std::string::npos) { value_str = paramValues.substr(0, pos); if (!value_str.empty()) _tableTailleHopitaux.push_back(std::stoi(value_str)); paramValues.erase(0, pos + delimiterValues.length()); } if(!paramValues.empty()) _tableTailleHopitaux.push_back(std::stoi(value_str)); break;
            case 1010: _facteurTailleHopitaux = std::stof(paramValues); break;
            case 1011: _nbSalleDeSport = std::stoi(paramValues); break;
            case 1012: _nbBoiteDeNuit = std::stoi(paramValues); break;
            case 1013: _nbMagasin = std::stoi(paramValues); break;
            case 1014: _nbRestaurant = std::stoi(paramValues); break;
            case 1015: _tailleVilles.clear(); while ((pos = paramValues.find(delimiterValues)) != std::string::npos) { value_str = paramValues.substr(0, pos); if (!value_str.empty()) _tailleVilles.push_back(std::stoi(value_str)); paramValues.erase(0, pos + delimiterValues.length()); } if (!paramValues.empty()) _tailleVilles.push_back(std::stoi(value_str)); break;
            case 1016: _nbHabitants.clear(); while ((pos = paramValues.find(delimiterValues)) != std::string::npos) { value_str = paramValues.substr(0, pos); if (!value_str.empty()) _nbHabitants.push_back(std::stoi(value_str)); paramValues.erase(0, pos + delimiterValues.length()); } if (!paramValues.empty()) _nbHabitants.push_back(std::stoi(value_str)); break;
            case 1017: _coordonneesX.clear(); while ((pos = paramValues.find(delimiterValues)) != std::string::npos) { value_str = paramValues.substr(0, pos); if (!value_str.empty()) _coordonneesX.push_back(std::stoi(value_str)); paramValues.erase(0, pos + delimiterValues.length()); } if (!paramValues.empty()) _coordonneesX.push_back(std::stoi(value_str)); break;
            case 1018: _coordonneesY.clear(); while ((pos = paramValues.find(delimiterValues)) != std::string::npos) { value_str = paramValues.substr(0, pos); if (!value_str.empty()) _coordonneesY.push_back(std::stoi(value_str)); paramValues.erase(0, pos + delimiterValues.length()); } if (!paramValues.empty()) _coordonneesY.push_back(std::stoi(value_str)); break;
            default: break;
          }
      } catch (const std::exception& e) {
           std::cerr << "AVERTISSEMENT: Exception lors du parsing du paramètre ville '" << paramName << "' avec valeur '" << paramValues << "': " << e.what() << std::endl;
      }
   }
   readFileCity.close();
}

SimulationParams::~SimulationParams(){}

std::vector<float> SimulationParams::getResistanceInfectionValuesByAge() const { return _resistanceInfectionValuesByAge; }
std::vector<float> SimulationParams::getMaxResistanceInjectionValuesByAge() const { return _maxResistanceInjectionValuesByAge; }
std::vector<float> SimulationParams::getMinResistanceInjectionValuesByAge() const { return _minResistanceInjectionValuesByAge; }
std::vector<float> SimulationParams::getTableTauxHospitalisationByAge() const { return _tableTauxHospitalisationByAge; }
std::vector<int> SimulationParams::getTableTailleHopitaux() const { return _tableTailleHopitaux; }
float SimulationParams::getFacteurTailleHopitaux() const { return _facteurTailleHopitaux; }
float SimulationParams::getTauxVaccination() const { return _tauxVaccination; }
int SimulationParams::getNbPlaceHospital() const { return _nbPlaceHospital; }
int SimulationParams::getNbPlaceReanimation() const { return _nbPlaceReanimation; }
float SimulationParams::getR0() const { return _r0; }
float SimulationParams::getTauxMortRea() const { return _tauxMortRea; }
int SimulationParams::getSize() const { return _size; }
int SimulationParams::getNbMalade() const { return _nbMalade; }
int SimulationParams::getNbHumain() const { return _nbHumain; }
int SimulationParams::getNbHopitaux() const { return _nbHopitaux; }
float SimulationParams::getTauxAugmentationContaminationHopitaux() const { return _tauxAugmentationContaminationHopitaux; }
float SimulationParams::getTauxContaminationHopitaux() const { return _tauxContaminationHopitaux; }
int SimulationParams::getNbSalleDeSport() const { return _nbSalleDeSport; }
float SimulationParams::getTauxAugmentationContaminationGym() const { return _tauxAugmentationContaminationGym; }
float SimulationParams::getTauxContaminationGym() const { return _tauxContaminationGym; }
int SimulationParams::getNbBoiteDeNuit() const { return _nbBoiteDeNuit; }
float SimulationParams::getTauxAugmentationContaminationClub() const { return _tauxAugmentationContaminationClub; }
float SimulationParams::getTauxContaminationClub() const { return _tauxContaminationClub; }
int SimulationParams::getNbMagasin() const { return _nbMagasin; }
float SimulationParams::getTauxAugmentationContaminationShop() const { return _tauxAugmentationContaminationShop; }
float SimulationParams::getTauxContaminationShop() const { return _tauxContaminationShop; }
int SimulationParams::getNbRestaurant() const { return _nbRestaurant; }
float SimulationParams::getTauxAugmentationContaminationRestaurant() const { return _tauxAugmentationContaminationRestaurant; }
float SimulationParams::getTauxContaminationRestaurant() const { return _tauxContaminationRestaurant; }
int SimulationParams::getNbIteration() const { return _nbIteration; }
int SimulationParams::getIsVaccin() const { return _isVaccin; }
std::vector<float> SimulationParams::getHistogrammeContamination() const { return _histogrammeContamination; }
float SimulationParams::getPourcentAsymptomatique() const { return _pourcentAsymptomatique; }
float SimulationParams::getTauxDeChanceDeMourirHospitalFull() const { return _tauxDeChanceDeMourirHospitalFull; }
float SimulationParams::getTauxContaDistanceDeux() const { return _tauxContaDistanceDeux; }
float SimulationParams::getTauxVaccinationRappel() const { return _tauxVaccinationRappel; }
std::vector<float> SimulationParams::getTauxDeProtectionHospVaccinByAge() const { return _tauxDeProtectionHospVaccinByAge; }
std::vector<float> SimulationParams::getTauxDeProtectionReaVaccinByAge() const { return _tauxDeProtectionReaVaccinByAge; }
std::vector<float> SimulationParams::getTauxDeProtectionHospVaccinRappelByAge() const { return _tauxDeProtectionHospVaccinRappelByAge; }
std::vector<float> SimulationParams::getTauxDeProtectionReaVaccinRappelByAge() const { return _tauxDeProtectionReaVaccinRappelByAge; }
std::vector<float> SimulationParams::getTauxDeProtectionHospInfectionByAge() const { return _tauxDeProtectionHospInfectionByAge; }
std::vector<float> SimulationParams::getTauxDeProtectionReaInfectionByAge() const { return _tauxDeProtectionReaInfectionByAge; }
int SimulationParams::getIsMedicament() const { return _isMedicament; }
float SimulationParams::getTauxProtectionReaMedicament() const { return _tauxProtectionReaMedicament; }
int SimulationParams::getIsMasqueTissu() const { return _isMasqueTissu; }
float SimulationParams::getTauxProtectionMasqueTissu() const { return _tauxProtectionMasqueTissu; }
int SimulationParams::getIsMasqueChir() const { return _isMasqueChir; }
float SimulationParams::getTauxProtectionMasqueChir() const { return _tauxProtectionMasqueChir; }
int SimulationParams::getIsMasqueFFP2() const { return _isMasqueFFP2; }
float SimulationParams::getTauxProtectionMasqueFFP2() const { return _tauxProtectionMasqueFFP2; }
int SimulationParams::getIsConfinement() const { return _isConfinement; }
int SimulationParams::getNbDeplacementReductionConfinement() const { return _nbDeplacementReductionConfinement; }
int SimulationParams::getNbLimiteDistanceMaxConfinement() const { return _nbLimiteDistanceMaxConfinement; }
int SimulationParams::getIsDeplacementLimites() const { return _isDeplacementLimites; }
int SimulationParams::getNbDistanceMax() const { return _nbDistanceMax; }
int SimulationParams::getIsGelHydroalcolique() const { return _isGelHydroalcolique; }
float SimulationParams::getTauxProtectionTransmissionGelHydro() const { return _tauxProtectionTransmissionGelHydro; }
int SimulationParams::getIsTestCovid() const { return _isTestCovid; }
float SimulationParams::getTauxDeDivisionAsymptomatique() const { return _tauxDeDivisionAsymptomatique; }
int SimulationParams::getIsCouvreFeu() const { return _isCouvreFeu; }
int SimulationParams::getNbDeplacementReductionCouvreFeu() const { return _nbDeplacementReductionCouvreFeu; }
int SimulationParams::getNbSuperContaminateur() const { return _nbSuperContaminateur; }
int SimulationParams::getRayonSuperContaminateur() const { return _rayonSuperContaminateur; }
std::vector<float> SimulationParams::getProbasCumulativesTrancheAge() const { return _probasCumulativesTrancheAge; }
int SimulationParams::getNbDeplacementJour() const { return _nbDeplacementJour; }
int SimulationParams::getNbVariants() const { return _nbVariants; }
std::vector<float> SimulationParams::getDefaultVariantHistoConta() const { return _defaultVariantHistoConta; }
std::vector<float> SimulationParams::getVariantsHistoConta() const { return _variantsHistoConta; }
std::vector<int> SimulationParams::getDureeVariants() const { return _dureeVariants; }
std::vector<float> SimulationParams::getPourcentAsymptomatiqueVariants() const { return _pourcentAsymptomatiqueVariants; }
std::vector<float> SimulationParams::getTableTauxHospitalisationByAgeVariants() const { return _tableTauxHospitalisationByAgeVariants; }
std::deque<int> SimulationParams::getTimelineVaccination() const { return _timelineVaccination; }
std::deque<int> SimulationParams::getTimelineVaccinationRappel() const { return _timelineVaccinationRappel; }
std::deque<int> SimulationParams::getTimelineMasqueTissu() const { return _timelineMasqueTissu; }
std::deque<int> SimulationParams::getTimelineMasqueChir() const { return _timelineMasqueChir; }
std::deque<int> SimulationParams::getTimelineMasqueFFP2() const { return _timelineMasqueFFP2; }
std::deque<int> SimulationParams::getTimelineGelHydroalcolique() const { return _timelineGelHydroalcolique; }
std::deque<int> SimulationParams::getTimelineConfinement() const { return _timelineConfinement; }
std::deque<int> SimulationParams::getTimelineCouvreFeu() const { return _timelineCouvreFeu; }
std::deque<int> SimulationParams::getTimelineMedicament() const { return _timelineMedicament; }
std::deque<int> SimulationParams::getTimelineDeplacementLimites() const { return _timelineDeplacementLimites; }
float SimulationParams::getTauxObesitePopulation() const { return _tauxObesitePopulation; }
float SimulationParams::getTauxAugmentationReaObesite() const { return _tauxAugmentationReaObesite; }
float SimulationParams::getTauxAugmentationHospObesite() const { return _tauxAugmentationHospObesite; }
float SimulationParams::getTauxDiabetePopulation() const { return _tauxDiabetePopulation; }
float SimulationParams::getTauxAugmentationReaDiabete() const { return _tauxAugmentationReaDiabete; }
float SimulationParams::getTauxAugmentationHospDiabete() const { return _tauxAugmentationHospDiabete; }
std::vector<int> SimulationParams::getTailleVilles() const { return _tailleVilles; }
std::vector<int> SimulationParams::getNbHabitants() const { return _nbHabitants; }
std::vector<int> SimulationParams::getCoordonneesX() const { return _coordonneesX; }
std::vector<int> SimulationParams::getCoordonneesY() const { return _coordonneesY; }

std::string SimulationParams::getModeSimulation() const { return _modeSimulation; }
int SimulationParams::getDureeIncubationGenerique() const { return _dureeIncubation_generique; }
int SimulationParams::getDureeContaminationGenerique() const { return _dureeContamination_generique; }
int SimulationParams::getDureeImmuniteGenerique() const { return _dureeImmunite_generique; }
float SimulationParams::getForceInfectionGenerique() const { return _forceInfectionGenerique; }
int SimulationParams::getDureeAsymptomatiqueContagieuse() const { return _dureeAsymptomatiqueContagieuse; }
int SimulationParams::getDureeIncubationNonContagieuse() const { return _dureeIncubationNonContagieuse; }
