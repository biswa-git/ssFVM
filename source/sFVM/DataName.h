#pragma once

enum VARIABLE
{
    PRESSURE,
    MASSFLUX1,
    MASSFLUX2,
    MASSFLUX3,
    TEMPERATURE,
    VOLUMEFRACTION
};

/*
enum DIRECTION
{
    D_1_P=1000,
    D_2_P,
    D_3_P,
    D_3_N,
    D_2_N,
    D_1_N
};
*/

enum BOUNDARY
{
    DRICHLET,
    NEUMANN,
    INLET,
    OUTFLOW,
    SYMMETRIC,
    WALL
};

//typedef const int USER_DEFINED_VARIABLE;
