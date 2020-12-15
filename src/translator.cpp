#include "translator.h"
#include <QDebug>

Translator::Translator() :
    m_grammar(Grammar::instance()),
    m_s(m_grammar.lr().at(0))
{

}

QVector<qint32> Translator::translation(const QString &in)
{
    QVector<qint32> result;

    m_l1 = std::stack<Translator::elementL1>();
    m_l2 = std::stack<Translator::elementL2>();
    m_result = std::stack<qint32>();

    bool isNotEnd = true;

    qint32 n = in.length();
    qint32 i = 0;

    m_s = QChar('q');
    m_y.isTerm = 0;
    m_y.symbol = static_cast<Translator::byte>(m_s.toLatin1());
    m_l2.push(m_y);

    while (isNotEnd) {
        switch (m_s.toLatin1()) {
        case 't': {
            while (!m_result.empty()) {
                result.push_back(m_result.top());
                m_result.pop();
            }

            isNotEnd = false;
        }
            break;
        case 'q': {
            m_y = m_l2.top();

            if (findNet(m_y.symbol) != -1) {
                step1();
            } else {
                if (m_y.symbol == in.at(i)) {
                    step2();

                    ++i;

                    if (i == n) {
                        // Формирование результата
                        if (m_l2.empty()) {
                            step3();
                            m_s = QChar('t');
                        } else {
                            // Шаг 3
                            m_s = QChar('b');
                        }
                    } else {
                        // Шаг 3
                        if (m_l2.empty()) {
                            m_s = QChar('b');
                        }
                    }
                } else {
                    // Шаг 4
                    m_s = QChar('b');
                }
            }
        }
            break;

        case 'b': {
            m_x = m_l1.top();

            if (findTerm(m_x.symbol) != -1) {
                step5();

                --i;
            } else {
                if (m_x.numberAlternative < m_x.countAlternative) {
                    step6a();

                    m_s = QChar('q');
                } else {
                    if (m_x.symbol == m_s && i == 0) {
                        isNotEnd = false;

                        qDebug() << "залупа";
                    } else {
                        step6v();
                    }
                }
            }
        }
            break;
        }
    }

    return result;
}

qint32 Translator::findNet(const QChar &symbol)
{
    for (qint32 i = 0; i < m_grammar.inf().size(); ++i) {
        const InfNet infNet = m_grammar.inf().at(i);

        if (infNet.net == symbol)
            return i;
    }

    return -1;
}

qint32 Translator::findTerm(const QChar &symbol)
{
    for (qint32 i = 0; i < m_grammar.t().length(); ++i) {
        if (m_grammar.t().at(i) == symbol)
            return i;
    }

    return -1;
}

// Применение альтернативы (удаление элеметов из стека L2)
void Translator::popL2_RR(const qint32 &rule)
{
    const qint32 length = m_grammar.rr().at(rule).length();

    for (qint32 i = 0; i < length; ++i)
        m_l2.pop();
}

// Заполнение стека L2 по правым частям правил
void Translator::pushL2_RR(const qint32 &rule)
{
    const qint32 length = m_grammar.rr().at(rule).length();

    for (qint32 i = 0; i < length; ++i) {
        // Заполнение элемента стека L2
        m_y.symbol = static_cast<Translator::byte>(m_grammar.rr()
                                                   .at(rule)
                                                   .at(length - i - 1)
                                                   .toLatin1()
                                                   );

        m_y.isTerm = static_cast<Translator::byte>(findNet(m_grammar.rr()
                                                           .at(rule)
                                                           .at(length - i - 1)
                                                           ) == -1);

        m_l2.push(m_y);
    }
}

// Заполнение стека L2 по левым частям правил
void Translator::pushL2_LR(const qint32 &rule)
{
    // Заполнение элемента стека L2
    m_y.symbol = static_cast<Translator::byte>(m_grammar.lr().at(rule).toLatin1());
    m_y.isTerm = static_cast<Translator::byte>(findNet(m_grammar.lr().at(rule)) == -1);
    m_l2.push(m_y);
}

// Шаг 1: применение первой альтернативы
void Translator::step1()
{
    const qint32 net = findNet(static_cast<QChar>(m_y.symbol));

    // Заполнение элемента стека L1
    m_x.isTerm = 0;
    m_x.symbol = m_y.symbol;
    m_x.numberAlternative = 1;
    m_x.countAlternative = m_grammar.inf().at(net).countAlternative;

    m_l1.push(m_x);
    m_l2.pop();

    const qint32 rule = m_grammar.inf().at(net).firstNumberAlternative + m_x.numberAlternative - 1;

    pushL2_RR(rule);
}

// Шаг 2: перенесение терминального символа из L2 в L1
void Translator::step2()
{
    m_l2.pop();

    // Заполнение элемента стека L1
    m_x.isTerm = 1;
    m_x.symbol = m_y.symbol;
    m_x.countAlternative = 0;
    m_x.numberAlternative = 0;

    m_l1.push(m_x);
}

// Шаг 3: завершение алгоритма и формироание цепочки
void Translator::step3()
{
    qint32 net;

    while (!m_l1.empty()) {
        m_x = m_l1.top();

        m_l1.pop();

        net = findNet(static_cast<QChar>(m_x.symbol));

        if (net != -1)
            m_result.push(m_grammar.inf().at(net).firstNumberAlternative + m_x.numberAlternative);
    }
}

// Шаг 5: возврат (перенос элемента из L1 в L2)
void Translator::step5()
{
    m_l1.pop();

    m_y.symbol = m_x.symbol;
    m_y.isTerm = 1;

    m_l2.push(m_y);
}

// Шаг 6a: применение следующей альтернативы
void Translator::step6a()
{
    m_l1.pop();

    ++m_x.numberAlternative;

    m_l1.push(m_x);

    const qint32 net = findNet(static_cast<QChar>(m_x.symbol));
    const qint32 rule = m_grammar.inf().at(net).firstNumberAlternative + m_x.numberAlternative - 1;

    popL2_RR(rule - 1);
    pushL2_RR(rule);
}

// Шаг 6v: возврат (отмена текущей альтернативы)
void Translator::step6v()
{
    const qint32 net = findNet(static_cast<QChar>(m_x.symbol));
    const qint32 rule = m_grammar.inf().at(net).firstNumberAlternative + m_x.numberAlternative - 1;

    popL2_RR(rule);
    pushL2_LR(rule);

    m_l1.pop();
}
