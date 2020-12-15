#include <QCoreApplication>
#include <QTextStream>
#include "grammar.h"
#include "translator.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTextStream cout(stdout);
    QTextStream cin(stdin);

    cout << Grammar::instance().grammar() << endl;

    cout << "Enter example: " << flush;

    QString example;

    cin >> example;

    Translator translator;
    const auto result = translator.translation(example);

    for (const auto &it : result)
        cout << it + " " ;

    cout << endl;

    return a.exec();
}
