#ifndef SPACE_TREE_HPP
#define SPACE_TREE_HPP

#include "position_list.hpp"
#include "select_params.hpp"
#include "type_root_item.hpp"
#include <QRect>

typedef QHash<int, bool> IntBoolHash;

class SpaceTree
{
public:
    PositionList selectObjects(QRect oldRect, int oldScale, QRect newRect, int newScale, const IntBoolHash* pClassHash, const IntBoolHash* pNodeHash)
    {
        PositionList posList;
        if (pNodeHash->empty())
            pNodeHash = nullptr;

        SelectParam sp(oldRect, newRect);

        for (auto item = m_RootHash.begin(); item != m_RootHash.end(); ++item)
        {
            if (item.key() == 0 || pClassHash->value(item.key(), false))
            {
                int minScale = item.value().m_MinScale;
                if (minScale && newScale < minScale)
                    continue;

                int maxScale = item.value().m_MaxScale;
                if (maxScale && newScale > maxScale)
                    continue;

                PositionList clsList;
                sp.m_pPosList = &clsList;
                sp.m_bSameScale = oldScale == newScale || ((!minScale || oldScale >= minScale) && (!maxScale || oldScale <= maxScale));
                item.value().selectObjects(&sp, pNodeHash);
                clsList.sort();
                posList.append(clsList);
            }
        }
        return posList;
    }
    void clear() { m_RootHash.clear(); }
    void sort();

private:
    QHash<int, TypeRootItem> m_RootHash;
};


#endif
