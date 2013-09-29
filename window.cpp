#include "window.h"
#include "mouse.h"
#include "object.h"
#include "player.h"
#include "explosion.h"

#include <QtOpenGL>
#include <sstream>

bool Window::Pressed_forward = FALSE;
bool Window::Pressed_rotateLeft = FALSE;
bool Window::Pressed_rotateRight = FALSE;
bool Window::Pressed_brake = FALSE;
bool Window::Pressed_shoot = FALSE;
bool Window::Pressed_strafeLeft = FALSE;
bool Window::Pressed_strafeRight = FALSE;
QQueue<Object*> Window::spawn;
QQueue<Object*> Window::kill;
int Window::mouseX;
int Window::mouseY;

int Window::score = 0;
bool Window::gameOver = FALSE;
int Window::mouse_count = 15;

Window::Window(QObject *parent)
{
    // jedyne miejsce gdzie wiążemy klawisze z akcjami
    actions.insert( Qt::Key_W, Forward );
    actions.insert( Qt::Key_Z, RotateLeft );
    actions.insert( Qt::Key_X, RotateRight );
    actions.insert( Qt::Key_E, Shoot );
    actions.insert( Qt::Key_S, Brake );
    actions.insert( Qt::Key_Escape, Escape );
    actions.insert( Qt::Key_A, StrafeLeft );
    actions.insert( Qt::Key_D, StrafeRight );

    this->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers))); // zmień viewport na OpenGL
    this->setRenderHint(QPainter::Antialiasing); // włącz antyaliasing
    this->setCacheMode(QGraphicsView::CacheBackground); // cacheowanie tła
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // bez scroolbarów
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate); // OpenGL nie umie przerysować części viewportu
    this->setDragMode(QGraphicsView::NoDrag); // bez przeciągania
    this->setCursor(Qt::CrossCursor); // kursor celownik
    this->setWindowTitle("Myszy v 4");
    this->setStyleSheet( "QGraphicsView { border-style: none; }" ); // usuwa ramkę w fullscreenie ALBO this->setFrameStyle(0);
    this->setMouseTracking(true); // Bez tego nie ma zdarzeń ruchu myszą!
    this->showFullScreen(); // wystarczy to zmienić aby był fullscreen
    this->background = QPixmap(":/images/Nebula1.png").scaled(this->width(),this->height(),Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    scene.setSceneRect(-5000, -5000, 10000, 10000);
    scene.setItemIndexMethod(QGraphicsScene::NoIndex);
    this->setScene(&scene); // przypisanie sceny do okna
    this->centerOn(0.0,0.0);
    player = new Player;
    scene.addItem(player);
    player->setPos(0.0, 0.0);
    for (int i = 0; i < mouse_count; ++i)
    {
        Mouse *mouse = new Mouse;
        mouse->setPos(::sin((i * 6.28) / mouse_count) * 300,
                      ::cos((i * 6.28) / mouse_count) * 200);
        scene.addItem(mouse);
    }
    this->timer.start(33); // zrób timer odpalający 30 razy na sekundę
    connect(&timer, SIGNAL(timeout()), this, SLOT(MainClockTick())); // podepnij timer do slotu (głównej pętli)
}

