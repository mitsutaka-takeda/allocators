#pragma once

#include "gsl.h"
#include "block.h"

namespace allocator {
    class null_allocator {
    public:
        allocator::block 
        allocate(size_t){
            const auto ptr = nullptr;
            Ensures(ptr == nullptr);
            return {ptr, 0};
        }

        allocator::block 
        allocateAll(){
            const auto ptr = nullptr;
            Ensures(ptr == nullptr);
            return {ptr, 0};
        }
        
        void
        deallocate(block b){
            Expects(b.ptr == nullptr);
        }

        // Nothing to deallocate.
        void
        deallocateAll(){
        }

        // null_allocator owns nothing.
        bool owns(block){
            const auto ret = false;
            Ensures(!ret);
            return ret;
        }
    };

} // namespace allocator
