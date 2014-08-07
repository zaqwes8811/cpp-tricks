#include <string>
#include <iostream>

using namespace std;
 
 
namespace gotchas {
  
// G47
template <int maxlen>
class NBString {
public:
  explicit NBString( const char *name ) : name_(name) { }
  // . . .
private:
  std::string name_;
  size_t len_;
  char s_[maxlen];
};

void fun() {
  NBString<32> a("Hello");
  NBString<32> b( a );  
  b = a;
}


// G82
// "For the community of C++ programmers, a const member function implements "logical" constness."
// Физическая и логическая константность
//  http://alenacpp.blogspot.ru/2005/10/mutable-constcast.html
//
class BoundedString {
public:
  explicit BoundedString( int len ) : buf_(0) { }
  // . . .
  size_t length() const { 
    //if ()
      //len_ = // lazy does't work
    return len_; 
    
  }
  void set( char c );
  void wipe() const { buf_[0] = '\0';}  // !! does't change obj !!
  
  // нельзя применить к неконстантным
  bool operator <( const BoundedString &rhs ) { 
    cout << "no const" << endl;
    return true; }  // не гарантирует, что не поменятся сам
    
  bool operator >=( const BoundedString &rhs ) const { return true; }

private:
  char * const buf_;  // non-logical const
  // mutable
  int len_;
  size_t maxLen_;
};

// G83
class Role {};
class Employee {
public:
  virtual ~Employee();
  void adoptRole( Role *newRole ); // take ownership
  void shareRole( const Role *sharedRole ); // does not own
  void copyRole( const Role *roleToCopy ); // set role to clone
  const Role *getRole() const;
  // . . .
};

 
}  // namespace..

int main() {
  using namespace gotchas;
  const BoundedString s(5);
  //const  // not compiled
  BoundedString d(7);
  //s.set('a');  // не даст установить
  d < s;
  d.length();
  
  return 0;
}