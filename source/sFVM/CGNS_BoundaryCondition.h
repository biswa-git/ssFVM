#pragma once
#include "essential.h"

namespace MESH
{
    class BoundaryCondition
    {
    public:
        BoundaryCondition();
        BoundaryCondition(const BoundaryCondition& BC);
        ~BoundaryCondition();
        void SetName(const string&);
        string GetName();
        void SetTypeId(const int&);
        int GetTypeId();

    private:
        string name;
        int typeId;
        double value;
    }; 
}
