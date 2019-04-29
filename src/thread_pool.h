#ifndef MFO_THREAD_POOL_HEADER
#define MFO_THREAD_POOL_HEADER
#pragma once

#include <condition_variable>
#include <mutex>
#include <thread>
#include <unordered_map>

namespace mfo {
    class thread_pool {
    public:
        thread_pool();
        ~thread_pool() noexcept;

        template<class Function, class... Args>
        void start_thread(Function&& f, Args&&... args);
    private:
        void gc();

        std::thread m_gc;
        bool m_kill_gc;

        std::unordered_map<std::thread::id, std::thread> m_threads;
        std::mutex m_mtx;
        std::condition_variable m_gc_wakeup;
    };
}

template<class Function, class... Args>
void mfo::thread_pool::start_thread(Function&& f, Args&&... args) {
    std::thread newT {std::forward<Function>(f), std::forward<Args>(args)...};

    std::unique_lock lck {m_mtx};
    m_threads[newT.get_id()] = std::move(newT);
    m_gc_wakeup.notify_one();
}

#endif //MFO_THREAD_POOL_HEADER