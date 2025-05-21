#ifndef _SIMU_EPIDEMIO_HUMAN_
#define _SIMU_EPIDEMIO_HUMAN_

#include <ostream>
#include <iostream>
#include <string>
#include <vector>

#include "Position.hpp"
#include "RandMT.hpp"
#include "SimulationParams.hpp"
#include "City.hpp"

extern RandMT * randmt;

class Human 
{
   private:
      
      static const char SYMBOL = 'X';
      static const char SYMBOL_SICK = 'O';
      static const char SYMBOL_SICK_CONFINED = 'C';
      static const char SYMBOL_SICK_HOSPITAL = 'H';
      static const char SYMBOL_SICK_REANIMATION = 'R';
      
      static std::vector<float> _resistanceInfectionValuesByAge;
      static std::vector<float> _maxResistanceInjectionValuesByAge;
      static std::vector<float> _minResistanceInjectionValuesByAge;
      static std::vector<float>  _tauxDeProtectionHospVaccinByAge;
      static std::vector<float>  _tauxDeProtectionReaVaccinByAge;
      static std::vector<float>  _tauxDeProtectionHospVaccinRappelByAge;
      static std::vector<float>  _tauxDeProtectionReaVaccinRappelByAge;
      static std::vector<float>  _tauxDeProtectionHospInfectionByAge;
      static std::vector<float>  _tauxDeProtectionReaInfectionByAge;
      static std::vector<float>  _probasCumulativesTrancheAge;
      
      static float      _tauxAugmentationReaObesite;
      static float      _tauxAugmentationHospObesite;
      static float      _tauxAugmentationReaDiabete;
      static float      _tauxAugmentationHospDiabete;
      
      int               _state = 0;
      char              _symbol;
      int               _age;
      bool               _sexe; //true homme et false femme
      bool              _isConfined = false;
      bool              _isHospital = false;
      bool              _isReanimation = false;
      bool              _isSuperContaminateur = false;
      float             _tauxDeProtectionInfection = 0;
      float             _tauxDeProtectionHospitalisation = 0;
      float             _tauxDeProtectionReanimation = 0;
      float             _newTauxDeProtectionInfection = 0;
      float             _newTauxDeProtectionHospitalisation = 0;
      float             _newTauxDeProtectionReanimation = 0;
      

      int               _dureeReanimation = 0;
      int               _daysSinceLastInfectionOrInjection = 0;

      int               _numberOfInfections = 0;
      int               _numberOfInjections = 0;
      bool              _isObese = false;
      bool              _isDiabete = false;
      float             _tauxComorbiditeRea = 0.0;
      float             _tauxComorbiditeHosp = 0.0;
      City *            _ville;
      Position          _pos;
      Position          _posDebutTour;

   public:
 
      Human(SimulationParams*,int,int);
      ~Human();

      int       getState() const;
      int       getAge() const;
      float     getTauxDeProtectionInfection() const;
      bool      getIsConfined() const;
      bool      getIsHospital() const;
      bool      getIsReanimation() const;
      int       getDureeReanimation() const;
      Position  getPosition() const;
      Position  getPositionDebutTour() const;
      City*     getVille() const;
      float     getTauxDeProtectionHospitalisation() const;
      float     getTauxDeProtectionReanimation() const;
      int       getNumberOfInfections() const;
      int       getNumberOfInjections() const;
      bool      getIsSuperContaminateur() const;
      float     getTauxComorbiditeRea() const;
      float     getTauxComorbiditeHosp() const;
      bool      isSick() const;
      
      void      setIsObese();
      void      setIsDiabete();
      void      setIsSuperContaminateur(bool);
      void      setVille(City*);
      void      setPosition(int,int);
      void      setPositionDebutTour(int,int);

      void      updateHumanProtection();
      void      goToHospital();
      void      goToReanimation();
      void      decreaseResistance();
      void      resetState();
      void      incrementState();
      char      to_string();
      void      contamine();
      void      vaccine();
      void      vaccineRappel();
      void      toConfined() ;

      static void initHumanParams(SimulationParams *);
};


#endif