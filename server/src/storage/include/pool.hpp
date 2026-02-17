#ifndef POOL_HPP
#define POOL_HPP
#include "space_item.hpp"
#include "spc_obj.hpp"
#include "select_params.hpp"
class Pool : public SpaceItem
{
public:
    Pool();
    ~Pool();
    void addObject(SpcObj* pObj, SpaceItem** ppRoot);
    bool removeObject(SpcObj const* pObj, SpaceItem** ppRoot);
    int  selectObjects(SelectParam* pSp);
    void split(SpaceItem** ppRoot);
    SpcObj* takeHead();

private:
    int     m_Size;
    SpcObj* m_pPool;
};

#endif
