#ifndef BRANCH_PTR_HPP
#define BRANCH_PTR_HPP
#include "space_item.hpp"
#include "spc_obj.hpp"
#include "pool.hpp"
#include "select_params.hpp"
class BranchPtr
{
public:
    BranchPtr() : m_pBranch(nullptr) {}
    ~BranchPtr() { delete m_pBranch; }
    int isNull() { return !m_pBranch || m_pBranch->isNull(); }
    void split(SpaceItem** ppRoot)
    {
        if (m_pBranch)
            m_pBranch->split(ppRoot);
    }
    void addObject(SpcObj* pObj, bool bSort)
    {
        if (!m_pBranch)
            m_pBranch = new Pool;
        if (m_pBranch)
            m_pBranch->addObject(pObj, bSort ? &m_pBranch : nullptr);
    }
    bool removeObject(SpcObj const* pObj)
    {
        return m_pBranch && m_pBranch->removeObject(pObj, &m_pBranch);
    }
    int selectObjects(SelectParam* pSp)
    {
        return m_pBranch ? m_pBranch->selectObjects(pSp) : 0;
    }
    SpaceItem** getRootPtr()
    {
        return &m_pBranch;
    }

public:
    SpaceItem* m_pBranch;
};
#endif
