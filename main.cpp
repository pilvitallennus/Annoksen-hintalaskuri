#include "mainwindow.hh"
#include "foodfunctionality.hh"
#include "product.hh"

#include <QCoreApplication>
#include <QApplication>


// tiedostot on aukaistu polulla, jotta ne näkyvät editorissa. poista polut niin ohjelma luo tiedostot build-.kansioon (?)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
