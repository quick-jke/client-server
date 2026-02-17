#ifndef SELECT_PARAMS_HPP
#define SELECT_PARAMS_HPP
#include <QRect>
#include "position_list.hpp"
struct SelectParam
{
    QRect         m_OldRect;
    QRect         m_NewRect;
    PositionList* m_pPosList;
    bool          m_bSameScale;

    SelectParam(QRect oldRect, QRect newRect) :
        m_OldRect(oldRect),
        m_NewRect(newRect)
    {
    }
};

#endif
