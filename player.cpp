#include <QDebug>

#include "player.h"
#include "window.h"
#include "projectile.h"

Player::Player()
    : Object()
    // lista inicjalizacji zawiera wywołanie konstruktora, aby zainicjalizować domyślne wartości parametrów fizycznych. Rzecz jasna, konstruktor bezargumentowy i tak by się wywołał, ale to jest sposób na wywołanie kontruktora z argumentami.
{
    weaponTimer = 0;
    health = 150;
}

bool Player::Hittable()
{
    return false;
}

void Player::control()
{
    if(Window::Pressed_forward)
    {
        speed = speed + acc;
    }
    if(Window::Pressed_brake)
    {
        speed = speed - brake;
    }
    if(Window::Pressed_rotateLeft)
    {
        rotvel = rotvel + rotacc;
    }
    if(Window::Pressed_rotateRight)
    {
        rotvel = rotvel - rotacc;
    }
    if(Window::Pressed_strafeLeft)
    {
        slide = slide - slideacc;
    }
    if(Window::Pressed_strafeRight)
    {
        slide = slide + slideacc;
    }
    if(Window::Pressed_shoot)
    {
        if(weaponTimer==0)
        {
            Projectile* pro = new Projectile;
            pro->setPos(this->pos());
            pro->setRotation(this->rotation());
            pro->speed = Projectile::initialSpeed;
            Window::spawn.enqueue(pro);
            weaponTimer=9;
        }
    }
    if(weaponTimer>0) { weaponTimer--; }
}


void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    /*
    // Body
    painter->setBrush(Qt::black);
    painter->drawEllipse(-14, -20, 28, 40);

    // Eyes
    painter->setBrush(Qt::white);
    painter->drawEllipse(-10, -17, 8, 8);
    painter->drawEllipse(2, -17, 8, 8);

    // Nose
    painter->setBrush(Qt::black);
    painter->drawEllipse(QRectF(-2, -22, 4, 4));

    // Pupils
    painter->setBrush(Qt::red);
    painter->drawEllipse(QRectF(-8.0 , -17, 4, 4));
    painter->drawEllipse(QRectF(4.0 , -17, 4, 4));

    // Ears
    painter->setBrush(Qt::gray);
    painter->drawEllipse(-17, -12, 16, 16);
    painter->drawEllipse(1, -12, 16, 16);

    // Tail
    QPainterPath path(QPointF(0, 20));
    path.cubicTo(-5, 22, -5, 22, 0, 25);
    path.cubicTo(5, 27, 5, 32, 0, 30);
    path.cubicTo(-5, 32, -5, 42, 0, 35);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(path);
    */
    QPixmap ship(":/images/statek.png");
    painter->drawPixmap(-40, -40, 80, 80, ship);
}
