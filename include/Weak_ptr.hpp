#ifndef WEAK_PTR
#define WEAK_PTR

#include "Shared_ptr.hpp" // Include SharedPtr definition
// When submitting to ACMOJ, please replace this with the content inside Shared_ptr.hpp

template <typename T>
class WeakPtr {
  friend class SharedPtr<T>;
  private:
    T* _ptr;
    ControlBlock* _block;
    void release(){
      if(_block != nullptr){
        _block->weak_cnt--;
        if(_block->weak_cnt == 0 && _block->shared_cnt == 0){
          delete _block;
        }
      }
      _ptr = nullptr;
      _block = nullptr;
    }
  public:
    // Constructors
    WeakPtr(){
      _ptr = nullptr;
      _block = nullptr;
    }                         // Default constructor
    WeakPtr(const SharedPtr<T>& temp) noexcept{
      _ptr = temp._ptr;
      _block = temp._block;
      if(_block){
        _block->weak_cnt++;
      }
    } // Constructor from SharedPtr
    WeakPtr(const WeakPtr& temp) noexcept{
      _ptr = temp._ptr;
      _block = temp._block;
      if(_block){
        _block->weak_cnt++;
      }
    }      // Copy constructor
    WeakPtr(WeakPtr&& temp) noexcept{
      _ptr = temp._ptr;
      _block = temp._block;
      temp._ptr = nullptr;
      temp._block = nullptr;
    }           // Move constructor

    // Destructor
    ~WeakPtr(){
      release();
    }

    // Assignment operators
    WeakPtr& operator=(const WeakPtr& temp) noexcept{
      if(this == &temp)return *this;
      release();
      _ptr = temp._ptr;
      _block = temp._block;
      if(_block)_block->weak_cnt++;
      return *this;
    }      // Copy assignment operator
    WeakPtr& operator=(const SharedPtr<T>& temp) noexcept{
      release();
      _ptr = temp._ptr;
      _block = temp._block;
      if(_block)_block->weak_cnt++;
      return *this;
    } // Assignment from SharedPtr
    WeakPtr& operator=(WeakPtr&& temp) noexcept{
      if(this == &temp)return *this;
      release();
      _ptr = temp._ptr;
      _block = temp._block;
      temp._ptr = nullptr;
      temp._block = nullptr;
      return *this;
    }           // Move assignment operator

    // Basic operations
    int use_count() const{
      if(_block)return _block->shared_cnt;
      return 0;
    }     // Get the number of shared owners
    explicit operator bool() const{
      return !expired();
    }   
    SharedPtr<T> lock() const{
      return SharedPtr<T>(*this);
    } // Get a SharedPtr to the managed object
    bool expired() const{
      if(_block){
        return _block->shared_cnt == 0;
      }
      return true;
    }      // Check if the managed object was deleted
    void reset(){
      release();
    }        // Reset to empty state

    // Utility functions
    void swap(WeakPtr& other) noexcept{
      std::swap(_ptr, other._ptr);
      std::swap(_block, other._block);
    } // Swap with another WeakPtr

};

// Non-member swap function
template <typename T>
void swap(WeakPtr<T>& lhs, WeakPtr<T>& rhs) noexcept{
  lhs.swap(rhs);
}

#endif // WEAK_PTR
