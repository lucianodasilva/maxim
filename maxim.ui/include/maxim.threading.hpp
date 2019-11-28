#pragma once
#ifndef	_maxim_threading_hpp_
#define _maxim_threading_hpp_

#include <atomic>

namespace maxim {
    namespace threading {

        struct spin_mutex {
        public:
            inline void lock() {
                while (_lockless_flag.test_and_set(std::memory_order_acquire)) {}
            }

            inline void unlock() {
                _lockless_flag.clear(std::memory_order_release);
            }
        private:
            std::atomic_flag _lockless_flag = ATOMIC_FLAG_INIT;

        };
	}
}

#endif