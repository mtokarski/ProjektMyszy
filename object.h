#ifndef OBJECT_H
#define OBJECT_H

#include <QGraphicsItem>

class Object : public QGraphicsItem     // podstawowa klasa, po której będą dziedziczyć elementy silnika
{
public:
    Object();
    QRectF boundingRect() const; // prostokąt okalający obiekt

    // funkcje wirtualne - domyślnie wykonywane będą funkcje klas potomnych
    virtual void control();    // funkcja sterująca obiektem - domyślnie pusta
    virtual void physics();    // funkcja fizyki - domyślnie pełna fizyka
    virtual void step();       // krok aktualizacji fizyki na scenie

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    double speed;           // predkosc do przodu wzgledem obiektu
    double slide;           // predkosc w bok wzgledem obiektu
    double rotvel;          // predkosc obrotowa (Rotational Velocity)

    double friction;        // wspolczynnik tarcia do przodu (blizej 1 to lepszy poslizg)
    double slidefriction;   // na boki
    float rotfriction;     // obrotowy

    double acc;             // przyspieszenie
    double brake;           // hamowanie
    double slideacc;        // przyspieszenie boczne
    double rotacc;          // przyspieszenie obrotowe

    virtual bool ProjectileHit();   // TRUE dla pocisków
    virtual bool Hittable();        // TRUE dla myszy - reagowanie na trafienia pociskami
    virtual void WasHit(int dmg);
    int ProjectileDMG;
    int health;
};

#endif // OBJECT_H
