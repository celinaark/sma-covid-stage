#ifndef _SIMU_EPIDEMIO_POSITION_
#define _SIMU_EPIDEMIO_POSITION_


#include <string>
using namespace std;

class Position
{

   private:
      int _posX;
      int _posY;

   public:

      // I don't know what i'm doing with friend, mais il faut definir operator == en dehors de la classe.
      friend bool operator==(Position const& inA, Position const& inB)
      {
         //Redefinition du equals pour les positions
         if (inA._posX == inB._posX && inA._posY == inB._posY)
            return true;
         else
            return false;
      }
      
      Position(int posX = 0, int posY = 0);
      
      int   getPosX() const;
      int   getPosY() const;
      void  setPosX(int);
      void  setPosY(int);
      
};


#endif