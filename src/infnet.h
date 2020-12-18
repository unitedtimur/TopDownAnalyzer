#ifndef INFNET_H
#define INFNET_H

#include <QString>

struct InfNet final
{
    // Нетерминал
    QChar net;
    // Количество альтернатив
    int countAlternative;
    // Номер первой альтернативы
    int firstNumberAlternative;
};

#endif // INFNET_H
