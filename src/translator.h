#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QStack>
#include "grammar.h"

class Translator
{
    using byte = unsigned char;

    struct elementL1
    {
        // Признак - является ли терминалом
        byte isTerm;
        // Символ
        byte symbol;
        // Номер текущей альтернативы (для нетерминала)
        qint32 numberAlternative;
        // Количество альтернатив (для нетерминала)
        qint32 countAlternative;
    };

    struct elementL2
    {
        // Признак - является ли терминалом
        byte isTerm;
        // Символ
        byte symbol;
    };

public:
    explicit Translator();

    QVector<qint32> translation(const QString &in);

protected:
    // Поиск нетерминала
    qint32 findNet(const QChar &symbol);

    // Поиск терминала
    qint32 findTerm(const QChar &symbol);

    // Работа со стэками
    void popL2_RR(const qint32 &rule);
    void pushL2_RR(const qint32 &rule);
    void pushL2_LR(const qint32 &rule);

    // Шаги
    void step1();
    void step2();
    void step3();
    void step5();
    void step6a();
    void step6v();

private:
    // Грамматика
    const Grammar &m_grammar;

    // Стеки и элементы
    QStack<elementL1> m_l1;
    QStack<elementL2> m_l2;

    elementL1 m_x;
    elementL2 m_y;

    // Результат распознования
    QStack<qint32> m_result;

    // Состояние алгоритма (q, b, t)
    QChar m_condition;

    // Начальный символ граматики
    QChar m_s;
};

#endif // TRANSLATOR_H
