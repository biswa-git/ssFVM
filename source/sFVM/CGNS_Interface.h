#pragma once
#include "essential.h"
#include "CGNS_Zone.h"

namespace MESH
{
    class Zone;
    class Interface
    {
    public:
        Interface();
        Interface(const Interface&);
        ~Interface();

        void SetConnectionName(const string&);
        string GetConnectionName();
        void SetZoneName(const string&);
        string GetZoneName();
        void SetDonorName(const string&);
        string GetDonorName();
        void SetRange(int[]);
        void SetDonorRange(int[]);
        int* GetZoneRange();
        int* GetDonorRange();
        void SetTransform(int[]);
        void SetZone(Zone*);
        void SetDonor(Zone*);
        void SetZoneFaceId(const int&);
        void SetDonorFaceId(const int&);

    private:
        string connectionName;
        string zoneName;
        string donorName;
        int zoneRange[6];
        int donorRange[6];
        int transform[3];

        Zone *zone, *donor;
        int zoneFaceId, donorFaceId;
        int zoneCellRange[6];
        int donorCellRange[6];
    };
}
