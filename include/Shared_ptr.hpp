#ifndef SHARED_PTR
#define SHARED_PTR

#include "Unique_ptr.hpp" // Include UniquePtr definition
// When submitting to ACMOJ, please replace this with the content inside Unique_ptr.hpp
struct ControlBlock
{
  int shared_cnt;
  int weak_cnt;
};

template <typename T>
class SharedPtr {
  friend class WeakPtr<T>;
  private:
    // Add whatever you want to add.
    T* _ptr;
    ControlBlock* _block;

    void release(){
      if(_block != nullptr){
        _block->shared_cnt--;
        if(_block->shared_cnt == 0){
          delete _ptr;
        }
        _block->weak_cnt--;
        if(_block->weak_cnt == 0){
          delete _block;
        }
      }
      _ptr = nullptr;
      _block = nullptr;
    }
  public:
    // Constructors
    SharedPtr(){
      _ptr = nullptr;
      _block = nullptr;
    }                         // Default constructor
    explicit SharedPtr(T* p){
      _ptr = p;
      if(_ptr){
        _block = new ControlBlock;
        _block->shared_cnt = 1;
        _block->weak_cnt = 1;
      }
      else _block = nullptr;
    }                     // Constructor from raw pointer
    SharedPtr(UniquePtr<T>& p){
      _ptr = p.release();
      if(_ptr){
        _block = new ControlBlock;
        _block->shared_cnt = 1;
        _block->weak_cnt = 1;
      }
      else _block = nullptr;
    }
    SharedPtr(UniquePtr<T>&& p){
      _ptr = p.release();
      if(_ptr){
        _block = new ControlBlock;
        _block->shared_cnt = 1;
        _block->weak_cnt = 1;
      }
      else _block = nullptr;
    }
    SharedPtr(const SharedPtr& tmp){
      _ptr = tmp._ptr;
      _block = tmp._block;
      if (_block != nullptr){
        _block->shared_cnt++;
        _block->weak_cnt++;
      }
    }     // Copy constructor
    SharedPtr(SharedPtr&& tmp) noexcept{
      _ptr = tmp._ptr;
      _block = tmp._block;
      tmp._ptr = nullptr;
      tmp._block = nullptr;
    }          // Move constructor
    // How about constructor from WeakPtr?
    SharedPtr(const WeakPtr<T>& p) noexcept{
      if(p.expired()){
        _ptr = nullptr;
        _block = nullptr;
      }
      else{
        _ptr = p._ptr;
        _block = p._block;
        if(_block){
          _block->shared_cnt++;
          _block->weak_cnt++;
        }
      }
      
    }
    // Destructor
    ~SharedPtr(){
      release();
    }

    // Assignment operators
    SharedPtr& operator=(const SharedPtr& temp){
      if(this == &temp)return *this;
      release();
      _ptr = temp._ptr;
      _block = temp._block;
      if(_block != nullptr){
        _block -> shared_cnt++;
        _block -> weak_cnt++;
      }
      return *this;
    }     // Copy assignment operator
    SharedPtr& operator=(SharedPtr&& temp) noexcept{
      if(this == &temp)return *this;
      release();
      _ptr = temp._ptr;
      _block = temp._block;
      temp._ptr = nullptr;
      temp._block = nullptr;
      return *this;
    } // Move assignment operator

    // Basic operations
    int use_count() const{
      if(_block != nullptr)return _block->shared_cnt;
      return 0;
    }       // Get the number of shared owners
    T* get() const{
      return _ptr;
    }              // Get the raw pointer
    explicit operator bool() const{
      return _ptr != nullptr;
    }       // Check if the pointer is not null
    T& operator*() const{
      return *_ptr;
    }        // Dereference operator
    T* operator->() const{
      return _ptr;
    }       // Arrow operator
    void reset(){
      release();
    }          // Reset to empty state
    void reset(T* p){
      if(p == _ptr)return;
      release();
      _ptr = p;
      if(_ptr){
        _block = new ControlBlock;
        _block->shared_cnt = 1;
        _block->weak_cnt = 1;
      }
    }      // Reset with a new raw pointer
};


// Then implement the make_shared function
// I think I'd better leave you to design the prototype :)

// template <?>
//? make_shared(?);
template<typename T, typename... Args>
SharedPtr<T> make_shared(Args&&... args){
  return SharedPtr<T>(new T(std::forward<Args>(args)...));
}
#endif // SHARED_PTR