#ifndef MAX_HPP
#define MAX_HPP

// https://en.cppreference.com/w/cpp/language/namespace
namespace myops{
  template <class T>
  T max(T x, T y)
  {
  	return (x > y) ? x : y; 
	// 1-line conditional statement
	// (<condition>) ? (return if <condition> True) : (return if <condition> False)
  }
// https://en.cppreference.com/w/cpp/language/function_template
// https://en.cppreference.com/w/cpp/language/class_template


}

#endif // MAX_HPP
