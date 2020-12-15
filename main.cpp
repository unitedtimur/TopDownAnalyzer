#include <QCoreApplication>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTextStream cout(stdout);
    QTextStream cin(stdin);

    cout << "Enter some shit: ";

    const QString line = cin.readLine();

    cout << line << endl;

    return a.exec();
}
