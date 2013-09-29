#ifndef WINDOW_H
#define WINDOW_H

#include <QtWidgets>
#include <QPixmap>

class Player;
class Object;
// Tzw. "forward class declaration" - dzięki temu nie musimy załączać do tego pliku nagłówkowego całego pliku "player.h"
// (nadal trzeba to zrobić w pliku .cpp). Sposób działa tylko jeśli posługujemy się wyłącznie referencjami
// (znaczy nie używamy tej klasy do dziedziczenia, ani nie deklarujemy obiektów). Znacznie skraca to czas kompilacji.

class Window : public QGraphicsView
{
    Q_OBJECT    // makro mówiące że to jest QObject - obiekt interfejsu QT
public:
    explicit Window(QObject *parent = 0);
    QGraphicsScene scene;         // scena jest pojemnikiem na obiekty bez reprezentacji graficznej
    QTimer timer;

    Player *player; // korzystamy z forward class declaration

    static bool Pressed_forward;
    static bool Pressed_rotateLeft;
    static bool Pressed_rotateRight;
    static bool Pressed_brake;
    static bool Pressed_shoot;

    static bool Pressed_strafeLeft;
    static bool Pressed_strafeRight;

    enum Action { Forward, RotateLeft, RotateRight, Brake, Shoot, Escape, StrafeLeft, StrafeRight };
    QMap<int,Action> actions;         // mapa przyporzadkowujaca stale kodujace przycisniecia klawiszy
                                      // do listy enum stworzonej powyżej

    static QQueue<Object*> spawn;     // kolejka obiektow do dodania
    static QQueue<Object*> kill;      // kolejka obiektow do usuniecia

    QPixmap background;
    static int mouseX;
    static int mouseY;
    static int score;
    static bool gameOver;
    static int mouse_count;
    void ProcessPlayer();

signals:

public slots:
    void keyPressEvent( QKeyEvent *event );
    void keyReleaseEvent( QKeyEvent *event );
    void wheelEvent( QWheelEvent *event);    // ruch kółkiem myszy
    void drawBackground(QPainter *painter, const QRectF &rect);
    void drawForeground(QPainter *painter, const QRectF &rect);
    void mouseMoveEvent ( QMouseEvent * event );
    void mousePressEvent ( QMouseEvent * event );
    void mouseReleaseEvent ( QMouseEvent * event );

private slots:
    void MainClockTick();

};

#endif // WINDOW_H
