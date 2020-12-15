#ifndef INFNET_H
#define INFNET_H

#include <QString>

struct InfNet final
{
    QChar m_net;
    int m_countAlternative;
    int m_firstNumberAlternative;
};

#endif // INFNET_H
