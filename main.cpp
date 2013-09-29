#include <QtWidgets>

#include <math.h>

#include "mouse.h"
#include "window.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv); // stworzenie aplikacji
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime())); // ziarno losowości
    Window view; // stworzenie okna - konstruktor robi resztę
    return app.exec();
}
