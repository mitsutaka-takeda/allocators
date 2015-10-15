#pragma once

#include <cstddef>

namespace allocator {
    
    struct block {
        void* ptr;
        size_t length;
    };

} // namespace allocator
