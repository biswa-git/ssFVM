#include "Vertex.h"

Vertex* Vertex::New()
{
    return new Vertex;
}
Vertex* Vertex::New(double x, double y, double z)
{
    return new Vertex(x,y,z);
}

Vertex::Vertex(double x, double y, double z)
{
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
}

Vertex::~Vertex()
{
    //
}

void Vertex::SetVertex(double x, double y, double z)
{
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
}

double* Vertex::GetVertex()
{
    return pos;
}
