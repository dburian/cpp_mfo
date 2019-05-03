#ifndef MFO_EVEN_OPTIMALIZER_HEADER
#define MFO_EVEN_OPTIMALIZER_HEADER
#pragma once

#include <vector>

#include "arg_types.h"

namespace mfo {
    class even_optimalizer {
    public:
        std::vector<arg_set<copy_arg>> split_copy_jobs(arg_set<copy_arg>&& args, std::size_t num_of_threads);
        std::vector<arg_set<move_arg>> split_move_jobs(arg_set<move_arg>&& args, std::size_t num_of_threads);
        std::vector<arg_set<remove_arg>> split_remove_jobs(arg_set<remove_arg>&& args, std::size_t num_of_threads);
        
        template<class UnaryPredicate>
        std::vector<arg_set<find_arg<UnaryPredicate>>> split_find_jobs(arg_set<find_arg<UnaryPredicate>>&& args, std::size_t num_of_threads);
        template<class UnaryPredicate>
        std::vector<arg_set<find_recursive_arg<UnaryPredicate>>> split_find_recursive_jobs(arg_set<find_recursive_arg<UnaryPredicate>>&& recursive_args, std::size_t num_of_threads);

        template<class ArgType>
        std::vector<arg_set<ArgType>> split_jobs(arg_set<ArgType>&& args, std::size_t num_of_threads);
    };
}

template<class UnaryPredicate>
std::vector<mfo::arg_set<mfo::find_arg<UnaryPredicate>>> mfo::even_optimalizer::split_find_jobs(mfo::arg_set<find_arg<UnaryPredicate>>&& args, std::size_t num_of_threads) {
    std::vector<mfo::arg_set<mfo::find_arg<UnaryPredicate>>> splittedJobs(std::min(num_of_threads, args.size()));

    std::size_t numOfJobsPerThread = std::max(args.size() / num_of_threads, 1UL);

    std::size_t threadsConstructed = 0;
    auto&& it = args.begin();
    while(it + numOfJobsPerThread < args.end()) {
        splittedJobs[threadsConstructed] = mfo::arg_set<mfo::find_arg<UnaryPredicate>>(numOfJobsPerThread);
        std::move(it, it + numOfJobsPerThread, splittedJobs[threadsConstructed].begin());
        it += numOfJobsPerThread;
        ++threadsConstructed;
    }

    splittedJobs[threadsConstructed] = mfo::arg_set<mfo::find_arg<UnaryPredicate>>(numOfJobsPerThread);
    std::move(it, it + numOfJobsPerThread, splittedJobs[threadsConstructed].begin());

    return splittedJobs;
}

template<class UnaryPredicate>
std::vector<mfo::arg_set<mfo::find_recursive_arg<UnaryPredicate>>> mfo::even_optimalizer::split_find_recursive_jobs(mfo::arg_set<find_recursive_arg<UnaryPredicate>>&& args, std::size_t num_of_threads) {
    std::vector<mfo::arg_set<mfo::find_recursive_arg<UnaryPredicate>>> splittedJobs(std::min(num_of_threads, args.size()));

    std::size_t numOfJobsPerThread = std::max(args.size() / num_of_threads, 1UL);

    std::size_t threadsConstructed = 0;
    auto&& it = args.begin();
    while(it + numOfJobsPerThread < args.end()) {
        splittedJobs[threadsConstructed] = mfo::arg_set<mfo::find_recursive_arg<UnaryPredicate>>(numOfJobsPerThread);
        std::move(it, it + numOfJobsPerThread, splittedJobs[threadsConstructed].begin());
        it += numOfJobsPerThread;
        ++threadsConstructed;
    }

    splittedJobs[threadsConstructed] = mfo::arg_set<mfo::find_recursive_arg<UnaryPredicate>>(numOfJobsPerThread);
    std::move(it, it + numOfJobsPerThread, splittedJobs[threadsConstructed].begin());

    return splittedJobs;
}

template<class ArgType>
std::vector<mfo::arg_set<ArgType>> mfo::even_optimalizer::split_jobs(mfo::arg_set<ArgType>&& args, std::size_t num_of_threads) {
    
    if constexpr (std::is_same<ArgType, mfo::copy_arg>::value)
        return mfo::even_optimalizer::split_copy_jobs(std::move(args), num_of_threads);

    else if constexpr (std::is_same<ArgType, mfo::move_arg>::value)
        return mfo::even_optimalizer::split_move_jobs(std::move(args), num_of_threads);

    else if constexpr (std::is_same<ArgType, mfo::remove_arg>::value)
        return mfo::even_optimalizer::split_remove_jobs(std::move(args), num_of_threads);

    else if constexpr (std::is_same<ArgType, mfo::find_arg<typename ArgType::predicate_t>>::value)
        return mfo::even_optimalizer::split_find_jobs(std::move(args), num_of_threads);

    else if constexpr (std::is_same<ArgType, mfo::find_recursive_arg<typename ArgType::predicate_t>>::value)
        return mfo::even_optimalizer::split_find_recursive_jobs(std::move(args), num_of_threads);
    
    throw std::invalid_argument("mfo::even_optimalizer::split_jobs");
}


#endif //MFO_EVEN_OPTIMALIZER_HEADER