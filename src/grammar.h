#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <QVector>
#include "infnet.h"

class Grammar final
{
public:
    static Grammar &instance();

    QString grammar() const;

    QString lr() const;

    QVector<QString> rr() const;

    QString t() const;

    QVector<InfNet> inf() const;

protected:
    explicit Grammar();
    explicit Grammar(const Grammar &) = default;
    Grammar& operator=(Grammar &) = default;

private:
    // Левые части правил
    mutable QString m_lr;

    // Правые части правил
    mutable QVector<QString> m_rr;

    // Множество терминалов
    QString m_t;

    // Множество нетерминалов
    mutable QVector<InfNet> m_inf;
};

#endif // GRAMMAR_H
