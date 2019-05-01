#include "one_thread_optimalizer.h"

std::vector<mfo::arg_set<mfo::copy_arg>> mfo::one_thread_optimalizer::split_copy_jobs(mfo::arg_set<mfo::copy_arg>&& args, std::size_t num_of_threads) {
    std::vector<mfo::arg_set<mfo::copy_arg>> splittedJobs(1);

    splittedJobs[0] = std::move(args);
    return splittedJobs;
}


std::vector<mfo::arg_set<mfo::move_arg>> mfo::one_thread_optimalizer::split_move_jobs(mfo::arg_set<mfo::move_arg>&& args, std::size_t num_of_threads) {
    std::vector<mfo::arg_set<mfo::move_arg>> splittedJobs(1);

    splittedJobs[0] = std::move(args);
    return splittedJobs;
}

std::vector<mfo::arg_set<mfo::remove_arg>> mfo::one_thread_optimalizer::split_remove_jobs(mfo::arg_set<mfo::remove_arg>&& args, std::size_t num_of_threads) {
    std::vector<mfo::arg_set<mfo::remove_arg>> splittedJobs(1);

    splittedJobs[0] = std::move(args);
    return splittedJobs;
}