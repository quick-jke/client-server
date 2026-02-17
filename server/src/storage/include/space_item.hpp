#ifndef SPACE_ITEM_HPP
#define SPACE_ITEM_HPP
#include <QRect>
#include "select_params.hpp"
#include "spc_obj.hpp"
class SpaceItem : public QRect
{
public:
    SpaceItem(QRect r = QRect()) : QRect(r) {}
    void setNull() { *this = QRect(); }
    void copyBox(SpaceItem* pItem) { *this = *pItem; }
    void addBox(QRect const& r) { *this |= r; }

public:
    virtual ~SpaceItem() {}
    virtual void split(SpaceItem**) {}
    virtual bool removeObject(SpcObj const*, SpaceItem**) { return false; }
    virtual void addObject(SpcObj*, SpaceItem** = nullptr) {}
    virtual int selectObjects(SelectParam*) { return 0; }
};


#endif
