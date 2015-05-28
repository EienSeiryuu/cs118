#include <string>
#include<sstream>


template <typename T>
std::string to_string_patched(T value)
{
  std::ostringstream os ;
  os << value ;
  return os.str() ;
}