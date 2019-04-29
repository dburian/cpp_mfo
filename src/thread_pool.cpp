#include "thread_pool.h"

mfo::thread_pool::thread_pool() : m_kill_gc{false}, m_threads{}, m_mtx{}, m_gc_wakeup{}, m_gc{gc} {}

mfo::thread_pool::~thread_pool() noexcept {
    std::lock_guard lck {m_mtx};
    m_kill_gc = true;
    m_gc_wakeup.notify_all();
}

void mfo::thread_pool::gc() {
    std::unique_lock lck{m_mtx};
    std::size_t lastSize = m_threads.size();    // simple protection against wake-up calls, after which gc would not have anything to do

    while (true) {
        while(!m_kill_gc && m_threads.size() == lastSize) m_gc_wakeup.wait(lck);

        if(m_kill_gc) return; 

        lastSize = m_threads.size();

        for(auto&& thread: m_threads) 
            if(!thread.second.joinable()) m_threads.extract(thread.first); // invalidates only current iterator

    }
}