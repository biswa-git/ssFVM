#include "Solution.h"

Solution* Solution::New()
{
    return new Solution;
}

Solution::Solution():mesh(Mesh::New())
{
    mesh->SetSolution(this);
}

Solution::~Solution()
{
    if (mesh != nullptr)
    {
        delete mesh;
        mesh = nullptr;
    }

    //matrix.clear();
}

Mesh* Solution::GetMesh()
{
    return mesh;
}
