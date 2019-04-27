#ifndef MFO_JOB_HEADER
#define MFO_JOB_HEADER
#pragma once

#include <exception>
#include <thread>
#include "operation_result.h"

namespace mfo {
    class job {
    public:
        job(const job& other) = delete;
        
        virtual ~job() noexcept = 0;

        virtual void run() = 0;

        template<class ReturnType>
        operation_result<ReturnType> to_result(std::unique_ptr<job>&& this_job);

        class invalid_job_given;
    protected:
        std::thread m_thread;
    };

    class job::invalid_job_given : public std::exception {
        virtual const char* what() const noexcept override;
    };

    class copy_job : public job {

    };

    class move_job : public job {

    };

    class find_job : public job {

    };

    class remove_job : public job {

    };
}

#endif //MFO_JOB_HEADER