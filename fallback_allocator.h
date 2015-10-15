#pragma once

#include "block.h"
#include "gsl.h"

namespace allocator {
    
    template <typename Primary, typename Fallback>
    class fallback_allocator : private Primary, private Fallback {
    public:
        allocator::block 
        allocate(size_t n){
            auto ret = Primary::allocate(n);
            if(ret.ptr == nullptr) { 
                ret = Fallback::allocate(n);
            }

            return ret;
        }
        
        allocator::block 
        allocateAll(){
            Primary::allocateAll();
            Fallback::allocateAll();
        }
        
        void
        deallocate(allocator::block b){
            Ensures(Primary::owns(b) || Fallback::owns(b));

            if(Primary::owns(b)){
                Primary::deallocate(b);
            } else {
                Fallback::deallocate(b);
            }
        }
        
        void
        deallocateAll(){
            Primary::deallocateAll();
            Fallback::deallocateAll();
        }
        
        bool owns(block b){
            return Primary::owns(b) || Fallback::owns(b);
        }
    };
} // namespace allocator
