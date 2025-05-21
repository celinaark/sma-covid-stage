#ifndef _SIMU_EPIDEMIO_CITY_
#define _SIMU_EPIDEMIO_CITY_

#include <ostream>
#include <iostream>

using namespace std;

class City
{

   private:
      int _coordX;
      int _coordY;
      int _taille;
      int _nbHab;

   public:
      
      City(int taille = -1,int nbHab = -1,int coordX = -1,int coordY = -1);
      
      int  getCoordX() const;
      int  getCoordY() const;
      int  getTaille() const;
      int  getNbHab()  const;
      void toString();
      
};


#endif