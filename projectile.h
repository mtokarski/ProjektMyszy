#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "object.h"

class Projectile : public Object
{
public:
    Projectile();
    void control();
    void physics();
    QRectF boundingRect() const; // wykorzystywane przez Qt do kolizji i rysowania
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    int lifetimer;
    static const double initialSpeed;// = 8.0;
    static const int initialTimer;// = 200;

    bool ProjectileHit();
    bool Hittable();
};

#endif // PROJECTILE_H
