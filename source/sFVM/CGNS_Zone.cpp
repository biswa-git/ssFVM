#include "CGNS_Zone.h"

MESH::Zone::Zone()
{
    //
}

MESH::Zone::~Zone()
{
    //
}

void MESH::Zone::SetName(const string &S)
{
    name = S;
}

string MESH::Zone::GetName()
{
    return name;
}

void MESH::Zone::SetSize(int arr[])
{
    nodeSize[0] = arr[0]; nodeSize[1] = arr[1]; nodeSize[2] = arr[2];
    cellSize[0] = arr[3]; cellSize[1] = arr[4]; cellSize[2] = arr[5];
    nodeData.push_back  (Array<Vertex,3>(Range(0,cellSize[0]),Range(0,cellSize[1]),Range(0,cellSize[2])));
    faceData.push_back  (Array<Vector,3>(Range(0,cellSize[0]),Range(1,cellSize[1]),Range(1,cellSize[2])));
    faceData.push_back  (Array<Vector,3>(Range(1,cellSize[0]),Range(0,cellSize[1]),Range(1,cellSize[2])));
    faceData.push_back  (Array<Vector,3>(Range(1,cellSize[0]),Range(1,cellSize[1]),Range(0,cellSize[2])));
    volumeData.push_back(Array<double,3>(Range(1,cellSize[0]),Range(1,cellSize[1]),Range(1,cellSize[2])));
}

int* MESH::Zone::GetNodeSize()
{
    return nodeSize;
}

void MESH::Zone::AddData(unsigned long enumId)
{
    try
    {
        unsigned long index = dataMap.size();
        map<int,int>::iterator itr;

        if(static_cast<int>(index) > 0)
        {
            for(itr = dataMap.begin(); itr!=dataMap.end();++itr)
            {
                if(itr->first == static_cast<int>(enumId))
                {
                    //throw warning
                    throw runtime_error("Duplicate data allocation is not allowed!");
                }
            }
        }

        dataMap.insert({enumId,index});
        data.push_back(Array<double,3>(Range(1,cellSize[0]),Range(1,cellSize[1]),Range(1,cellSize[2])));
    }
    catch (exception &e)
    {
        cerr << "ERROR: " << e.what() << endl;
        cerr << "Type: " << typeid(e).name() << endl;
        exit(EXIT_FAILURE);
    }
}

Array<double,3>& MESH::Zone::GetData(unsigned long enumId)
{
     map<int,int>::iterator itr = dataMap.find(static_cast<int>(enumId));
     return data.at(static_cast<unsigned long>(itr->second));
}

Array<Vertex,3>& MESH::Zone::GetNodeData()
{
    return nodeData.at(0);
}

Array<Vector,3>& MESH::Zone::GetFaceData(unsigned long faceID)
{
    try
    {
        if(faceID > 2) throw runtime_error("Invalid faceID called!");
        return faceData.at(faceID);
    }
    catch (exception &e)
    {
        cerr << "ERROR: " << e.what() << endl;
        cerr << "Type: " << typeid(e).name() << endl;
        exit(EXIT_FAILURE);
    }
}

Array<double,3>& MESH::Zone::GetVolumeData()
{
        return volumeData.at(0);
}

void MESH::Zone::CalculateGeometryData()
{
    Array<Vertex,3> &nd = nodeData.at(0);
    int n1 = nodeSize[0];
    int n2 = nodeSize[1];
    int n3 = nodeSize[2];

    {
        Array<Vector,3> &fd = faceData.at(0);
         for (int i = 0; i < n1; ++i)
         {
             for (int j = 1; j < n2; ++j)
             {
                 for (int k = 1; k < n3; ++k)
                 {
                     Vector v1(nd(i,j-1,k-1),nd(i,j-1,k  ));
                     Vector v2(nd(i,j  ,k-1),nd(i,j-1,k-1));
                     Vector v3(nd(i,j  ,k  ),nd(i,j  ,k-1));
                     Vector v4(nd(i,j-1,k  ),nd(i,j  ,k  ));

                     fd(i,j,k)= (v1*v2+v3*v4)/2.0;
                 }
             }
         }
    }

    {
        Array<Vector,3> &fd = faceData.at(1);
         for (int i = 1; i < n1; ++i)
         {
             for (int j = 0; j < n2; ++j)
             {
                 for (int k = 1; k < n3; ++k)
                 {
                     Vector v1(nd(i-1,j,k-1),nd(i  ,j,k-1));
                     Vector v2(nd(i-1,j,k  ),nd(i-1,j,k-1));
                     Vector v3(nd(i  ,j,k  ),nd(i-1,j,k  ));
                     Vector v4(nd(i  ,j,k-1),nd(i  ,j,k  ));

                     fd(i,j,k)= (v1*v2+v3*v4)/2.0;
                 }
             }
         }
    }

    {
        Array<Vector,3> &fd = faceData.at(2);
         for (int i = 1; i < n1; ++i)
         {
             for (int j = 1; j < n2; ++j)
             {
                 for (int k = 0; k < n3; ++k)
                 {
                     Vector v1(nd(i-1,j-1,k),nd(i-1,j  ,k));
                     Vector v2(nd(i  ,j-1,k),nd(i-1,j-1,k));
                     Vector v3(nd(i  ,j  ,k),nd(i  ,j-1,k));
                     Vector v4(nd(i-1,j  ,k),nd(i  ,j  ,k));

                     fd(i,j,k)= (v1*v2+v3*v4)/2.0;
                 }
             }
         }
    }

    {
        //VOLUME IS CALCULATED FOLLOWING THE WORK PRESENTED IN http://www.dept.ku.edu/~cfdku/papers/1999-AIAAJ.pdf
        Array<double,3> &vd = volumeData.at(0);
        for (int i = 1; i < n1; ++i)
        {
            for (int j = 1; j < n2; ++j)
            {
                for (int k = 1; k < n3; ++k)
                {
                    Vertex Vert(0,0,0);
                    Vector V1(nd(i-1,j-1,k-1),Vert);
                    Vector V2(nd(i  ,j  ,k  ),Vert);

                    vd(i,j,k) = -(DotProduct(faceData.at(0)(i-1,j,k),V1)+
                                  DotProduct(faceData.at(1)(i,j-1,k),V1)+
                                  DotProduct(faceData.at(2)(i,j,k-1),V1));

                    vd(i,j,k) += (DotProduct(faceData.at(0)(i  ,j,k),V2)+
                                  DotProduct(faceData.at(1)(i,j  ,k),V2)+
                                  DotProduct(faceData.at(2)(i,j,k  ),V2));

                    vd(i,j,k)/=3.0;
                }
            }
        }
    }
}

void MESH::Zone::PrintGrid()
{
    ofstream myfile;
    myfile.open ("grid.dat",ios::app);
    myfile << "zone"<<" i = "<<nodeSize[0]<<" j = "<<nodeSize[1]<<" k = "<<nodeSize[2]<<endl;

    for (int k=0;k<nodeSize[2];++k)
    {
        for (int j=0;j<nodeSize[1];++j)
        {
            for (int i=0;i<nodeSize[0];++i)
            {
                double *vertex = nodeData.at(0)(i,j,k).GetVertex();
                myfile << vertex[0] << " " << vertex[1] << " " << vertex[2] << endl;
            }
        }
    }

    myfile.close();
}


map<int,MESH::Interface*>& MESH::Zone::GetInterfaceList()
{
    return interfaceList;
}
