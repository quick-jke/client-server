#ifndef SPC_OBJ_HPP
#define SPC_OBJ_HPP

#include <QRect>

struct SpcObj : public QRect
{
    SpcObj(QRect const& r, qint64 pos, qint64 size, SpcObj* pNxt = nullptr) :
        QRect(r),
        m_Pos(pos),
        m_Size(size),
        m_pNext(pNxt)
    {
    }

    qint64  m_Pos;
    qint64  m_Size;
    SpcObj* m_pNext;
};

#endif
