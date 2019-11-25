#include "Block.h"

Block* Block::New()
{
    return new Block;
}

Block* Block::New(int n1, int n2, int n3, string name)
{
    return new Block(n1,n2,n3,name);
}

Block::Block(int n1, int n2, int n3, string name): name(name),n1(n1),n2(n2),n3(n3),dimFlag(false),lockDimFlag(false),
                                                   data(new vector<DATA>),
                                                   nodeData(new vector<NODE>),
                                                   cellFaceData(new vector<CELLFACE>),
                                                   cellVolumeData(new vector<CELLVOLUME>)
{
        for (int i=0;i<6;++i)
        {
            face[i] = Face::New(this,i);
        }
        if(n1>0 && n2>0 && n3>0)
        {
            dimFlag = true;
        }
        else
        {
            dimFlag = false;
        }
}

Block::~Block()
{
    for(int i=0;i<6;++i)
    {
        if(face[i] != nullptr) delete face[i];
    }

    if(data           != nullptr) data          ->clear(); FreeObjMacro(data          );
    if(nodeData       != nullptr) nodeData      ->clear(); FreeObjMacro(nodeData      );
    if(cellFaceData   != nullptr) cellFaceData  ->clear(); FreeObjMacro(cellFaceData  );
    if(cellVolumeData != nullptr) cellVolumeData->clear(); FreeObjMacro(cellVolumeData);
}

void Block::SetSize(int n1,int n2,int n3)
{

    if(!lockDimFlag)
    {
        this->n1 = n1;
        this->n2 = n2;
        this->n3 = n3;
        if(n1>0 && n2>0 && n3>0)
        {
            dimFlag = true;
        }
        else
        {
            dimFlag = false;
        }
    }
    else
    {
        cerr<<"Warning:Can't update Block size as the Mesh is already initialized!"<<endl;
    }
}

int Block::GetSize(int n)
{
    switch (n)
    {
        case 1: return n1;
        case 2: return n2;
        case 3: return n3;
    default:
         cerr<<"Argument must be 1,2 or 3"<<endl;
        exit(EXIT_FAILURE);
    }
}

int Block::GetGlobalIdStart()
{
    return globalIdStart;
}

void Block::SetMesh(Mesh* msh)
{
    this->msh = msh;
}

DATA& Block::GetData(int i)
{
    int l = msh->dataMap.at(i);
    return data->at(sizeType(l));
}

bool Block::GetDimFlag()
{
    return dimFlag;
}

void Block::ReadGeomData()
{
    //temp valid
    NODE &nd = nodeData->at(0);
    for (int i = 0; i < n1+1; ++i)
    {
        for (int j = 0; j < n2+1; ++j)
        {
            for (int k = 0; k < n3+1; ++k)
            {
                nd(i,j,k).SetVertex(double(i+1),double(j*3),double(2*k));
            }
        }
    }

    CELLFACE &cfd1 = cellFaceData->at(0);
    for (int i = 0; i < n1+1; ++i)
    {
        for (int j = 1; j < n2+1; ++j)
        {
            for (int k = 1; k < n3+1; ++k)
            {
                Vector v1(nd(i,j-1,k-1),nd(i,j-1,k  ));
                Vector v2(nd(i,j  ,k-1),nd(i,j-1,k-1));
                Vector v3(nd(i,j  ,k  ),nd(i,j  ,k-1));
                Vector v4(nd(i,j-1,k  ),nd(i,j  ,k  ));

                cfd1(i,j,k)= (v1*v2+v3*v4)/2.0;
            }
        }
    }

    CELLFACE &cfd2 = cellFaceData->at(1);
    for (int i = 1; i < n1+1; ++i)
    {
        for (int j = 0; j < n2+1; ++j)
        {
            for (int k = 1; k < n3+1; ++k)
            {
                Vector v1(nd(i-1,j,k-1),nd(i  ,j,k-1));
                Vector v2(nd(i-1,j,k  ),nd(i-1,j,k-1));
                Vector v3(nd(i  ,j,k  ),nd(i-1,j,k  ));
                Vector v4(nd(i  ,j,k-1),nd(i  ,j,k  ));

                cfd2(i,j,k)= (v1*v2+v3*v4)/2.0;
            }
        }
    }
    CELLFACE &cfd3 = cellFaceData->at(2);
    for (int i = 1; i < n1+1; ++i)
    {
        for (int j = 1; j < n2+1; ++j)
        {
            for (int k = 0; k < n3+1; ++k)
            {
                Vector v1(nd(i-1,j-1,k),nd(i-1,j  ,k));
                Vector v2(nd(i  ,j-1,k),nd(i-1,j-1,k));
                Vector v3(nd(i  ,j  ,k),nd(i  ,j-1,k));
                Vector v4(nd(i-1,j  ,k),nd(i  ,j  ,k));

                cfd3(i,j,k)= (v1*v2+v3*v4)/2.0;
            }
        }
    }

    //VOLUME IS CALCULATED FOLLOWING THE WORK PRESENTED IN http://www.dept.ku.edu/~cfdku/papers/1999-AIAAJ.pdf
    CELLVOLUME &cvd = cellVolumeData->at(0);
    for (int i = 1; i < n1+1; ++i)
    {
        for (int j = 1; j < n2+1; ++j)
        {
            for (int k = 1; k < n3+1; ++k)
            {
                Vertex Vert(0,0,0);
                Vector V1(nd(i-1,j-1,k-1),Vert);
                Vector V2(nd(i  ,j  ,k  ),Vert);

                cvd(i,j,k) = -(VectorDot(cfd1(i-1,j,k),V1)+VectorDot(cfd2(i,j-1,k),V1)+VectorDot(cfd3(i,j,k-1),V1));
                cvd(i,j,k) += (VectorDot(cfd1(i  ,j,k),V2)+VectorDot(cfd2(i,j  ,k),V2)+VectorDot(cfd3(i,j,k  ),V2));
                cvd(i,j,k)/=3.0;
            }
        }
    }
}
