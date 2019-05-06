#include "ihmrapportderonde.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    IHMRapportDeRonde w;
    w.show();

    return a.exec();
}
