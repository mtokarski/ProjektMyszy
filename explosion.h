#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "object.h"
#include "window.h"

class Explosion: public Object
{
public:
    Explosion();
    Explosion(int radius);

    int radius;
    int lifetimer;

    void control();
    void physics();
    void step();
    bool Hittable();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    int lifemax;
};

#endif // EXPLOSION_H
