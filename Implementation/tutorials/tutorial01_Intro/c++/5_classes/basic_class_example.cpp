#include <iostream>
#include "simpleclass.hpp"

int main()
{
    std::cout << "No. objects: " << SimpleClass::get_object_counter() << std::endl; // 0 objects
    SimpleClass obj;
    std::cout << "No. objects: " << SimpleClass::get_object_counter() << std::endl; // 1 object
    
    // create a new scope to demonstrate the destructor:
    {
        SimpleClass tmp_obj;
        std::cout << "No. objects: " << SimpleClass::get_object_counter() << std::endl; // 2 objects
    }   // tmp_obj is destroyed when it is out of scope
    
    std::cout << "No. objects: " << SimpleClass::get_object_counter() << std::endl; // 1 object
    
    // how to access member functions:
    std::cout << "Member of obj: " << obj.get_member() << std::endl;    // 0
    obj.set_member(5);
    std::cout << "Member of obj: " << obj.get_member() << std::endl;    // 5
    
    return 0;
}
