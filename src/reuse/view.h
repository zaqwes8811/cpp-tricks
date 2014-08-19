#ifndef REUSE_VIEW_H_
#define REUSE_VIEW_H_

#include <vector>
#include <ostream>
#include <algorithm>

namespace view {
template <typename T>
class Action {
public:
  explicit Action(std::ostream* o_) : o(o_) { }
  void operator()(const T& elem) const {
    *o << elem << " ";
  }
  std::ostream* const o;
}; 
 
template <typename T>
std::ostream& operator<<(std::ostream& o, const std::vector<T>& a) 
{
  std::for_each(a.begin(), a.end(), Action<T>(&o));
  o << std::endl;
  return o;
}

}

#endif  // REUSE_VIEW_H_