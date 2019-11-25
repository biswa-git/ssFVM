#include "Mesh.h"

Mesh* Mesh::New()
{
    return new Mesh();
}

Mesh::Mesh():CellCount(0),dataCount(0),initFlag(false),reader(Reader::New()),
             numberOfBases(0),base(nullptr),meshOutputFileName("meshfile.dat")
{
    reader->SetMesh(this);
}

Mesh::~Mesh()
{
    if (reader != nullptr)
    {
        delete reader;
        reader = nullptr;
    }

    if (base != nullptr)
    {
        delete[] base;
        base = nullptr;
    }
 }

void Mesh::SetCellCount(int n)
{
    CellCount = n;
}

int Mesh::GetCellCount()
{
    return CellCount;
}

map<int,int> Mesh::GetDataMapReadOnly(){return dataMap;}
map<int,int>& Mesh::GetDataMapReadWrite(){return dataMap;}

void Mesh::SetSolution(Solution* solution)
{
    this->solution = solution;
}

Solution* Mesh::GetSolution()
{
    return solution;
}

Reader* Mesh::GetReader()
{
    return reader;
}

void Mesh::SetNumberOfBases(int n)
{
    try
    {
        numberOfBases = n;
        if(base == nullptr)
        {
            unsigned long nbases = static_cast<unsigned long>(numberOfBases);
            base = MESH::Base::New(nbases);
        }
        else
        {
            throw runtime_error("Number of Bases can be set only once!");
        }
    }
    catch (exception &e)
    {
        cerr << "ERROR: " << e.what() << endl;
        cerr << "Type: " << typeid(e).name() << endl;
        exit(EXIT_FAILURE);
    }
}
int  Mesh::GetNumberOfBases()
{
    return numberOfBases;
}

void  Mesh::SetNumberOfZone(const int &B, const int &n)
{
    if(base!=nullptr && B>0 && B<=numberOfBases)
    {
        base[B-1].SetNumOfZone(n);
    }
}

void  Mesh::SetZoneInfo(const int &B, const int &Z, const string &S, int* size)
{
    if(base!=nullptr && B>0 && B<=numberOfBases)
    {
        GetBaseById(B)->GetZoneById(Z)->SetName(S);
    //fix for 2D
    //--------------------------------------
        if(GetBaseById(B)->GetCellDim()==2)
        {
            int temp[2];
            temp[0]=size[2];
            temp[1]=size[3];

            size[2]=2      ;
            size[3]=temp[0];
            size[4]=temp[1];
            size[5]=1      ;
        }
    //--------------------------------------
        MESH::Zone *zone = GetBaseById(B)->GetZoneById(Z);
        zone->SetSize(size);
    }
}

MESH::Base* Mesh::GetBaseById(const int &n)
{
    try
    {
        if(base!=nullptr)
        {
            return &base[n-1];
        }
        else
        {
            throw runtime_error("Zone is not assigned yet!");
        }
    }
    catch (exception &e)
    {
        cerr << "ERROR: " << e.what() << endl;
        cerr << "Type: " << typeid(e).name() << endl;
        exit(EXIT_FAILURE);
    }
}

void Mesh::Preprocess()
{

}
