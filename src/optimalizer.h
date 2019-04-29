#ifndef MFO_OPTIMALIZER_HEADER
#define MFO_OPTIMALIZER_HEADER
#pragma once

#include <vector>
#include "arg_types.h"

namespace mfo {
    class optimalizer {
    public:
        optimalizer();

        std::vector<std::vector<copy_arg>> split_copy_jobs(std::vector<copy_arg>&& copy_args, std::size_t num_of_threads);
        std::vector<std::vector<move_arg>> split_move_jobs(std::vector<move_arg>&& move_args, std::size_t num_of_threads);
        std::vector<std::vector<remove_arg>> split_remove_jobs(std::vector<remove_arg>&& remove_args, std::size_t num_of_threads);
        
        template<class UnaryPredicate>
        std::vector<std::vector<find_arg<UnaryPredicate>>> split_find_jobs(std::vector<find_arg<UnaryPredicate>>&& find_args, std::size_t num_of_threads);
        template<class UnaryPredicate>
        std::vector<std::vector<find_recursive_arg<UnaryPredicate>>> split_find_recursive_jobs(std::vector<find_recursive_arg<UnaryPredicate>>&& find_recursive_args, std::size_t num_of_threads);
    };

}



template<class UnaryPredicate>
std::vector<std::vector<mfo::find_arg<UnaryPredicate>>> mfo::optimalizer::split_find_jobs(std::vector<mfo::find_arg<UnaryPredicate>>&& find_args, std::size_t num_of_threads) {
    std::vector<std::vector<mfo::find_arg<UnaryPredicate>>> jobsSplitted(1);

    jobsSplitted[0] = std::move(find_args);
    return jobsSplitted;
}

template<class UnaryPredicate> std::vector<std::vector<mfo::find_recursive_arg<UnaryPredicate>>> 
mfo::optimalizer::split_find_recursive_jobs(std::vector<mfo::find_recursive_arg<UnaryPredicate>>&& find_recursive_args, std::size_t num_of_threads) {
    std::vector<std::vector<mfo::find_recursive_arg<UnaryPredicate>>> jobsSplitted(1);

    jobsSplitted[0] = std::move(find_recursive_args);
    return jobsSplitted;
}

#endif //MFO_OPTIMALIZER_HEADER