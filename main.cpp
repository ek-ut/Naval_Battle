#include "game_map.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    game_map w;
    w.show();

    return a.exec();
}