void Window::MainClockTick()
{
    if(player->health <= 0 || mouse_count == 0) { gameOver = TRUE; }
    QList<QGraphicsItem *> items = scene.items();
    foreach(QGraphicsItem *item, items)
    {
        Object *object = static_cast<Object *> (item);
        object->control();
        object->physics();
        object->step();

        if(object->Hittable())
        {
            if(player->collidesWithItem(object))
            {
                Explosion *explosion = new Explosion(50);
                explosion->setPos(player->scenePos());
                Window::spawn.enqueue(explosion);
                object->WasHit(100);
                player->health-=50;
                //Window::kill.enqueue(player);
                Window::kill.enqueue(object);
            }
        }

        if(object->ProjectileHit())
        {
            foreach(QGraphicsItem *item2, items)
            {
                Object *target = static_cast<Object *>(item2);
                if(target->Hittable())
                {
                    if(object->collidesWithItem(target))
                    {
                        target->WasHit(object->ProjectileDMG);
                        if(target->health <= 0) { score++; mouse_count--;}
                        Explosion *explosion = new Explosion(16);
                        explosion->setPos(object->scenePos());
                        Window::spawn.enqueue(explosion);
                        Window::kill.enqueue(object);
                    }
                }
            }
        }
    }

    // to wykonuje się bardzo wolno:
    //   this->centerOn(player);
    // więc robimy prosty trick: chcemy żeby player zawsze był w punkcie (0,0)
    // dlatego przesuwamy wszystkie inne obiekty o odwrotność tego co powinien przesunąć się player
    qreal PlayerScenePosX = Window::player->scenePos().x();
    qreal PlayerScenePosY = Window::player->scenePos().y();
    foreach(QGraphicsItem *item, scene.items())
    {
        Object *object = static_cast<Object *> (item);
        object->setPos(object->x() - PlayerScenePosX, object->y() - PlayerScenePosY);
    }
    player->setPos(0,0);
    this->ProcessPlayer();
    scene.update();           // aktualizacja sceny i przerysowanie widoku

    while(!spawn.empty())
    {
        scene.addItem(spawn.dequeue());
    }

    while(!kill.empty())
    {
        Object *dead = kill.dequeue();
        if(dead != NULL)
        {
            scene.removeItem(dead);
            delete dead;
        }
    }
}

void Window::keyPressEvent( QKeyEvent *event )  // zdarzenie przyciśnięcia klawisza
{
    if ( event->isAutoRepeat() || !actions.contains( event->key() ) )
    {                     // autorepeat to wirtualne przyciśnięcia klawisza kiedy przytrzyma się na nim palec
        event->ignore();  // ignoruj zdarzenie jeśli autorepeat lub jesli nie ma akcji dla tego klawisza
        return;
    }
    Action action = actions[event->key()]; // wybieramy akcje w zależności od klawisza
    switch ( action )
    {
    case Forward:
        Pressed_forward = TRUE;
        break;
    case RotateLeft:
        Pressed_rotateLeft = TRUE;
        break;
    case RotateRight:
        Pressed_rotateRight = TRUE;
        break;
    case StrafeLeft:
        Pressed_strafeLeft = TRUE;
        break;
    case StrafeRight:
        Pressed_strafeRight = TRUE;
        break;
    case Shoot:
        Pressed_shoot = TRUE;
        break;
    case Brake:
        Pressed_brake= TRUE;
        break;
    case Escape:
        this->close();
        break;
    default:
        event->ignore();
        return;
    }
    event->accept();
}


void Window::keyReleaseEvent( QKeyEvent *event )
{
    if ( event->isAutoRepeat() || !actions.contains( event->key() ) )
    {
        event->ignore();
        return;
    }
    Action action = actions[event->key()];
    switch ( action )
    {
    case Forward:
        Pressed_forward = FALSE;
        break;
    case RotateLeft:
        Pressed_rotateLeft = FALSE;
        break;
    case RotateRight:
        Pressed_rotateRight = FALSE;
        break;
    case StrafeLeft:
        Pressed_strafeLeft = FALSE;
        break;
    case StrafeRight:
        Pressed_strafeRight = FALSE;
        break;
    case Shoot:
        Pressed_shoot = FALSE;
        break;
    case Brake:
        Pressed_brake= FALSE;
        break;
    default:
        event->ignore();
        return;
    }
    event->accept();
}

void Window::wheelEvent( QWheelEvent *event )
{
    float scale = 1.0 + event->delta()*0.001;
    this->scale(scale,scale);
    event->accept();
}

void Window::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->save();
    painter->resetTransform();
    painter->drawPixmap(0,0,this->background);
    painter->restore();
}


