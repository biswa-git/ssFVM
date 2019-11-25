//notHidden
#pragma once
#include "essential.h"
#include "CGNS_Zone.h"
#include "CGNS_Interface.h"
#include "CGNS_BoundaryCondition.h"
namespace MESH
{
    class Base
    {
    public:
        static Base* New(unsigned long n);
        ~Base();
        void SetName(const string &);
        string GetName();

        void SetCellDim(int);
        int  GetCellDim();

        void SetPhysDim(int);
        int  GetPhysDim();

        void SetNumOfZone(const int &);
        int GetNumOfZone();

        Zone* GetZoneById(const int &);
        Zone* GetZoneFromName(const string&);

        void SetNumOfInterface(const int &);
        int GetNumOfInterface();

        void AddInterface(const Interface&);
        Interface* GetInterfaceById(const int&);

        void AddBoundaryCondition(const BoundaryCondition& BC);
        vector<BoundaryCondition>* GetBoundaryConditionList();
    protected:
        Base();
    private:
        string name;
        int cellDim;
        int physDim;
        int numOfZone;
        Zone *zone;
        int numOfInterface;

        vector<Interface> interfaces;
        vector<BoundaryCondition> boundaryCondition;
    };
}
