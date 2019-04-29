#ifndef MFO_OPERATION_RESULT_HEADER
#define MFO_OPERATION_RESULT_HEADER
#pragma once

#include <filesystem>
#include <future>
#include <vector>

namespace mfo {
    enum class operation_type {copy, move, remove, find, find_recursive}

    template<class ReturnType, class ArgumentType>
    class operation_result {
    public:
        using argument_t = ArgumentType;
        using return_t = ReturnType;

        operation_result(const std::shared_future<std::vector<return_t>>& res, std::size_t index, operation_type oper_t, const argument_t& arg)
            : m_res{res}, m_index{index}, m_arg{arg}, m_holds_error_state{false}, m_res_retrieved{false}, m_oper_t{oper_t} {}

        operation_result(std::filesystem::filesystem_error&& err, operation_type oper_t, const argument_t& arg)
            : m_err{std::move(err)}, m_arg{arg}, m_holds_error_state{true}, m_res_retrieved{false}, m_oper_t{oper_t} {}

        const argument_t& operation_arguments() const;
        const operation_type& operation_type() const;

        const return_t& get();

    private:
        bool m_holds_error_state;
        bool m_res_retrieved;

        std::filesystem::filesystem_error m_err;

        std::shared_future<std::vector<return_t>> m_res;
        const std::vector<return_t>& m_cached_res;
        std::size_t m_index;

        argument_t m_arg;
        operation_type m_oper_t;
        
    };
}

template<class ReturnType, class ArgumentType>
const ArgumentType& mfo::operation_result::operation_arguments() const {
    return m_arg;
}

template<class ReturnType, class ArgumentType>
const mfo::operation_type& mfo::operation_result::operation_type() const {
    return m_oper_t;
}


template<class ReturnType, class ArgumentType>
const RetrurnType& mfo::operation_result::get() {
    if(m_holds_error_state) throw m_err;

    if(m_res_retrieved) return m_cached_res[m_index];

    m_cached_res = m_res.get()
    m_res_retrieved = true;

    return m_cached_res[m_index];
}

#endif //MFO_OPERATION_RESULT_HEADER