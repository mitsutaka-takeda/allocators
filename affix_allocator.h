#pragma once

#include <gsl.h>

#include "block.h"
#include "detail/unique_resource.h"

namespace allocator {
    
    template <typename A, typename Prefix, typename Suffix = void>
    class affix_allocator {
    public:
        static constexpr std::size_t prefix_size = std::is_void<Prefix>::value ? 0 : sizeof(Prefix);
        static constexpr std::size_t suffix_size = std::is_void<Suffix>::value ? 0 : sizeof(Suffix);

        allocator::block 
        allocate(size_t n){
            // Allocate the memory with extra bytes for Prefix and Suffix object.
            auto b = allocator::detail::make_unique_resource(parent_.allocate(prefix_size + n + suffix_size),
                                                             [this](allocator::block b_) { parent_.deallocate(b_); });
            
            if(b->ptr != nullptr){
                if(prefix_size != 0) {
                    new (b->ptr) Prefix{};
                }
                if(suffix_size != 0) {
                    new (b->ptr + prefix_size + n) Suffix{};
                }
            }

            return trim_prefix_and_suffix(b.release());
        }
        
        allocator::block 
        allocateAll(){
            return trim_prefix_and_suffix(parent_.allocateAll());
        }
        
        void
        deallocate(allocator::block b){
            Expects(owns(extend_prefix_and_suffix(b)));
            
            parent_.deallocate(extend_prefix_and_suffix(b));
        }
        
        void
        deallocateAll(){
        }
        
        bool owns(allocator::block b){
            return parent_.owns(extend_prefix_and_suffix(b));
        }

    private:
        static allocator::block
        trim_prefix_and_suffix(allocator::block b){
            return {static_cast<Guide::byte*>(b.ptr) + prefix_size, b.length - prefix_size - suffix_size};
        }

        static allocator::block
        extend_prefix_and_suffix(allocator::block b){
            return {static_cast<Guide::byte*>(b.ptr) - prefix_size, b.length + prefix_size + suffix_size};
        }

        A parent_;
    };
} // namespace allocator
