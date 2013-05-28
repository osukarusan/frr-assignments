#include <QApplication>
#include <QFileDialog>
#include <QString>
#include <QtDebug>
#include "principal.h"

int main(int argc, char ** argv)
{
    QApplication a(argc, argv);
    Principal w;
    w.show();
    w.ui.glWidget->addObject();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
