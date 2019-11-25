#include "Face.h"

Face* Face::New(Block* blk, int d)
{
    return new Face(blk, d);
}

Face::Face(Block* blk, int d):dir(d),bc(BC::New()),parentBlk(blk)
{

}

Face::~Face()
{
    FreeObjMacro(bc)
}

int Face::GetDir()
{
    return dir;
}
Block* Face::GetParentBlk()
{
    return parentBlk;
}

void Face::SetBC(BC* b)
{
	bc = b;
}

void Face::SetNeighbourBlk(Block* blk)
{
    neighbourBlk = blk;
}

BC* Face::GetBC()
{
    return bc;
}
