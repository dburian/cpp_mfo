#include "optimalizer.h"

std::vector<std::vector<mfo::copy_arg>> mfo::optimalizer::split_copy_jobs(std::vector<mfo::copy_arg>&& copy_args, std::size_t num_of_threads) {
    std::vector<std::vector<mfo::copy_arg>> splittedJobs(1);

    splittedJobs[0] = std::move(copy_args);
    return splittedJobs;
}


std::vector<std::vector<mfo::move_arg>> mfo::optimalizer::split_move_jobs(std::vector<mfo::move_arg>&& move_args, std::size_t num_of_threads) {
    std::vector<std::vector<mfo::move_arg>> splittedJobs(1);

    splittedJobs[0] = std::move(move_args);
    return splittedJobs;
}

std::vector<std::vector<mfo::remove_arg>> mfo::optimalizer::split_remove_jobs(std::vector<mfo::remove_arg>&& remove_args, std::size_t num_of_threads) {
    std::vector<std::vector<mfo::remove_arg>> splittedJobs(1);

    splittedJobs[0] = std::move(remove_args);
    return splittedJobs;
}