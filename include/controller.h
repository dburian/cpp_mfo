#ifndef MFO_CONTROLLER_HEADER
#define MFO_CONTROLLER_HEADER
#pragma once

#include <filesystem>
#include <future>
#include <vector>

#include "../src/arg_types.h"
#include "../src/one_thread_optimalizer.h"
#include "../src/operate.h"
#include "../src/operation_result.h"
#include "../src/thread_pool.h"
#include "../src/validate.h"

namespace mfo{

    template <class Optimalizer = one_thread_optimalizer>
    class controller {
    public:
        using optimalizer_t = Optimalizer;

        std::vector<copy_operation_result> copy(const std::vector<copy_arg>& args, std::size_t num_of_threads);
        std::vector<move_operation_result> move(const std::vector<move_arg>& args, std::size_t num_of_threads);
        std::vector<remove_operation_result> remove(const std::vector<remove_arg>& args, std::size_t num_of_threads);

        template<class UnaryPredicate>
        std::vector<find_operation_result<UnaryPredicate>> find(const std::vector<find_arg<UnaryPredicate>>& args, std::size_t num_of_threads);
        template<class UnaryPredicate>
        std::vector<find_recursive_operation_result<UnaryPredicate>> find_recursive(const std::vector<find_recursive_arg<UnaryPredicate>>& args, std::size_t num_of_threads);

    private:
        using copy_task_t = std::packaged_task<std::vector<bool>(std::vector<copy_arg>&&)>;
        using move_task_t = std::packaged_task<std::vector<bool>(std::vector<move_arg>&&)>;
        using remove_task_t = std::packaged_task<std::vector<std::uintmax_t>(std::vector<remove_arg>&&)>;

        template<class UnaryPredicate>
        using find_task_t = std::packaged_task<std::vector<std::vector<std::filesystem::directory_entry>>(std::vector<find_arg<UnaryPredicate>>&&)>;
        template<class UnaryPredicate>
        using find_recursive_task_t = std::packaged_task<std::vector<std::vector<std::filesystem::directory_entry>>(std::vector<find_recursive_arg<UnaryPredicate>>&&)>;

        template<class ArgType, class ReturnType>
        std::vector<ReturnType> multithreaded_operation(const std::vector<ArgType>& args, std::size_t num_of_threads, operation_type oper_t);

        template<class ArgType, class TaskReturnType>
        std::packaged_task<std::vector<TaskReturnType>(std::vector<ArgType>&&)> create_task();

        static std::vector<bool> copy_task(std::vector<copy_arg>&& args);
        static std::vector<bool> move_task(std::vector<move_arg>&& args);
        static std::vector<std::uintmax_t> remove_task(std::vector<remove_arg>&& args);

        template<class UnaryPredicate>
        static std::vector<std::vector<std::filesystem::directory_entry>> find_task(std::vector<find_arg<UnaryPredicate>>&& args);
        template<class UnaryPredicate>
        static std::vector<std::vector<std::filesystem::directory_entry>> find_recursive_task(std::vector<find_recursive_arg<UnaryPredicate>>&& args);

        optimalizer_t m_optimalizer;
        thread_pool m_threads;
    };
}

template<class Optimalizer>
template<class ArgType, class ResultType>
std::vector<ResultType> mfo::controller<Optimalizer>::multithreaded_operation(const std::vector<ArgType>& args, std::size_t num_of_threads, mfo::operation_type oper_t) {
    std::vector<ResultType> results;
    mfo::arg_set<ArgType> validArgs;

    std::filesystem::filesystem_error err{"", std::error_code()};

    for(const ArgType& arg: args) {
        if(!mfo::validate::validate_arbitrary(arg, err)) 
            results.emplace_back(std::move(err), mfo::operation_type::copy, arg);
        else
            validArgs.push_back(arg);                                               // First copy of argument
    }

    auto&& optimalizedArgSets = m_optimalizer.split_jobs(std::move(validArgs), num_of_threads);

    while(!optimalizedArgSets.empty()) {
        
        mfo::arg_set<ArgType> v = std::move(optimalizedArgSets.back());       
        optimalizedArgSets.pop_back();                     

        auto task = create_task<ArgType, typename ResultType::return_t>();
        auto sharedF = task.get_future().share();

        for(std::size_t i = 0; i < v.size(); ++i)
            results.emplace_back(sharedF, i, mfo::operation_type::copy, v[i]);
        
        m_threads.start_thread(std::move(task), std::move(v));
    }

    return results;
}

template<class Optimalizer>
template<class ArgType, class TaskReturnType>
std::packaged_task<std::vector<TaskReturnType>(std::vector<ArgType>&&)> mfo::controller<Optimalizer>::create_task() {
    
    if constexpr (std::is_same<ArgType, mfo::copy_arg>::value)
        return std::packaged_task<std::vector<TaskReturnType>(std::vector<ArgType>&&)>(copy_task);

    else if constexpr (std::is_same<ArgType, mfo::move_arg>::value)
        return std::packaged_task<std::vector<TaskReturnType>(std::vector<ArgType>&&)>(move_task);

    else if constexpr (std::is_same<ArgType, mfo::remove_arg>::value)
        return std::packaged_task<std::vector<TaskReturnType>(std::vector<ArgType>&&)>(remove_task);

    else if constexpr (std::is_same<ArgType, mfo::find_arg<typename ArgType::predicate_t>>::value)
        return std::packaged_task<std::vector<TaskReturnType>(std::vector<ArgType>&&)>(find_task<typename ArgType::predicate_t>);

    else if constexpr (std::is_same<ArgType, mfo::find_recursive_arg<typename ArgType::predicate_t>>::value)
        return std::packaged_task<std::vector<TaskReturnType>(std::vector<ArgType>&&)>(find_recursive_task<typename ArgType::predicate_t>);
}

