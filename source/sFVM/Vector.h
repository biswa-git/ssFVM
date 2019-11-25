#pragma once
#include "essential.h"
#include "Vertex.h"

class Vector
{
    friend double DotProduct(Vector const &, Vector const &);
public:
    static Vector* New(Vertex const &,Vertex const &);
    static Vector* New();
    Vector(Vector&);
    Vector();
    Vector(Vertex const &,Vertex  const &);
    ~Vector();
    void SetVector(Vertex  const &,Vertex  const &);
    Vector operator +(Vector const &);
    Vector operator *(Vector const &);
    Vector operator /(double const);
    void operator =(Vector const &);
    double GetNorm();
    Vector* GetUnit();

    void Print();
private:
    double xLen,yLen,zLen;
    void SetLength(Vertex const &,Vertex const &);
protected:
};
