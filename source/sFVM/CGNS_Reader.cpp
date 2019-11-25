#include "CGNS_Reader.h"
#define CGNS_NAME_MAX_SIZE 33
Reader* Reader::New()
{
    return new Reader();
}

Reader* Reader::New(string const &objName)
{
    return new Reader(objName);
}

Reader::Reader():isFileOpened(false),FilePathExist(false),isNubBaseRead(false)
{
    //
}

Reader::Reader(string const &objName):name(objName),isFileOpened(false),FilePathExist(false),isNubBaseRead(false)
{
    //
}

Reader::~Reader()
{
    //
}

void Reader::SetMesh(Mesh* mesh)
{
    this->mesh = mesh;
}

Mesh* Reader::GetMesh()
{
    return mesh;
}

void Reader::SetName(string const &objName)
{
    name = objName;
}

void Reader::SetFilePath(const string &fPath)
{
    filePath = fPath;
    FilePathExist = true;
}


void Reader::Open()
{
    try
    {
        const char* fname = filePath.c_str();
        bool ierr = false;
        ierr = cg_open(fname, CG_MODE_READ,&FileId);
        if(!ierr)
        {
            float version;
            ierr = cg_version(FileId, &version);
            if(!ierr)
            {
                cout<<"File: \""<<filePath<<"\" opened in CGNS "<<version<<endl;
                isFileOpened = true;
            }
            else
            {
                throw runtime_error("Failed to parse file version!");
            }
        }
        else
        {
            throw runtime_error("Failed to open file!");
        }
    }
    catch (exception &e)
    {
        cerr << "ERROR: " << e.what() << endl;
        cerr << "Type: " << typeid(e).name() << endl;
        cg_error_exit();
    }
}

void Reader::ReadBase() //NEED TRY CATCH
{
    try
    {
        bool ierr = false;
        int nbases;

        ierr = cg_nbases(FileId, &nbases);
        if(ierr) throw runtime_error("Failed to read nbases!");

        // SETTING NBASES IN MESH
        if(nbases == 1)
        {
            mesh->SetNumberOfBases(nbases);
        }
        else
        {
            throw runtime_error("CGNS with multiple bases are not supported at this moment!");
        }

        for (int baseId=1;baseId<=mesh->GetNumberOfBases();++baseId)
        {
            char basename[CGNS_NAME_MAX_SIZE];
            int cell_dim;
            int phys_dim;
            ierr = cg_base_read(FileId, baseId, basename, &cell_dim, &phys_dim);
            if(ierr) throw runtime_error("Failed to read base!");
            string BaseName(basename);
            mesh->GetBaseById(baseId)->SetName(BaseName);
            mesh->GetBaseById(baseId)->SetCellDim(cell_dim);
            mesh->GetBaseById(baseId)->SetPhysDim(phys_dim);

            int n1to1_global;
            ierr = cg_n1to1_global(FileId, baseId, &n1to1_global);
            if(ierr) throw runtime_error("Failed to read Interface Number!");

            mesh->GetBaseById(baseId)->SetNumOfInterface(n1to1_global);
        }
    }
    catch (exception &e)
    {
        cerr << "ERROR: " << e.what() << endl;
        cerr << "Type: " << typeid(e).name() << endl;
        cg_error_exit();
    }
}

