#include "job.h"

mfo::operation_result mfo::job::to_result(std::unique_ptr<mfo::job>&& this_job) {
    if(this_job.get() != this) throw mfo::job::invalid_job_given{};

    return mfo::operation_result(std::move(this_job), std::move(m_f));
}



const char* mfo::job::invalid_job_given::what() const noexcept {
    return "mfo::job::to_result got unique_ptr that does hold different instance of mfo::job";
}