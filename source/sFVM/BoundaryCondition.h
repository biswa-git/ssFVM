//notHidden
#pragma once
#include "essential.h"

class BC
{
public:
    static BC* New();
    ~BC();

    void SetBC(int, int = BLOCK_INTERFACE, double = 0.0);
    void SetBCVar(int);
    void SetBC_Id(int = BLOCK_INTERFACE);
    void SetBCValue(double = 0.0);

    int GetBC_Id();
    int GetBC_Var();
    double GetBC_Value();
protected:
    BC();
    BC(int, double = 0.0);
    BC(int, int, double = 0.0);
private:
    int BC_Var,BC_Id;
    double BC_Value;
};