void Reader::ReadZone()
{
    try
    {
        bool ierr = false;
        for (int baseId=1;baseId<=mesh->GetNumberOfBases();++baseId)
        {
            MESH::Base *base = mesh->GetBaseById(baseId);
            int nzones;
            ierr = cg_nzones(FileId, baseId, &nzones);
            if(ierr) throw runtime_error("Failed to read base!");

            mesh->SetNumberOfZone(baseId,nzones);

            for (int zoneId=1;zoneId<=nzones;++zoneId)
            {
                MESH::Zone *zone = base->GetZoneById(zoneId);
                ZoneType_t zonetype;
                ierr = cg_zone_type(FileId, baseId, zoneId, &zonetype);
                if(ierr) throw runtime_error("Failed to read zonetype!");
                if(zonetype != 2) throw runtime_error("Only structured zone allowed in CGNS file!");

                char zonename[CGNS_NAME_MAX_SIZE];
                cgsize_t size[9];
                ierr = cg_zone_read(FileId, baseId, zoneId, zonename, size);
                if(ierr) throw runtime_error("Failed to read zone!");

                string ZoneName(zonename);
                mesh->SetZoneInfo(baseId,zoneId,ZoneName,size);

            //COORD READ
            //----------------------------------------------------------------------------------
                int* nodeSize = mesh->GetBaseById(baseId)->GetZoneById(zoneId)->GetNodeSize();
                const cgsize_t n1 = nodeSize[0];
                const cgsize_t n2 = nodeSize[1];
                const cgsize_t n3 = nodeSize[2];

                const unsigned long n1_ul = static_cast<unsigned long>(nodeSize[0]);
                const unsigned long n2_ul = static_cast<unsigned long>(nodeSize[1]);
                const unsigned long n3_ul = static_cast<unsigned long>(nodeSize[2]);
            //--------------------------------------------------------------------------------------//
            //    IT IS "SUPER-IMPORTANT" TO STORE THE COORD TEMPORARILY IN HEAP, SINCE THE ARRAY   //
            //    SIZE IS QUITE LARGE AND MAY RESULT IN STACK OVERFLOW!                             //
            //--------------------------------------------------------------------------------------//
                double *tempXCoordStorage = new double[n1_ul*n2_ul*n3_ul];// freed
                double *tempYCoordStorage = new double[n1_ul*n2_ul*n3_ul];// freed
                double *tempZCoordStorage = new double[n1_ul*n2_ul*n3_ul];// freed

                cgsize_t irmin[3]{ 1, 1, 1};
                cgsize_t irmax[3]{n1,n2,n3};

                ierr=cg_coord_read(FileId, baseId, zoneId,"CoordinateX", RealDouble,irmin,irmax,tempXCoordStorage);
                if(ierr) throw runtime_error("Error while reading Xcoord");
                ierr=cg_coord_read(FileId, baseId, zoneId,"CoordinateY", RealDouble,irmin,irmax,tempYCoordStorage);
                if(ierr) throw runtime_error("Error while reading Ycoord");
                if(base->GetCellDim()==3)
                {
                    ierr=cg_coord_read(FileId, baseId, zoneId,"CoordinateZ", RealDouble,irmin,irmax,tempZCoordStorage);
                    if(ierr) throw runtime_error("Error while reading Zcoord");
                }

                for (int i=0;i<n1;++i) {
                    for (int j=0;j<n2;++j) {
                        for (int k=0;k<n3;++k) {
                            Array<Vertex,3> &nodeData =zone->GetNodeData();
                            int nodeId = i+j*n1+k*n1*n2;
                            nodeData(i,j,k).SetVertex(tempXCoordStorage[nodeId],
                                                      tempYCoordStorage[nodeId],
                                                      tempZCoordStorage[nodeId]);
                        }
                    }
                }

                delete[] tempXCoordStorage;
                delete[] tempYCoordStorage;
                delete[] tempZCoordStorage;

                zone->CalculateGeometryData();
            //END OF COORD READ
            //----------------------------------------------------------------------------------

            //READING BOUNDARY CONDITION INFO
            //----------------------------------------------------------------------------------
                int nbocos;
                ierr = cg_nbocos(FileId, baseId, zoneId, &nbocos);
                if(ierr) throw runtime_error("Error while reading Number of BC");
                for (int BC_Id=1;BC_Id<=nbocos;++BC_Id)
                {
                    char boconame[CGNS_NAME_MAX_SIZE];
                    BCType_t bocotype;
                    PointSetType_t ptset_type;
                    cgsize_t npnts;
                    int NormalIndex;
                    cgsize_t NormalListSize;
                    DataType_t NormalDataType;
                    int ndataset;
                    ierr = cg_boco_info(FileId, baseId, zoneId, BC_Id, boconame,
                                        &bocotype, &ptset_type, &npnts,
                                        &NormalIndex, &NormalListSize,
                                        &NormalDataType, &ndataset);
                    if(ierr) throw runtime_error("Error while reading BC info");
                    //cout<<boconame<<" "<<bocotype<<endl;

                    vector<MESH::BoundaryCondition> *boundaryConditionList = base->GetBoundaryConditionList();
                    vector<MESH::BoundaryCondition>::iterator ptr;
                    MESH::BoundaryCondition *boundaryCondition = nullptr;

                    bool isBocoExist{false};
                    for (ptr = boundaryConditionList->begin(); ptr < boundaryConditionList->end(); ptr++)
                    {
                        if(ptr->GetName() == boconame)
                        {
                            isBocoExist = true;
                            //assign the boco to face
                            break;
                        }
                    }
                    if(!isBocoExist)
                    {
                        MESH::BoundaryCondition BC;
                        BC.SetName(boconame);
                        BC.SetTypeId(bocotype);
                        boundaryConditionList->push_back(BC);
                        //assign the boco to face
                    }
                }
            //READING BOUNDARY CONDITION INFO
            //----------------------------------------------------------------------------------
            }
        }
    }
    catch (exception &e)
    {
        cerr << "ERROR: " << e.what() << endl;
        cerr << "Type: " << typeid(e).name() << endl;
        cg_error_exit();
    }
}

