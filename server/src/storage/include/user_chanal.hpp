#ifndef USER_CHANAL_HPP
#define USER_CHANAL_HPP
typedef QHash<int, bool> IntBoolHash;

class UserChanal
{
public:
    IntBoolHash  m_NodeHash;
    IntBoolHash  m_ClassHash;
    PositionList m_PosList;
};

#endif
