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

    if (result.at(0) != -1) {
        for (const auto &it : result)
            cout << QString::number(it) + " ";

        cout << endl;
    } else {
        cout << QString("Fatal error! " + QString::fromUtf8(__FUNCTION__ )) << flush;
    }

    return a.exec();
}
