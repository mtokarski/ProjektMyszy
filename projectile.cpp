#include "projectile.h"
#include "window.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>

Projectile::Projectile()
    : Object() , lifetimer(Projectile::initialTimer)
{
    ProjectileDMG = 4;// zamiast lifetimer=500
}

const double Projectile::initialSpeed = 8.0;
const int Projectile::initialTimer = 200;

QRectF Projectile::boundingRect() const
{
    return QRectF(-1,0,1,10);
}


void Projectile::control()
{
    lifetimer--;
    if(lifetimer == 0)
    {
        Window::kill.enqueue(this);
    }
}

void Projectile::physics()
{
    ;
}

bool Projectile::ProjectileHit()
{
    return true;
}

bool Projectile::Hittable()
{
    return false;
}


void Projectile::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QPen pen(Qt::red, 2.5, Qt::SolidLine, Qt::RoundCap);
    painter->setPen(pen);
    painter->drawLine(0, 0, 0, 7);
}
