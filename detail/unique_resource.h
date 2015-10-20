// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4189.pdf
// Sample implementation of unique_resource from n4189.

#pragma once

#include <type_traits>
#include <utility>

namespace allocator {
    namespace detail {

        template<typename R,typename D>
        class unique_resource{
            R resource;
            D deleter;
            bool execute_on_destruction; // exposition only
        public:
            // construction
            explicit
            unique_resource(R && resource_, D && deleter_, bool shouldrun_=true) noexcept
                : resource(std::move(resource_))
                , deleter(std::move(deleter_))
                , execute_on_destruction{shouldrun_}{}
            // move
            unique_resource(unique_resource &&other) noexcept
                :resource(std::move(other.resource))
                ,deleter(std::move(other.deleter))
                ,execute_on_destruction{other.execute_on_destruction}{
                other.release();
            }

            // no copies!
            unique_resource& operator=(unique_resource const &)=delete;
            unique_resource(unique_resource const &)=delete;

            unique_resource&
            operator=(unique_resource &&other) noexcept(noexcept(this->reset())) {
                this->reset();
                this->deleter=std::move(other.deleter);
                this->resource=std::move(other.resource);
                this->execute_on_destruction=other.execute_on_destruction;
                other.release();
                return *this;
            }
            // resource release
            ~unique_resource() noexcept(noexcept(this->reset())){
                this->reset();
            }

            void reset() noexcept(noexcept(this->get_deleter()(resource))) {
                if (execute_on_destruction) {
                    this->execute_on_destruction = false;
                    this->get_deleter()(resource);
                }
            }

            void reset(R && newresource) noexcept(noexcept(this->reset())) {
                this->reset();
                this->resource = std::move(newresource);
                this->execute_on_destruction = true;
            }

            R const & release() noexcept {
                this->execute_on_destruction = false;
                return this->get();
            }

            // resource access
            R const & get() const noexcept {
                return this->resource;
            }
            operator R const &() const noexcept {
                return this->resource;
            }

            R operator->() const noexcept {
                return this->resource;
            }

            std::add_lvalue_reference_t<std::remove_pointer_t<R>>
            operator*() const {
                return * this->resource;
            }

            // deleter access
            const D &
            get_deleter() const noexcept {
                return this->deleter;
            }
        };

        //factories
        template<typename R,typename D>
        auto
        make_unique_resource( R && r,D &&d) noexcept {
            return unique_resource<R,std::remove_reference_t<D>>(
                std::move(r)
                ,std::forward<std::remove_reference_t<D>>(d)
                ,true);
        }
        template<typename R,typename D>
        auto
        make_unique_resource_checked(R r, R invalid, D d ) noexcept {
            bool shouldrun = not bool(r == invalid);
            return unique_resource<R,D>(std::move(r), std::move(d), shouldrun);
        }
        
    } // namespace detail
} // namespace allocator
