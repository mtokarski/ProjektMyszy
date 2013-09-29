#include "object.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>

Object::Object()
    : speed(0),slide(0),rotvel(0), friction(0.9925), slidefriction (0.9850), rotfriction (0.9800), acc(0.15), slideacc(0.08), brake (0.10), rotacc (0.5)
    // domyślne wartości parametrów fizycznych. Dlaczego zapisujemy je tak a nie tak jak poniżej, wewnątrz konstruktora? Ten sposób nazywa się initialization list i jest bardziej wydajny: zmienne tworzone są od razu z zadanymi wartościami, w przeciwnym razie tworzony jest tymczasowy obiekt, który dopiero później jest przypisywany do zmiennej. Jest to też sposób inicjalizacji pól klasy które są const.
{
//    speed = 0;
//    slide = 0;
//    rot = 0;
//    friction = 0.9925;
//    slidefriction = 0.9850;
//    rotfriction = 0.9800;
//    acc = 0.15;
//    slideacc = 0.08;
//    brake = 0.10;
//    rotacc = 0.5;
}

bool Object::ProjectileHit()
{
    return false;
}

bool Object::Hittable()
{
    return true;
}

void Object::WasHit(int dmg) {}

QRectF Object::boundingRect() const
{
    return QRectF(-10,-10,20,20);
}


void Object::control()
{
    ;
}

void Object::physics()
{
        speed = speed * friction;         // zmniejszenie prędkości o tarcie
        rotvel = rotvel * rotfriction;          // prędkość obrotowa i tarcie obrotowe
        slide = slide * slidefriction;   // predkosc przesuwania na boki
        speed -= rotvel*slide*0.0175;    // zamieniana jest na przesuw w bok - i na odwrot
        slide += rotvel*speed*0.0175;    // dzieki temu mamy poslizg zamiast przyklejenia do asfaltu
                    // wspolczynnik 0.0175 pozwala na pełny obrót bez zmiany kierunku ruchu
}

void Object::step()
{
    this->setRotation(this->rotation() - rotvel);  // obróć
    this->setPos(mapToParent(slide,-speed) );  // przesun we wspolrzednych obiektu
}

void Object::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setBrush(Qt::gray);
    painter->drawRect(-10, -10, 20, 20);
}
