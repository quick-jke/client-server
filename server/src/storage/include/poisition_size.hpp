#ifndef POSISTION_SIZE_HPP
#define POSISTION_SIZE_HPP
#include <QtCore>
struct PosSize
{
    qint64 m_Pos;
    qint64 m_Size;
    PosSize(qint64 p = 0, qint64 s = 0) : m_Pos(p), m_Size(s) {}
};


#endif
