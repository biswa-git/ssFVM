#pragma once
#include "essential.h"
#include "Mesh.h"

class Mesh;
class Solution
{
public:
    static Solution* New();
    ~Solution();
    Mesh* GetMesh();
    //Reader* GetReader();
    //void AddData(int);
    //void AddMatrix();
    //LIS_MATRIX& GetMatrix(int);

private:
    //CGNSREADER

    Mesh *mesh = nullptr;
    //Reader *reader = nullptr;
    //vector<LIS_MATRIX> matrix;
    //vector<LIS_VECTOR> vec;
protected:
    Solution();
};
