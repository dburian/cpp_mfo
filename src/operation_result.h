#ifndef MFO_OPERATION_RESULT_HEADER
#define MFO_OPERATION_RESULT_HEADER
#pragma once

#include <future>
#include <memory>
#include <vector>

namespace mfo {
    template<class ReturnType, class ArgType>
    class job;

    template<class ReturnType, class ArgType>
    class operation_result {
    public:
        using return_type = ReturnType;

        operation_result(std::unique_ptr<job<ReturnType, ArgType>>&& job, std::vector<std::future<ReturnType>>&& results);
        operation_result(const operation_result&) = delete;

        std::vector<ReturnType> get();
    private:
        std::vector<std::future<ReturnType>> m_results;
        std::unique_ptr<job> m_job;
    };
}

template<class ReturnType, class ArgType>
mfo::operation_result<ReturnType, ArgType>::operation_result(std::unique_ptr<job<ReturnType, ArgType>>&& job, std::vector<std::future<ReturnType>>&& results) 
    : m_job{std::move(job)}, m_results{std::move(results)} {}

templat
std::vector<ReturnType> mfo::operation_result::get() {
    std::vector<ReturnType> results(m_results.size());

    std::size_t i = 0;
    for(auto&& fut: m_results)
        results[i++] = fut.get();
    
    return results;
}


#endif //MFO_OPERATION_RESULT_HEADER