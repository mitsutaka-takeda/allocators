#pragma once

#include <array>

#include "block.h"
#include "gsl.h"

namespace allocator {

    template <size_t s>
    class stack_allocator
    {
    public:
        allocator::block
        allocate(size_t n){
            const auto rounded = roundToAligned(n);
            if(rounded > (std::cend(d_) - p_)){
                return {nullptr, 0};
            }

            block b{p_, n};
            p_ += rounded;
            Ensures(std::cbegin(d_) <= p_ && p_ <= std::cend(d_));
            return b;
        }

        allocator::block
        allocateAll(){
            const auto end = std::cend(d_);
            if(p_ == end) {
                return {nullptr, 0};
            }

            const auto ptr = p_;
            p_ = end;
            return {ptr, end - ptr};
        }

        void
        deallocate(allocator::block b){
            Ensures(owns(b));
            if(b.ptr + roundToAligned(b.length) == p_){
                p_ = b.ptr;
            }
        }

        void
        deallocateAll(){
            p_ = std::begin(d_);
        }

        bool
        owns(allocator::block b){
            return (std::cbegin(d_) <= b.ptr) && (b.ptr < p_);
        }

    private:

        size_t
        roundToAligned(size_t n){
            // todo : implement.
            return n;
        }

        std::array<Guide::byte, s> d_;
        Guide::byte* p_;
    };


} // namespace allocator
