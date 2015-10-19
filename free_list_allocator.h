#pragma once

#include <cstddef>

#include "block.h"
#include "gsl.h"

namespace allocator {

    // This allocator allocates an object of the size between 'min' and 'max' inclusive by some
    // allocator of type 'A'. The allocation by 'A' is done in a batch of the size 'batch_size'.
    // This allocator allocates at most 'capacity' number of objects of the size 'max'.
    template <typename A, size_t min, size_t max, size_t batch_size, size_t capacity>
    class free_list_allocator
    {
        static_assert(min <= max, "min should be less than or equal to max.");
        static_assert(capacity % batch_size == 0, "capacity should be a multiple of batch_size. This assumption makes the implementation easier.");
    public:
        allocator::block 
        allocate(size_t n){
            if(n < min || max < n) {
                return parent_.allocate(n);
            }

            if(root_ != nullptr){
                allocator::block b{root_, n};
                root_ = root_->next;
                --size_;
                Ensures(b.ptr != nullptr && b.length == n);
                return b;
            }

            auto b = parent_.allocate(max);
            return {b.ptr, n};
        }
        
        void
        deallocate(allocator::block b){
            if(b.length < min || max < b.length) {
                return parent_.deallocate(b);
            }

            if(capacity < size_){
                return parent_.deallocate({b.ptr, max});
            } else {
                const auto p = static_cast<Node*>(b.ptr);
                p.next = root_;
                root_ = p;
                ++size_;
            }
        }
        
        allocator::block 
        allocateAll(){
            // @todo I'm not sure if this is the correct implementation. Can Free List support this operation?
            return {nullptr, 0};
        }
        
        void
        deallocateAll(){
            // @todo I'm not sure if this is the correct implementation. Can Free List support this operation?
            // This implementation is depends on allocateAll, that is, deallocateAll must undo what allocateAll has done.
        }
        
        bool 
        owns(allocator::block b){
            return ((min <= b.length) && (b.length <= max)) || parent_.owns(b);
        }

    private:

        struct Node { Node* next; } * root_ = nullptr;
        size_t size_ = 0; // size of free list.
        A parent_;
    };

} // namespace allocator
