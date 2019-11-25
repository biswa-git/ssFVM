//notHidden
#pragma once
#include "essential.h"
#include "Vector.h"
#include "Vertex.h"
#include "CGNS_Interface.h"
#include "DataStruct.h"

namespace MESH
{
class Interface;
    class Zone
    {
    public:
        Zone();
        ~Zone();
        void SetName(const string &);
        string GetName();

        void SetSize(int[]);
        int* GetNodeSize(); //copy instead of passing

        void AllocateNodeData();

        void AddData(unsigned long);
        Array<double,3>& GetData(unsigned long);
        Array<Vertex,3>& GetNodeData();
        Array<Vector,3>& GetFaceData(unsigned long);
        Array<double,3>& GetVolumeData();

        void CalculateGeometryData();

        void PrintGrid();
        map<int,Interface*>& GetInterfaceList();
    protected:

    private:
        string name;
        int nodeSize[3];
        int cellSize[3];

        vector<CELLVOLUMEDATA> data;
        map<int,int> dataMap;
        vector<NODEDATA> nodeData;
        vector<CELLFACEDATA> faceData;
        vector<CELLVOLUMEDATA>volumeData;

        map<int,Interface*> interfaceList;
    };
}
