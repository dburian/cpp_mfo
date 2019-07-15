#ifndef MFO_ONE_THREAD_OPTIMALIZER_HEADER
#define MFO_ONE_THREAD_OPTIMALIZER_HEADER

#include <unordered_map>
#include <vector>

#include "arg_types.h"
#include "operation_type.h"

namespace mfo {
    class one_thread_optimalizer {
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
std::vector<mfo::arg_set<mfo::find_arg<UnaryPredicate>>> mfo::one_thread_optimalizer::split_find_jobs(mfo::arg_set<find_arg<UnaryPredicate>>&& args, std::size_t num_of_threads) {
    std::vector<mfo::arg_set<mfo::find_arg<UnaryPredicate>>> jobsSplitted(1);

    jobsSplitted[0] = std::move(args);
    return jobsSplitted;
}

template<class UnaryPredicate> std::vector<mfo::arg_set<mfo::find_recursive_arg<UnaryPredicate>>>
mfo::one_thread_optimalizer::split_find_recursive_jobs(mfo::arg_set<find_recursive_arg<UnaryPredicate>>&& args, std::size_t num_of_threads) {
    std::vector<mfo::arg_set<mfo::find_recursive_arg<UnaryPredicate>>> jobsSplitted(1);

    jobsSplitted[0] = std::move(args);
    return jobsSplitted;
}

template<class ArgType>
std::vector<mfo::arg_set<ArgType>> mfo::one_thread_optimalizer::split_jobs(mfo::arg_set<ArgType>&& args, std::size_t num_of_threads) {
    
    if constexpr (std::is_same<ArgType, mfo::copy_arg>::value)
        return mfo::one_thread_optimalizer::split_copy_jobs(std::move(args), num_of_threads);

    else if constexpr (std::is_same<ArgType, mfo::move_arg>::value)
        return mfo::one_thread_optimalizer::split_move_jobs(std::move(args), num_of_threads);

    else if constexpr (std::is_same<ArgType, mfo::remove_arg>::value)
        return mfo::one_thread_optimalizer::split_remove_jobs(std::move(args), num_of_threads);

    else if constexpr (std::is_same<ArgType, mfo::find_arg<typename ArgType::predicate_t>>::value)
        return mfo::one_thread_optimalizer::split_find_jobs(std::move(args), num_of_threads);

    else if constexpr (std::is_same<ArgType, mfo::find_recursive_arg<typename ArgType::predicate_t>>::value)
        return mfo::one_thread_optimalizer::split_find_recursive_jobs(std::move(args), num_of_threads);
    
    throw std::invalid_argument("mfo::one_thread_optimalizer::split_jobs");
}

#endif //MFO_ONE_THREAD_OPTIMALIZER_HEADER