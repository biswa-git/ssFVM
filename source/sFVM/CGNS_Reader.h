#pragma once
#include "essential.h"
#include "cgnslib.h"

#include "CGNS_Interface.h"
#include "Mesh.h"

class Mesh;
class Reader
{
public:
    static Reader* New();
    static Reader* New(string const &);
    virtual ~Reader();

    void SetMesh(Mesh*);
    Mesh* GetMesh();

    void SetName(string const &);
    void SetFilePath(string const &);

    void Open();
    void Close();
    void ReadBase();
    void ReadZone();
    void ReadInterface();
    void ReadCGNS();

private:
    string name;
    string filePath;
    int FileId;

    Mesh* mesh = nullptr;
    bool isFileOpened;
    bool FilePathExist;
    bool isNubBaseRead;

protected:
    Reader();
    Reader(string const &);
};
