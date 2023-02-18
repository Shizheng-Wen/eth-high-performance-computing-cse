#ifndef SIMPLECLASS_HPP
#define SIMPLECLASS_HPP

// https://www.cplusplus.com/doc/tutorial/classes/
class SimpleClass
{
public:
    using simple_type = int;
    using counter_type = unsigned int;
    
    // constructors
    SimpleClass(simple_type value);
    SimpleClass();
    
    // destructor
    ~SimpleClass();
    
    // member functions
    void set_member(simple_type value);
    simple_type get_member();
    
    // static functions
    static counter_type get_object_counter();
    
private:
    // a member variable
    simple_type x_;
    
    // a static variable
    static counter_type obj_counter_;
};  // don't forget the semicolon!

#endif // SIMPLECLASS_HPP
