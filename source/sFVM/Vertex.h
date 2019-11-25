#pragma once
#include "essential.h"
class Vertex
{
    friend class Vector;
public:
    static Vertex* New();
    static Vertex* New(double,double,double);
    Vertex(double=0, double=0, double=0);
    ~Vertex();
    void SetVertex(double=0, double=0, double=0);
    double* GetVertex();
private:
    double pos[3];
protected:
};
