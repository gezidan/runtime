//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: libcpp-has-no-threads
//  ... assertion fails line 38

// <atomic>

// template <class T>
//     bool
//     atomic_compare_exchange_weak_explicit(volatile atomic<T>* obj, T* expc,
//                                           T desr,
//                                           memory_order s, memory_order f);
// 
// template <class T>
//     bool
//     atomic_compare_exchange_weak_explicit(atomic<T>* obj, T* expc, T desr,
//                                           memory_order s, memory_order f);

#include <atomic>
#include <type_traits>
#include <cassert>

#include <cmpxchg_loop.h>

template <class T>
void
test()
{
    {
        typedef std::atomic<T> A;
        A a;
        T t(T(1));
        std::atomic_init(&a, t);
        assert(c_cmpxchg_weak_loop(&a, &t, T(2),
               std::memory_order_seq_cst, std::memory_order_seq_cst) == true);
        assert(a == T(2));
        assert(t == T(1));
        assert(std::atomic_compare_exchange_weak_explicit(&a, &t, T(3),
               std::memory_order_seq_cst, std::memory_order_seq_cst) == false);
        assert(a == T(2));
        assert(t == T(2));
    }
    {
        typedef std::atomic<T> A;
        volatile A a;
        T t(T(1));
        std::atomic_init(&a, t);
        assert(c_cmpxchg_weak_loop(&a, &t, T(2),
               std::memory_order_seq_cst, std::memory_order_seq_cst) == true);
        assert(a == T(2));
        assert(t == T(1));
        assert(std::atomic_compare_exchange_weak_explicit(&a, &t, T(3),
               std::memory_order_seq_cst, std::memory_order_seq_cst) == false);
        assert(a == T(2));
        assert(t == T(2));
    }
}

struct A
{
    int i;

    explicit A(int d = 0) noexcept {i=d;}

    friend bool operator==(const A& x, const A& y)
        {return x.i == y.i;}
};

int main()
{
    test<A>();
    test<char>();
    test<signed char>();
    test<unsigned char>();
    test<short>();
    test<unsigned short>();
    test<int>();
    test<unsigned int>();
    test<long>();
    test<unsigned long>();
    test<long long>();
    test<unsigned long long>();
    test<wchar_t>();
#ifndef _LIBCPP_HAS_NO_UNICODE_CHARS
    test<char16_t>();
    test<char32_t>();
#endif  // _LIBCPP_HAS_NO_UNICODE_CHARS
    test<int*>();
    test<const int*>();
}
