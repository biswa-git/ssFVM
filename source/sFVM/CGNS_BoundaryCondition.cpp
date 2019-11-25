#include "CGNS_BoundaryCondition.h"

MESH::BoundaryCondition::BoundaryCondition()
{

}

MESH::BoundaryCondition::BoundaryCondition(const BoundaryCondition& BC)
{
    name = BC.name;
    typeId = BC.typeId;
    value = BC.value;
}

MESH::BoundaryCondition::~BoundaryCondition()
{

}

void MESH::BoundaryCondition::SetName(const string &S)
{
    name = S;
}

string MESH::BoundaryCondition::GetName()
{
    return name;
}

void MESH::BoundaryCondition::SetTypeId(const int &I)
{
    typeId = I;
}

int MESH::BoundaryCondition::GetTypeId()
{
    return typeId;
}
