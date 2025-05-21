#include "City.hpp"

City::City(int inTaille, int inNbHab, int inCoordX, int inCoordY): 
   _coordX(inCoordX), _coordY(inCoordY), _taille(inTaille), _nbHab(inNbHab) 
{}


int City::getCoordX() const
{
   return _coordX;
}

int City::getCoordY() const
{
   return _coordY;
}

int City::getTaille() const
{
   return _taille;
}

int City::getNbHab() const
{
   return _nbHab;
}

void City::toString()
{
    cout << "La ville a pour coord X : " << _coordX << " , coordY : " << _coordY << ", taille : " << _taille << " , nbHab : " << _nbHab << endl; 
}


