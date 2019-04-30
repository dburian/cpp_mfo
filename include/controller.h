#ifndef MFO_CONTROLLER_HEADER
#define MFO_CONTROLLER_HEADER
#pragma once

#include <filesystem>
#include <future>
#include <vector>

#include "../src/arg_types.h"
#include "../src/optimalizer.h"
#include "../src/operate.h"
#include "../src/operation_result.h"
#include "../src/thread_pool.h"
#include "../src/validate.h"

namespace mfo{

    template <class Optimalizer = optimalizer>
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
std::vector<mfo::copy_operation_result> mfo::controller<Optimalizer>::copy(const std::vector<mfo::copy_arg>& args, std::size_t num_of_threads) {
    std::vector<mfo::copy_operation_result> results;
    std::vector<mfo::copy_arg> validArgs;

    std::filesystem::filesystem_error err{"", std::error_code()};

    for(const mfo::copy_arg& arg: args) {
        if(!mfo::validate::validate_copy(arg, err)) 
            results.emplace_back(std::move(err), mfo::operation_type::copy, arg);
        else
            validArgs.push_back(arg);
    }

    std::vector<std::vector<mfo::copy_arg>> optimalizedArgs = m_optimalizer.split_copy_jobs(std::move(validArgs), num_of_threads);

    while(!optimalizedArgs.empty()) {
        
        std::vector<mfo::copy_arg> v;                   //
        std::swap(v, optimalizedArgs.back());           // hopefully cheap move
        optimalizedArgs.pop_back();                     //

        copy_task_t t(copy_task);
        auto sharedF = t.get_future().share();

        for(std::size_t i = 0; i < v.size(); ++i)
            results.emplace_back(sharedF, i, mfo::operation_type::copy, v[i]);
        
        m_threads.start_thread(std::move(t), std::move(v));
    }

    return results;
}

template<class Optimalizer>
std::vector<mfo::move_operation_result> mfo::controller<Optimalizer>::move(const std::vector<mfo::move_arg>& args, std::size_t num_of_threads) {
    std::vector<mfo::move_operation_result> results;
    std::vector<mfo::move_arg> validArgs;

    std::filesystem::filesystem_error err{"", std::error_code()};

    for(const mfo::move_arg& arg: args) {
        if(!mfo::validate::validate_move(arg, err)) 
            results.emplace_back(std::move(err), mfo::operation_type::move, arg);
        else
            validArgs.push_back(arg);
    }

    std::vector<std::vector<mfo::move_arg>> optimalizedArgs = m_optimalizer.split_move_jobs(std::move(validArgs), num_of_threads);

    while(!optimalizedArgs.empty()) {

        std::vector<mfo::move_arg> v;               //
        std::swap(v, optimalizedArgs.back());       // hopefully cheap move
        optimalizedArgs.pop_back();                 //

        move_task_t t{move_task};
        auto sharedF = t.get_future().share();

        for(std::size_t i = 0; i < v.size(); ++i)
            results.emplace_back(sharedF, i, mfo::operation_type::move, v[i]);
        
        m_threads.start_thread(std::move(t), std::move(v));
    }

    return results;
}


template<class Optimalizer>
std::vector<mfo::remove_operation_result> mfo::controller<Optimalizer>::remove(const std::vector<mfo::remove_arg>& args, std::size_t num_of_threads) {
    std::vector<mfo::remove_operation_result> results;
    std::vector<mfo::remove_arg> validArgs;

    std::filesystem::filesystem_error err{"", std::error_code()};

    for(const mfo::remove_arg& arg: args) {
        if(!mfo::validate::validate_remove(arg, err)) 
            results.emplace_back(std::move(err), mfo::operation_type::remove, arg);
        else
            validArgs.push_back(arg);
    }

    std::vector<std::vector<mfo::remove_arg>> optimalizedArgs = m_optimalizer.split_remove_jobs(std::move(validArgs), num_of_threads);

    while(!optimalizedArgs.empty()) {

        std::vector<mfo::remove_arg> v;             //
        std::swap(v, optimalizedArgs.back());       // hopefully cheap move
        optimalizedArgs.pop_back();                 //

        remove_task_t t{remove_task};
        auto sharedF = t.get_future().share();

        for(std::size_t i = 0; i < v.size(); ++i)
            results.emplace_back(sharedF, i, mfo::operation_type::remove, v[i]);
        
        m_threads.start_thread(std::move(t), std::move(v));
    }

    return results;
}


template<class Optimalizer>
template<class UnaryPredicate>
std::vector<mfo::find_operation_result<UnaryPredicate>> mfo::controller<Optimalizer>::find(const std::vector<mfo::find_arg<UnaryPredicate>>& args, std::size_t num_of_threads) {
    std::vector<mfo::find_operation_result<UnaryPredicate>> results;
    std::vector<mfo::find_arg<UnaryPredicate>> validArgs;

    std::filesystem::filesystem_error err{"", std::error_code()};

    for(const mfo::find_arg<UnaryPredicate>& arg: args) {
        if(!mfo::validate::validate_find(arg, err)) 
            results.emplace_back(std::move(err), mfo::operation_type::find, arg);
        else
            validArgs.push_back(arg);
    }

    std::vector<std::vector<mfo::find_arg<UnaryPredicate>>> optimalizedArgs = m_optimalizer.split_find_jobs(std::move(validArgs), num_of_threads);

    while(!optimalizedArgs.empty()) {

        std::vector<mfo::find_arg<UnaryPredicate>> v;               //
        std::swap(v, optimalizedArgs.back());                       // hopefully cheap move
        optimalizedArgs.pop_back();                                 //

        find_task_t<UnaryPredicate> t{find_task<UnaryPredicate>};
        auto sharedF = t.get_future().share();

        for(std::size_t i = 0; i < v.size(); ++i)
            results.emplace_back(sharedF, i, mfo::operation_type::find, v[i]);
        
        m_threads.start_thread(std::move(t), std::move(v));
    }

    return results;
}


template<class Optimalizer>
template<class UnaryPredicate>
std::vector<mfo::find_recursive_operation_result<UnaryPredicate>> mfo::controller<Optimalizer>::find_recursive(const std::vector<mfo::find_recursive_arg<UnaryPredicate>>& args, std::size_t num_of_threads) {
    std::vector<mfo::find_recursive_operation_result<UnaryPredicate>> results;
    std::vector<mfo::find_recursive_arg<UnaryPredicate>> validArgs;

    std::filesystem::filesystem_error err{"", std::error_code()};

    for(const mfo::find_recursive_arg<UnaryPredicate>& arg: args) {
        if(!mfo::validate::validate_find_recursive(arg, err)) 
            results.emplace_back(std::move(err), mfo::operation_type::find_recursive, arg);
        else
            validArgs.push_back(arg);
    }

    std::vector<std::vector<mfo::find_recursive_arg<UnaryPredicate>>> optimalizedArgs = m_optimalizer.split_find_recursive_jobs(std::move(validArgs), num_of_threads);

    while(!optimalizedArgs.empty()) {

        std::vector<mfo::find_recursive_arg<UnaryPredicate>> v;     //
        std::swap(v, optimalizedArgs.back());                       // hopefully cheap move
        optimalizedArgs.pop_back();                                 //

        find_recursive_task_t<UnaryPredicate> t{find_recursive_task<UnaryPredicate>};
        auto sharedF = t.get_future().share();

        for(std::size_t i = 0; i < v.size(); ++i)
            results.emplace_back(sharedF, i, mfo::operation_type::find_recursive, v[i]);
        
        m_threads.start_thread(std::move(t), std::move(v));
    }

    return results;
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