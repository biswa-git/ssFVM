//notHidden
#pragma once
#include "essential.h"
#include "Block.h"
#include "BoundaryCondition.h"

class Block;
class Face
{
public:
    static Face* New(Block*, int);
    ~Face();
    int GetDir();
    void SetBC(BC*);
    BC* GetBC();
    Block* GetParentBlk();
    void SetNeighbourBlk(Block*);
protected:
    Face(Block*,int);
private:
    int dir;
    BC *bc;
    Block *parentBlk;
    Block *neighbourBlk;
};
