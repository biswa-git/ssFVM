#include "CGNS_Base.h"

MESH::Base* MESH::Base::New(unsigned long n)
{
    return new Base[static_cast<unsigned long>(n)];
}

MESH::Base::Base():zone(nullptr)
{
    //
}

MESH::Base::~Base()
{
    if(zone!=nullptr)
    {
        delete[] zone;
        zone = nullptr;
    }
}

void MESH::Base::SetName(const string &S)
{
    name = S;
}

string MESH::Base::GetName()
{
    return name;
}

void MESH::Base::SetCellDim(int n)
{
    cellDim = n;
}

int  MESH::Base::GetCellDim()
{
    return cellDim;
}

void MESH::Base::SetPhysDim(int n)
{
    physDim = n;
}

int  MESH::Base::GetPhysDim()
{
    return physDim;
}

void MESH::Base::SetNumOfZone(const int &n)
{
    numOfZone = n;
    if(zone!=nullptr) delete [] zone;
    zone = new MESH::Zone[static_cast<unsigned long>(n)]();
}

int MESH::Base::GetNumOfZone()
{
    return  numOfZone;
}

MESH::Zone* MESH::Base::GetZoneById(const int &n)
{
    try
    {
        if(zone!=nullptr)
        {
            return &zone[n-1];
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

MESH::Zone *MESH::Base::GetZoneFromName(const string& name)
{
    try
    {
        if(zone == nullptr) throw runtime_error("Zone is not present. Aborting...");
        for(int zoneId=1;zoneId<=numOfZone;++zoneId)
        {
            if(name == GetZoneById(zoneId)->GetName()) return GetZoneById(zoneId);
        }
        throw runtime_error("Zone name doesn't exist. Aborting...");
    }
    catch (exception &e)
    {
        cerr << "ERROR: " << e.what() << endl;
        cerr << "Type: " << typeid(e).name() << endl;
        exit(EXIT_FAILURE);
    }
}

void MESH::Base::SetNumOfInterface(const int &n)
{
    numOfInterface = n;
}

int MESH::Base::GetNumOfInterface()
{
    return numOfInterface;
}

void MESH::Base::AddInterface(const Interface& I)
{
    interfaces.push_back(I);
}

MESH::Interface*  MESH::Base::GetInterfaceById(const int &n)
{
    return &interfaces[static_cast<size_t>(n)-1];
}

vector<MESH::BoundaryCondition>* MESH::Base::GetBoundaryConditionList()
{
    return &boundaryCondition;
}