template<class Optimalizer>
std::vector<mfo::copy_operation_result> mfo::controller<Optimalizer>::copy(const std::vector<mfo::copy_arg>& args, std::size_t num_of_threads) {
    return multithreaded_operation<mfo::copy_arg, mfo::copy_operation_result>(args, num_of_threads, mfo::operation_type::copy);
}

template<class Optimalizer>
std::vector<mfo::move_operation_result> mfo::controller<Optimalizer>::move(const std::vector<mfo::move_arg>& args, std::size_t num_of_threads) {
    return multithreaded_operation<mfo::move_arg, mfo::move_operation_result>(args, num_of_threads, mfo::operation_type::move);
}

template<class Optimalizer>
std::vector<mfo::remove_operation_result> mfo::controller<Optimalizer>::remove(const std::vector<mfo::remove_arg>& args, std::size_t num_of_threads) {
    return multithreaded_operation<mfo::remove_arg, mfo::remove_operation_result>(args, num_of_threads, mfo::operation_type::remove);
}

template<class Optimalizer>
template<class UnaryPredicate>
std::vector<mfo::find_operation_result<UnaryPredicate>> mfo::controller<Optimalizer>::find(const std::vector<mfo::find_arg<UnaryPredicate>>& args, std::size_t num_of_threads) {
    return multithreaded_operation<mfo::find_arg<UnaryPredicate>, mfo::find_operation_result<UnaryPredicate>>(args, num_of_threads, mfo::operation_type::find);
}

template<class Optimalizer>
template<class UnaryPredicate>
std::vector<mfo::find_recursive_operation_result<UnaryPredicate>> mfo::controller<Optimalizer>::find_recursive(const std::vector<mfo::find_recursive_arg<UnaryPredicate>>& args, std::size_t num_of_threads) {
    return multithreaded_operation<mfo::find_recursive_arg<UnaryPredicate>, mfo::find_recursive_operation_result<UnaryPredicate>>(args, num_of_threads, mfo::operation_type::find_recursive);
}


// DEFINITIONS OF TASKS
template<class Optimalizer>
std::vector<bool> mfo::controller<Optimalizer>::copy_task(std::vector<mfo::copy_arg>&& args) {
    std::vector<bool> results(args.size());

    for(std::size_t i = 0; i < args.size(); ++i) {
        try {
            results[i] = mfo::operate::copy(args[i]);
        }catch (const std::filesystem::filesystem_error& er) {
            results[i] = false;
        }
    }

    return results;
}

template<class Optimalizer>
std::vector<bool> mfo::controller<Optimalizer>::move_task(std::vector<mfo::move_arg>&& args) {
    std::vector<bool> results(args.size());

    for(std::size_t i = 0; i < args.size(); ++i) {
        try {
            results[i] = mfo::operate::move(args[i]);
        }catch (const std::filesystem::filesystem_error& er) {
            results[i] = false;
        }
    }

    return results;
}

template<class Optimalizer>
std::vector<std::uintmax_t> mfo::controller<Optimalizer>::remove_task(std::vector<mfo::remove_arg>&& args) {
    std::vector<std::uintmax_t> results(args.size());

    for(std::size_t i = 0; i < args.size(); ++i) {
        try {
            results[i] = mfo::operate::remove(args[i]);
        }catch (const std::filesystem::filesystem_error& er) {
            results[i] = static_cast<std::uintmax_t>(-1);
        }
    }

    return results;
}

template<class Optimalizer>
template<class UnaryPredicate>
std::vector<std::vector<std::filesystem::directory_entry>> mfo::controller<Optimalizer>::find_task(std::vector<mfo::find_arg<UnaryPredicate>>&& args) {
    std::vector<std::vector<std::filesystem::directory_entry>> results(args.size());

    for(std::size_t i = 0; i < args.size(); ++i) {
        try {
            results[i] = mfo::operate::find(args[i]);
        }catch (const std::filesystem::filesystem_error& er) {
            results[i] = std::vector<std::filesystem::directory_entry>();
        }
    }

    return results;
}

template<class Optimalizer>
template<class UnaryPredicate>
std::vector<std::vector<std::filesystem::directory_entry>> mfo::controller<Optimalizer>::find_recursive_task(std::vector<mfo::find_recursive_arg<UnaryPredicate>>&& args) {
    std::vector<std::vector<std::filesystem::directory_entry>> results{args.size()};

    for(std::size_t i = 0; i < args.size(); ++i) {
        try {
            results[i] = mfo::operate::find_recursive(args[i]);
        }catch (const std::filesystem::filesystem_error& er) {
            results[i] = std::vector<std::filesystem::directory_entry>();
        }
    }

    return results;
}
#endif //MFO_CONTROLLER_HEADER