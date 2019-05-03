#include <algorithm>

#include "even_optimalizer.h"



std::vector<mfo::arg_set<mfo::copy_arg>> mfo::even_optimalizer::split_copy_jobs(mfo::arg_set<mfo::copy_arg>&& args, std::size_t num_of_threads){
    std::vector<mfo::arg_set<mfo::copy_arg>> splittedJobs(std::min(num_of_threads, args.size()));

    std::size_t numOfJobsPerThread = std::max(args.size() / num_of_threads, 1UL);

    std::size_t threadsConstructed = 0;
    auto&& it = args.begin();
    while(it + numOfJobsPerThread < args.end()) {
        splittedJobs[threadsConstructed] = mfo::arg_set<mfo::copy_arg>(numOfJobsPerThread);
        std::move(it, it + numOfJobsPerThread, splittedJobs[threadsConstructed].begin());
        it += numOfJobsPerThread;
        ++threadsConstructed;
    }

    splittedJobs[threadsConstructed] = mfo::arg_set<mfo::copy_arg>(numOfJobsPerThread);
    std::move(it, it + numOfJobsPerThread, splittedJobs[threadsConstructed].begin());

    return splittedJobs;
}

std::vector<mfo::arg_set<mfo::move_arg>> mfo::even_optimalizer::split_move_jobs(mfo::arg_set<mfo::move_arg>&& args, std::size_t num_of_threads) {
    std::vector<mfo::arg_set<mfo::move_arg>> splittedJobs(std::min(num_of_threads, args.size()));

    std::size_t numOfJobsPerThread = std::max(args.size() / num_of_threads, 1UL);

    std::size_t threadsConstructed = 0;
    auto&& it = args.begin();
    while(it + numOfJobsPerThread < args.end()) {
        splittedJobs[threadsConstructed] = mfo::arg_set<mfo::move_arg>(numOfJobsPerThread);
        std::move(it, it + numOfJobsPerThread, splittedJobs[threadsConstructed].begin());
        it += numOfJobsPerThread;
        ++threadsConstructed;
    }

    splittedJobs[threadsConstructed] = mfo::arg_set<mfo::move_arg>(numOfJobsPerThread);
    std::move(it, it + numOfJobsPerThread, splittedJobs[threadsConstructed].begin());

    return splittedJobs;
}
 
std::vector<mfo::arg_set<mfo::remove_arg>> mfo::even_optimalizer::split_remove_jobs(mfo::arg_set<mfo::remove_arg>&& args, std::size_t num_of_threads){
    std::vector<mfo::arg_set<mfo::remove_arg>> splittedJobs(std::min(num_of_threads, args.size()));

    std::size_t numOfJobsPerThread = std::max(args.size() / num_of_threads, 1UL);

    std::size_t threadsConstructed = 0;
    auto&& it = args.begin();
    while(it + numOfJobsPerThread < args.end()) {
        splittedJobs[threadsConstructed] = mfo::arg_set<mfo::remove_arg>(numOfJobsPerThread);
        std::move(it, it + numOfJobsPerThread, splittedJobs[threadsConstructed].begin());
        it += numOfJobsPerThread;
        ++threadsConstructed;
    }

    splittedJobs[threadsConstructed] = mfo::arg_set<mfo::remove_arg>(numOfJobsPerThread);
    std::move(it, it + numOfJobsPerThread, splittedJobs[threadsConstructed].begin());

    return splittedJobs;
}