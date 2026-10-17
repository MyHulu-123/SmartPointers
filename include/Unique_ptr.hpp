#ifndef UNIQUE_PTR
#define UNIQUE_PTR

#include <utility>
template <typename T> class SharedPtr;
template <typename T> class WeakPtr;
template <typename T>

class UniquePtr {
  private:
    // Add whatever you want to add.
    T* _ptr;
  public:
    // Constructors
    UniquePtr(){
      _ptr = nullptr;
    }                          // Default constructor
    explicit UniquePtr(T* p){            
      _ptr = p;
    }             // Constructor from raw pointer
    UniquePtr(UniquePtr& other) = delete; // Copy constructor is deleted
    UniquePtr(UniquePtr&& other) noexcept : _ptr(other._ptr){
      other._ptr = nullptr;
    }         // Move constructor

    // Destructor
    ~UniquePtr(){
      delete _ptr;
      _ptr = nullptr;
    }

    // Assignment operators
    UniquePtr& operator=(UniquePtr& other) = delete; // Copy assignment operator is deleted
    UniquePtr& operator=(UniquePtr&& other) noexcept{
      if(this == &other)return *this;
      delete _ptr;
      _ptr = other._ptr;
      other._ptr = nullptr;
      return *this;
    } // Move assignment operator

    // Basic operations
    T* get() const{
      return _ptr;
    }                 // Get the raw pointer
    explicit operator bool() const{
      return (_ptr != nullptr);
    } // Check if the pointer is not null
    T& operator*() const{
      return *_ptr;
    }           // Dereference operator
    T* operator->() const{
      return _ptr;
    }          // Arrow operator
    void reset(){
      delete _ptr;
      _ptr = nullptr;
    }                   // Reset to empty state
    void reset(T* p){
      if(p == _ptr)return;
      delete _ptr;
      _ptr = p;
    }               // Reset with a new raw pointer
    T* release(){
      T* _tmp = _ptr;
      _ptr = nullptr;
      return _tmp;
    }                   // Release ownership of the managed object
};


// make_unique function
template<typename T, typename... Args>
UniquePtr<T> make_unique(Args&&... args){
  return UniquePtr<T>(new T(std::forward<Args>(args)...));
}
#endif // UNIQUE_PTR
