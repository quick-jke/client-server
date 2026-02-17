#ifndef TYPE_ROOT_ITEM_HPP
#define TYPE_ROOT_ITEM_HPP
#include <QHash>
#include "branch_ptr.hpp"
#include "select_params.hpp"

typedef QHash<int, bool> IntBoolHash;

class TypeRootItem : public QHash<int, BranchPtr>
{
public:
    TypeRootItem(int mis = 0, int mas = 0) :
        m_MinScale(mis),
        m_MaxScale(mas)
    {
    }
    void sort();
    int  selectObjects(SelectParam* pSp, IntBoolHash const* pNodeHash)
    {
        int n = 0;
        for (auto item = begin(); item != end(); ++item)
            if (!pNodeHash || pNodeHash->value(item.key(), false))
                n += item.value().selectObjects(pSp);
        return n;
    }


    int m_MinScale;
    int m_MaxScale;
};


#endif