void Reader::ReadInterface() //editing
{
    try
    {
        bool ierr = false;
        for (int baseId=1;baseId<=mesh->GetNumberOfBases();++baseId)
        {
            MESH::Base *base = mesh->GetBaseById(baseId);
            int n1to1_global;
            ierr = cg_n1to1_global(FileId, baseId, &n1to1_global);
            if(ierr)throw runtime_error("Error: reading cg_n1to1_global!");

            unsigned long n1to1_global_int = static_cast<unsigned long>(n1to1_global);
            char     **connectname = new char*    [n1to1_global_int];
            char     **zonename    = new char*    [n1to1_global_int];
            char     **donorname   = new char*    [n1to1_global_int];
            cgsize_t **range       = new cgsize_t*[n1to1_global_int];
            cgsize_t **donor_range = new cgsize_t*[n1to1_global_int];
            int      **transform   = new int*     [n1to1_global_int];

            for (int intId=1;intId<=n1to1_global;++intId)
            {
                int intId_array =intId-1;
                connectname[intId_array] = new char[CGNS_NAME_MAX_SIZE];
                zonename[intId_array]    = new char[CGNS_NAME_MAX_SIZE];
                donorname[intId_array]   = new char[CGNS_NAME_MAX_SIZE];
                range[intId_array]       = new cgsize_t[6];
                donor_range[intId_array] = new cgsize_t[6];
                transform[intId_array]   = new int[3];
            }

            ierr = cg_1to1_read_global(FileId, baseId, connectname,zonename,donorname,range,donor_range,transform);
            if(ierr)throw runtime_error("Error: reading cg_1to1_read_global");

            for (int intId=1;intId<=n1to1_global;++intId)
            {
                int intId_array =intId-1;
                MESH::Interface interface;
                interface.SetConnectionName(static_cast<string>(connectname[intId_array]));
                interface.SetZoneName(static_cast<string>(zonename[intId_array]));
                interface.SetDonorName(static_cast<string>(donorname[intId_array]));
                interface.SetRange(range[intId_array]);
                interface.SetDonorRange(donor_range[intId_array]);
                interface.SetTransform(transform[intId_array]);

                base->AddInterface(interface);
             }

            for (int intId=1;intId<=n1to1_global;++intId)
            {
                int intId_array =intId-1;
                delete [] connectname[intId_array];
                delete [] zonename[intId_array];
                delete [] donorname[intId_array];
                delete [] range[intId_array];
                delete [] donor_range[intId_array];
                delete [] transform[intId_array];
            }
            delete [] connectname;
            delete [] zonename;
            delete [] donorname;
            delete [] range;
            delete [] donor_range;
            delete [] transform;

            int numberOfInterface = base->GetNumOfInterface();
            for(int intId=1;intId<=numberOfInterface;++intId)
            {
                MESH::Interface *interface = base->GetInterfaceById(intId);
                int *zoneRange = interface->GetZoneRange();
                int *donorRange = interface->GetDonorRange();

                int val=0;

                for(int i=0;i<3;++i)
                {
                    if(zoneRange[i]==zoneRange[i+3])
                    {
                        val = i+1;
                        if(zoneRange[i]==1)
                        {
                            val *= -1;
                        }
                    }
                }
                interface->SetZoneFaceId(val);

                for(int i=0;i<3;++i)
                {
                    if(donorRange[i]==donorRange[i+3])
                    {
                        val = i+1;
                        if(donorRange[i]==1)
                        {
                            val *= -1;
                        }
                    }
                }
                interface->SetDonorFaceId(val);

                interface->SetZone(base->GetZoneFromName(interface->GetZoneName()));
                interface->SetDonor(base->GetZoneFromName(interface->GetDonorName()));
            }
        }
    }
    catch (exception &e)
    {
        cerr << "ERROR: " << e.what() << endl;
        cerr << "Type: " << typeid(e).name() << endl;
        cg_error_exit();
    }
}

void Reader::Close()
{
    try
    {
        bool ierr = false;

        ierr = cg_close(FileId);
        if(ierr) throw runtime_error("Failed to close CGNS file!");
        isFileOpened = false;
    }
    catch (exception &e)
    {
        cerr << "ERROR: " << e.what() << endl;
        cerr << "Type: " << typeid(e).name() << endl;
        cg_error_exit();
    }
}

void Reader::ReadCGNS()
{
    try
    {
        if(!FilePathExist) throw runtime_error("File path need to be set before reading!");
        Open();
        ReadBase();
        ReadZone();
        ReadInterface();
        Close();
    }
    catch (exception &e)
    {
        cerr << "ERROR: " << e.what() << endl;
        cerr << "Type: " << typeid(e).name() << endl;
        exit(EXIT_FAILURE);
    }

}
