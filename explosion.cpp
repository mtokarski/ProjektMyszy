#include "explosion.h"
#include "window.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>

Explosion::Explosion():
    radius(30), lifemax(50), lifetimer(lifemax)
{
    ;
}

Explosion::Explosion(int rad)
{
    this->radius = rad;
    lifemax = rad;
    lifetimer = lifemax;
}

void Explosion::control()
{
    lifetimer = lifetimer - 2;
    if(!lifetimer)
    {
        Window::kill.enqueue(this);
    }
}

void Explosion::physics() {}
void Explosion::step() {}
bool Explosion::Hittable() { return false; }

void Explosion::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setPen(Qt::NoPen);
    qreal currentRadius = radius*(lifemax-lifetimer)/(lifemax);
    QRadialGradient gradient(QPointF(0,0), currentRadius, QPointF(0,0));
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(0.2, QColor(255, 255, 100, 255));
    gradient.setColorAt(0.4, QColor(255, 80, 0, 200));
    gradient.setColorAt(1.0, QColor(255, 255, 255, 0));
    painter->setBrush(gradient);
    painter->drawEllipse(-currentRadius,-currentRadius,currentRadius*2,currentRadius*2);

    /*
    m_RedExplosion.setColorAt(0.2, QColor(255, 255, 100, 255));
    m_RedExplosion.setColorAt(0.4, QColor(255, 80, 0, 200));
    m_RedExplosion.setColorAt(1.0, QColor(255, 255, 255, 0));
    */
}
