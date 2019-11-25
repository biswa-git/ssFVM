#include "CGNS_Interface.h"

MESH::Interface::Interface():zone(nullptr), donor(nullptr)
{

}

MESH::Interface::Interface(const Interface& I):zone(nullptr), donor(nullptr)
{
    connectionName = I.connectionName;
    zoneName = I.zoneName;
    donorName = I.donorName;
    for (size_t i=0;i<6;++i)
    {
        zoneRange[i] = I.zoneRange[i];
        donorRange[i] = I.donorRange[i];
    }
    for (size_t i=0;i<3;++i)
    {
        transform[i] = I.transform[i];
    }
}

MESH::Interface::~Interface()
{
    //code
}


void MESH::Interface::SetConnectionName(const string& S)
{
    connectionName = S;
}

string MESH::Interface::GetConnectionName()
{
    return  connectionName;
}

void MESH::Interface::SetZoneName(const string& S)
{
    zoneName = S;
}

string MESH::Interface::GetZoneName()
{
    return  zoneName;
}

void MESH::Interface::SetDonorName(const string& S)
{
    donorName = S;
}

string MESH::Interface::GetDonorName()
{
    return donorName;
}

void MESH::Interface::SetRange(int I[])
{
    for (int i=0;i<6;++i) zoneRange[i]=I[i];
}

void MESH::Interface::SetDonorRange(int I[])
{
    for (int i=0;i<6;++i) donorRange[i]=I[i];
}

int* MESH::Interface::GetZoneRange()
{
    return zoneRange;
}
int* MESH::Interface::GetDonorRange()
{
    return donorRange;
}

void MESH::Interface::SetTransform(int I[])
{
    for (int i=0;i<3;++i) transform[i]=I[i];
}

void MESH::Interface::SetZone(MESH::Zone* z)
{
    zone = z;
}
void MESH::Interface::SetDonor(MESH::Zone* d)
{
    donor = d;
}

void MESH::Interface::SetZoneFaceId(const int& n)
{
    zoneFaceId = n;
}

void MESH::Interface::SetDonorFaceId(const int& n)
{
    donorFaceId = n;
}
