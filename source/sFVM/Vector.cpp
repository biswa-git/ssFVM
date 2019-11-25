#include "Vector.h"

Vector* Vector::New(Vertex  const &head,Vertex  const &tail)
{
    return new Vector(head,tail);
}

Vector* Vector::New()
{
    Vertex *Vrtx = Vertex::New();
    Vector *Vect = Vector::New(*Vrtx,*Vrtx);
    delete Vrtx;
    return Vect;
}

Vector::Vector(Vector &V):xLen(V.xLen),yLen(V.yLen),zLen(V.zLen)
{
    //
}

Vector::Vector(Vertex  const &head,Vertex const &tail)
{
    SetLength(head,tail);
}

Vector::Vector()
{

}

Vector::~Vector()
{

}

void Vector::SetVector(Vertex const &head, Vertex const &tail)
{
    SetLength(head,tail);
}

void Vector::SetLength(Vertex const &head, Vertex  const &tail)
{
    xLen = head.pos[0]-tail.pos[0];
    yLen = head.pos[1]-tail.pos[1];
    zLen = head.pos[2]-tail.pos[2];
}
Vector Vector::operator+(Vector const &V)
{
    Vector tmp;
    tmp.xLen = this->xLen+V.xLen;
    tmp.yLen = this->yLen+V.yLen;
    tmp.zLen = this->zLen+V.zLen;
    return tmp;
}
Vector Vector::operator*(Vector const &V)
{
    //PERFORMING CROSS PRODUCT
    Vector tmp;
    tmp.xLen =  (this->yLen*V.zLen-V.yLen*this->zLen);
    tmp.yLen = -(this->xLen*V.zLen-V.xLen*this->zLen);
    tmp.zLen =  (this->xLen*V.yLen-V.xLen*this->yLen);
    return tmp;
}

Vector Vector::operator/(double const D)
{
    Vector tmp;
    tmp.xLen = this->xLen/D;
    tmp.yLen = this->yLen/D;
    tmp.zLen = this->zLen/D;
    return tmp;
}

void Vector::operator=(Vector const &V)
{
    this->xLen = V.xLen;
    this->yLen = V.yLen;
    this->zLen = V.zLen;
}

double Vector::GetNorm()
{
    // GETTING LENGTH
    return sqrt(pow2(xLen)+pow2(yLen)+pow2(zLen));
}

Vector* Vector::GetUnit()
{
    double norm = GetNorm();
    Vector *unit = new Vector();
    unit->xLen = this->xLen/norm;
    unit->yLen = this->yLen/norm;
    unit->zLen = this->zLen/norm;
    return unit;
}

void Vector::Print()
{
    cout<<"["<<xLen<<","<<yLen<<","<<zLen<<"]"<<endl;
}


double DotProduct(Vector const & V1, Vector const & V2)
{
    return V1.xLen*V2.xLen + V1.yLen*V2.yLen + V1.zLen*V2.zLen;
}
