#pragma once
#include "essential.h"
#include "DataStruct.h"

struct Block
{
private:
// THIS IS WHERE BIG CHUNK OF DATA ARE STORED
/*------------------------------------BLOCK DATA------------------------------------*/
    vector<DATA>       *data   = nullptr;
    NODE nodeData              = nullptr;
    CELLFACE   *cellFaceData   = nullptr;
    CELLVOLUME *cellVolumeData = nullptr;
/*----------------------------------------------------------------------------------*/
};
