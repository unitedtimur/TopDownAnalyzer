#include "grammar.h"

Grammar &Grammar::instance()
{
    static Grammar grammar;
    return grammar;
}

QString Grammar::grammar() const
{
    QString result;

    for (qint32 i = 0; i < m_lr.size(); ++i)
        result += QString::number(i + 1) + QString(": ")
                + m_lr.at(i) + QString(" -> ") + m_rr.at(i) + QString("\n");

    return result;
}

Grammar::Grammar() :
    m_lr("ABBTTMMM"),
    m_rr({ "!B!","T+B","T","M","M*T","a","b","(B)" })
{
    QVector<QChar> statusSymbols(256, 0);

    qint32 count = 0;

    for (qint32 i = 0; i < m_lr.size(); i += count) {
        const qint32 index = static_cast<qint32>(m_lr.at(i).toLatin1());

        if (statusSymbols[index] == 0) {
            statusSymbols[index] = 1;
            count = 1;

            while (m_lr[i + count] == m_lr[i]) {
                ++count;
            }

            m_inf.push_back(InfNet({ m_lr[i], count, i }));
        }
    }

    for (qint32 i = 0; i < m_rr.size(); ++i) {
        for (qint32 j = 0; j < m_rr.size(); ++j) {
            const qint32 index = static_cast<qint32>(m_rr[i][j].toLatin1());

            if (statusSymbols[index] == 0) {
                statusSymbols[index] = 2;
                m_t += m_rr[i][j];
            }
        }
    }
}
