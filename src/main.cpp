#include <QCoreApplication>
#include <QTextStream>
#include "grammar.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    QTextStream cout(stdout);
    QTextStream cin(stdin);

    cout << Grammar::instance().grammar() << flush;

    return a.exec();
}
