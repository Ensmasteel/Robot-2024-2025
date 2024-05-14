#ifndef GAMEELEMENT_H_
#define GAMEELEMENT_H_

#include "Vector.h"
#include "Arduino.h"

enum CouleurPalet : uint8_t{
    Marron,
    Rose,
    Jaune
};

enum CouleurBase : uint8_t{
    Vert,
    Bleu
};

struct Palet{
    Vector coordonnees;
    bool onTable = true;
    CouleurPalet couleur;
};

class Base{

    public:

        bool empty();
        bool full();
        void setCoordonnees(VectorOriented coor);
        void setNbGateau(uint8_t nbg);
        VectorOriented getCoordonnees();

    private :

        VectorOriented coordonnees;
        uint8_t nbGateau = 0;

};


class GameElement{

    public :

        void setup(CouleurBase coulB);

        Palet* nearestPaletJaune(VectorOriented posRobot);
        Palet* nearestPaletMarron(VectorOriented posRobot);
        Palet* nearestPaletRose(VectorOriented posRobot);
        Vector nearestPalet(VectorOriented posRobot, bool jaune, bool marron, bool rose);

        VectorOriented nearestBase(VectorOriented posRobot);

        Palet* getPaletInProgress();

        Base* getBaseInProgress();

        void printDebug(const String& prefix);

        CouleurPalet getCouleurPalet();

    private :

        Palet palets[12];
        CouleurBase couleurBase;
        Base bases[5];
        CouleurPalet couleurChosen;
        Base* baseInProgress;
        Palet* paletInProgress;
        Palet* nearestJaune;
        Palet* nearestRose;
        Palet* nearestMarron;
};

#endif