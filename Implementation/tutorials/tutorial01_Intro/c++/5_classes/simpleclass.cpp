#include "simpleclass.hpp"

// static member variables should be initialised in the implementation file:
SimpleClass::counter_type SimpleClass::obj_counter_ = 0;

SimpleClass::SimpleClass(simple_type value) : x_(value)
{
    // increase the object counter to keep track of how many objects we have created so far
    ++obj_counter_;
}

// don't copy paste code, but call the constructor we have already implemented!
SimpleClass::SimpleClass() : SimpleClass(0) {}

SimpleClass::~SimpleClass()
{
    // clean up actions should be here:
    // most importantly, free all dynamically allocated memory, unregister MPI types
    // (see later in this course or next semester for details about MPI)
    
    // in this example, decrease the object counter when an object is destroyed
    --obj_counter_;
}

void SimpleClass::set_member(simple_type value)
{
    x_ = value;
}

SimpleClass::simple_type SimpleClass::get_member()
{
    return x_;
}

SimpleClass::counter_type SimpleClass::get_object_counter()
{
    return obj_counter_;
}
