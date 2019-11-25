#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <cmath>
#include <string>
#include<stdexcept>
#include <typeinfo>
#include <vector>

#include "blitz/array.h"
#include "lis.h"
#include "SmartPointer.h"
#include "DataName.h"

using namespace std;
using namespace blitz;

typedef Array<double,3> DATA;
//-----------------------------------------------------------------


#define SetMacro(name,type) \
virtual void Set##name (type _arg) \
{ \
    if (this->name != _arg) \
    { \
        this->name = _arg; \
    } \
}


//-----------------------------------------------------------------


#define GetMacro(name,type) \
virtual type Get##name () \
{ \
    return this->name; \
}


//-----------------------------------------------------------------


#define FreeObjMacro(name) \
if(name!=nullptr){  \
    delete name; \
    name = nullptr; \
}


//-----------------------------------------------------------------


#define FreeArrMacro1p(name) \
if(name!=nullptr){ \
    delete[] name; \
    name = nullptr; \
}


//-----------------------------------------------------------------


#define ErrorMacro(x) \
cout<<"ERROR: "  <<x<<endl<<"       in file: "  <<__FILE__<<endl<<"       at line no: "  <<__LINE__<<endl<<"       in function: "  <<__func__<<endl \
<<endl<<endl<<"                             EXITING..."<<endl;



//-----------------------------------------------------------------


#define WarningMacro(x) \
cout<<"WARNING: "<<x<<endl<<"         in file: "<<__FILE__<<endl<<"         at line no: "<<__LINE__<<endl<<"         in function: "<<__func__<<endl;


//-----------------------------------------------------------------


#define MessageMacro(x) \
cout<<x<<endl;


//-----------------------------------------------------------------
#define DATA(SolutionName,BlockID,VariableID) \
SolutionName->GetMesh()->GetBlock()->at(BlockID)->GetData(VariableID)

#define CHECK(CGNS_ARG) \
ierr = CGNS_ARG; \
if(ierr) throw "ERROR:";
