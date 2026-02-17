#ifndef POSITION_LIST_HPP
#define POSITION_LIST_HPP

#include <QVector>
#include "poisition_size.hpp"

class PositionList : public QVector<PosSize>
{
public:
    void insert(qint64 pos, qint64 size) { append(PosSize(pos, size)); }
    void sort() { std::sort(begin(), end(), [](PosSize& i0, PosSize& i1) { return i0.m_Pos < i1.m_Pos; }); }
};


#endif