void Window::drawForeground(QPainter *painter, const QRectF &rect)
{
    std::stringstream buffer;
    buffer << score;
    std::stringstream buffer2;
    buffer2 << player->health;
    painter->save();
    painter->resetTransform();
    painter->setPen(QColor(50,255,50,200));
    painter->setBrush(QColor(0,150,0,100));
    painter->setFont( QFont("Verdana", 16, QFont::Bold));
    painter->drawText(10,10,80,30, Qt::AlignVCenter | Qt::AlignLeft, "Score: ");
    painter->drawText(120,10,30,30, Qt::AlignVCenter | Qt::AlignLeft, buffer.str().c_str());

    painter->drawText(10,50,100,30, Qt::AlignVCenter | Qt::AlignLeft, "Health: ");
    painter->drawText(120,50,50,30, Qt::AlignVCenter | Qt::AlignLeft, buffer2.str().c_str());
    painter->drawRect(1,0,200,100);

    if(gameOver)
    {
        painter->setFont( QFont("Verdana", 50, QFont::Bold));
        painter->drawText((this->width()/2)-250,(this->height()/2)-100,500,200, Qt::AlignVCenter | Qt::AlignVCenter, "Game Over");
        Pressed_brake = FALSE;
        Pressed_forward = FALSE;
        Pressed_strafeLeft = FALSE;
        Pressed_strafeRight = FALSE;
        Pressed_rotateLeft = FALSE;
        Pressed_rotateRight = FALSE;
        Pressed_shoot = FALSE;
    }
    painter->restore();
}

void Window::mouseMoveEvent(QMouseEvent * event )
{
    Window::mouseX = event->x();
    Window::mouseY = event->y();
    event->accept();
}

void Window::ProcessPlayer()
{
    float centerx = this->width()/2.0; // srodek ekranu
    float centery = this->height()/2.0;
    float linex = (Window::mouseX - centerx); // linia od środka do kursora
    float liney = (Window::mouseY - centery);

    float arc = atan2(linex,-liney);
    arc = arc * 180.0 / 3.14156872; // kąt od srodka do kursora
    float diff = arc - player->rotation(); // roznica miedzy katem gracza a katem do kursora
    while(diff < -180.0) diff += 360.0;
    while(diff >  180.0) diff -= 360.0; // normalizujemy do (-180, 180)

    float time = fabs(diff/player->rotvel); // czas w jakim gracz obroci sie do zadanego kąta
    float endRot = fabs(player->rotvel * pow(player->rotfriction, time)) - fabs(player->rotacc*time);
    // rotacja jaka gracz bedzie mial po tym czasie JEŚLI każemy mu hamować

    if(diff>-0.8 && diff<0.8) // wewnątrz tolerancji, nie ruszamy, tłumimy
    {
        Window::Pressed_rotateLeft = FALSE;
        Window::Pressed_rotateRight = FALSE;
        player->rotvel*=0.9333; //sprytne tlumienie
    }
    else if(diff>0.0 && player->rotvel<0.0 && endRot > 0.0) // hamujemy
    {
        Window::Pressed_rotateLeft = TRUE;
        Window::Pressed_rotateRight = FALSE;
    }
    else if(diff<0.0 && player->rotvel>0.0 && endRot > 0.0)
    {
        Window::Pressed_rotateRight = TRUE;
        Window::Pressed_rotateLeft = FALSE;
    }
    else if(diff>0.0) // krecimy sie ale zdążymy wyhamować więc kręcimy bardziej
    {
        Window::Pressed_rotateRight = TRUE;
        Window::Pressed_rotateLeft = FALSE;
    }
    else
    {
        Window::Pressed_rotateLeft = TRUE;
        Window::Pressed_rotateRight = FALSE;
    }
}

void Window::mousePressEvent ( QMouseEvent * event )
{
    if (event->button() == Qt::LeftButton ||
        event->button()== Qt::RightButton)
    {
        Window::Pressed_shoot = true;
    }
    event->accept();
}

void Window::mouseReleaseEvent ( QMouseEvent * event )
{
    if (event->button() == Qt::LeftButton ||
        event->button()== Qt::RightButton)
    {
        Window::Pressed_shoot = false;
    }
    event->accept();
}
