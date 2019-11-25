#include "BoundaryCondition.h"

BC* BC::New()
{
    return new BC;
}

BC::BC():BC_Id(BLOCK_INTERFACE)
{
    //
}

BC::BC(int id, double value):BC_Id(id),BC_Value(value)
{
    //
}

BC::BC(int id, int varId, double value):BC_Var(varId),BC_Id(id),BC_Value(value)
{
    //
}

BC::~BC()
{
    //
}

void BC::SetBC(int varId, int id, double value)
{
    BC_Var = varId;
    BC_Id = id;
    BC_Value = value;
}

void BC::SetBCVar(int varId)
{
    BC_Var = varId;
}

void BC::SetBC_Id(int id)
{
    BC_Id = id;
}

void BC::SetBCValue(double value)
{
     BC_Value = value;
}

int BC::GetBC_Id()
{
    return BC_Id;
}

int BC::GetBC_Var()
{
    return BC_Var;
}

double BC::GetBC_Value()
{
    return BC_Value;
}
