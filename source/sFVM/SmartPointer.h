#pragma once

template <class T>
class SmartPtr
{
   T *ptr;
public:
   explicit SmartPtr(T *p = nullptr) { ptr = p; }
   ~SmartPtr()
   {
       if(ptr != nullptr)
       {
           delete ptr; ptr = nullptr;
       }
   }

   T & operator *  () { return *ptr; }
   T * operator &  () { return  ptr; }
   T * operator -> () { return  ptr; }
   void operator = (T *p) {if(ptr != nullptr) delete ptr; ptr = p;}
   T* Get(){return ptr;}
   void Clear(){delete ptr; ptr = nullptr;}
};
