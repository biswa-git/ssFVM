//notHidden
#pragma once
#include "essential.h"
#include "CGNS_Base.h"
#include "CGNS_Reader.h"
#include "Solution.h"

class Reader;
class Solution;
class Mesh
{
public:
    static Mesh* New();
    ~Mesh();

    void SetCellCount(int);
    int GetCellCount();
    map<int,int> GetDataMapReadOnly();
    map<int,int>& GetDataMapReadWrite();
    void SetSolution(Solution*);
    Solution* GetSolution();
    Reader* GetReader();

    void SetNumberOfBases(int);
    int  GetNumberOfBases();

    void  SetNumberOfZone(const int &, const int &);
    void  SetZoneInfo(const int &, const int &, const string &, int*);

    MESH::Base* GetBaseById(const int &);

    void Preprocess();
protected:
    Mesh();

private:

    int CellCount;

    map<int,int> dataMap;
    int dataCount;
    bool initFlag;

    //CGNS
    Solution *solution = nullptr;
    Reader *reader = nullptr;

    int numberOfBases;
    MESH::Base *base;

    string meshOutputFileName;
};
