#include "Solution.h"
int main()
{
      /*
    Solution S;
    Mesh* M = S.GetMesh();
    Block *B1 = new Block(nx,ny,nz);
    Block *B2 = new Block(1,2,5);
    M->push(B1);
    M->push(B2);
    M->push(new Block(5,5,5));
    M->push(new Block(5,5,5));

    S.AddData(PRESSURE);
    S.AddData(TEMPERATURE);

    firstIndex i;
    secondIndex j;
    thirdIndex k;

    B1->GetData(TEMPERATURE)=j+k;
    B2->GetData(PRESSURE)=i;
    cout<<B2->GetData(PRESSURE)<<endl;
    cout<<B1->GetData(TEMPERATURE)<<endl;
    Data(S,1,PRESSURE) = i+2;
    Data(S,1,PRESSURE)(1,1,1) = 200;
    cout<<Data(S,1,PRESSURE)(1,1,1)<<endl;
    cout<<M->GetCellCount()<<endl;

    for (int i=0;i<4;i++) cout<<M->GetBlock()->at(sizeType(i))->GetGlobalIdStart()<<endl;
*/

    //Solution *S = Solution::New();
    SmartPtr<Solution>S(Solution::New());
    Reader *R = S->GetMesh()->GetReader();
    R->SetFilePath("../../input/test1.cgns");
    R->ReadCGNS();

    int nbase = R->GetMesh()->GetNumberOfBases();
    for (int baseId=1;baseId<=nbase;++baseId)
    {
        MESH::Base *base = R->GetMesh()->GetBaseById(baseId);

        ofstream myfile;
        myfile.open ("grid.dat");
        myfile << "variables = x,y,z"<<endl;
        myfile.close();
        cout<<"base name: "<<base->GetName()<<" has "<<base->GetNumOfInterface()<<" interfaces"<<endl;
        int nzone = base->GetNumOfZone();

        for (int zoneId=1;zoneId<=nzone;++zoneId)
        {
            MESH::Zone *zone = base->GetZoneById(zoneId);
            zone->PrintGrid();
            zone->AddData(PRESSURE);
            zone->AddData(TEMPERATURE);
            zone->AddData(VOLUMEFRACTION);
            zone->AddData(MASSFLUX1);
            zone->AddData(MASSFLUX2);
        }

        vector<MESH::BoundaryCondition> *boundaryCondition = base->GetBoundaryConditionList();
        vector<MESH::BoundaryCondition>::iterator ptr;

        for (ptr = boundaryCondition->begin(); ptr < boundaryCondition->end(); ++ptr)
        {
            cout<<"hey "<<ptr->GetName()<<"        "<<ptr->GetTypeId()<<endl;
        }
    }

    //S->GetMesh()->push(Block::New(20,20,20));
    //S->GetMesh()->Initialize();

    //S->AddData(TEMPERATURE);

    //USER_DEFINED_VARIABLE MYVARIABLE = 500;
    //S->AddData(MYVARIABLE);

/*
    Block *B = S->GetMesh()->GetBlock()->at(0);

    const int nx = B->GetSize(1);
    const int ny = B->GetSize(2);
    const int nz = B->GetSize(3);

    LIS_INT LIS_MatSize,gn,is,ie,iter;
    S->AddMatrix();
    LIS_VECTOR b,x;
    LIS_SOLVER solver;
    LIS_Comm LIS_COMM_WORLD_alias = 1;
    LIS_MatSize = nx*ny*nz;
    lis_initialize(&argc,&argv);
    lis_matrix_create(LIS_COMM_WORLD_alias,&S->GetMatrix(0));
    lis_matrix_set_size(S->GetMatrix(0),0,LIS_MatSize);
    lis_matrix_get_size(S->GetMatrix(0),&LIS_MatSize,&gn);
    lis_matrix_get_range(S->GetMatrix(0),&is,&ie);
    lis_vector_duplicate(S->GetMatrix(0),&b);
    lis_vector_duplicate(S->GetMatrix(0),&x);
    int p,e,w,n,s,t,bb;

    for (int i = 0; i < nx; ++i)
    {
        for (int j = 0; j < ny; ++j)
        {
            for (int k = 0; k < nz; ++k)
            {
                p = i    *ny*nz + j*nz + k;
                e = (i+1)*ny*nz + j*nz + k;
                w = (i-1)*ny*nz + j*nz + k;
                n = i    *ny*nz + (j+1)*nz + k;
                s = i    *ny*nz + (j-1)*nz + k;
                t = i    *ny*nz + j    *nz + k+1;
                bb = i   *ny*nz + j    *nz + k-1;

                lis_matrix_set_value(LIS_INS_VALUE,p,p,-6.0,S->GetMatrix(0));
                if( i>0   ) lis_matrix_set_value(LIS_INS_VALUE,p,w,1.0,S->GetMatrix(0));
                if( j>0   ) lis_matrix_set_value(LIS_INS_VALUE,p,s,1.0,S->GetMatrix(0));
                if( k>0   ) lis_matrix_set_value(LIS_INS_VALUE,p,bb,1.0,S->GetMatrix(0));
                if( i<nx-1) lis_matrix_set_value(LIS_INS_VALUE,p,e,1.0,S->GetMatrix(0));
                if( j<ny-1) lis_matrix_set_value(LIS_INS_VALUE,p,n,1.0,S->GetMatrix(0));
                if( k<nz-1) lis_matrix_set_value(LIS_INS_VALUE,p,t,1.0,S->GetMatrix(0));
                if( k==nz-1)
                {
                    lis_vector_set_value(LIS_INS_VALUE,p,-100.0,b);
                }
                else
                {
                    lis_vector_set_value(LIS_INS_VALUE,p,0.0,b);
                }
            }
        }
    }

    lis_matrix_set_type(S->GetMatrix(0),LIS_MATRIX_CSR);
    lis_matrix_assemble(S->GetMatrix(0));
    lis_solver_create(&solver);
    lis_solver_set_option("-i bicgstab -p ilu",solver);
    lis_solver_set_option("-tol 1.0e-12",solver);
    lis_solver_set_option("-maxiter 10000",solver);
    lis_solve(S->GetMatrix(0),b,x,solver);

    lis_solver_get_iter(solver,&iter);
    printf("number of iterations = %d\n",iter);


    DATA &temperature = Data(S,0,TEMPERATURE);
    for (int i = 0; i < nx; ++i)
    {
        for (int j = 0; j < ny; ++j)
        {
            for (int k = 0; k < nz; ++k)
            {
                p = i    *ny*nz + j*nz + k;
                lis_vector_get_value(x,p,&temperature(i,j,k));
            }
        }
    }


    lis_matrix_destroy(S->GetMatrix(0));
    lis_vector_destroy(b);
    lis_vector_destroy(x);
    lis_solver_destroy(solver);
    lis_finalize();

    ofstream myfile;
    myfile.open ("temperature.dat");
    myfile << "title = temperature"<<endl;
    myfile << "variables = x,y,z,temperature"<<endl;
    myfile << "zone"<<" i = "<<nx+1<<" j = "<<ny+1<<" k = "<<nz+1<<" DATAPACKING=BLOCK VARLOCATION=(4=CELLCENTERED)"<<endl;



    for (int k = 0; k < nz+1; ++k)
    {
        for (int j = 0; j < ny+1; ++j)
        {
            for (int i = 0; i < nx+1; ++i)
            {
                myfile << i << endl;
            }
        }
    }
    for (int k = 0; k < nz+1; ++k)
    {
        for (int j = 0; j < ny+1; ++j)
        {
            for (int i = 0; i < nx+1; ++i)
            {
                myfile << j << endl;
            }
        }
    }
    for (int k = 0; k < nz+1; ++k)
    {
        for (int j = 0; j < ny+1; ++j)
        {
            for (int i = 0; i < nx+1; ++i)
            {
                myfile << k << endl;
            }
        }
    }
    for (int k = 0; k < nz; ++k)
    {
        for (int j = 0; j < ny; ++j)
        {
            for (int i = 0; i < nx; ++i)
            {
                myfile <<temperature(i,j,k)<<endl;
            }
        }
    }

    myfile.close();
*/
/*

    Vertex *p0 = Vertex::New( 0,0,0);
    Vertex *p1 = Vertex::New(-1,1,0);
    Vertex *p2 = Vertex::New(1,1,0);

    Vector *v1 = Vector::New(p1,p0);
    Vector *v2 = Vector::New(p2,p0);
    Vector v3;
    v3 = (*v1)*(*v2);
    v3.Print();
    cout<<v3.GetNorm()<<endl;
    delete p0;delete p1;delete p2;delete v1;delete v2;
*/
    return 0;
}
