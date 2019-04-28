#ifndef MFO_ARG_TYPES_HEADER
#define MFO_ARG_TYPES_HEADER
#pragma once

#include <filesystem>

namespace mfo {
    namespace fs = std::filesystem;

    struct copy_arg {
        copy_arg(const fs::path& init_from, const fs::path& init_to) : from{init_from}, to{init_to} {} 
        copy_arg(fs::path&& init_from, fs::path&& init_to) : from{std::move(init_from)}, to{std::move(init_to)} {}

        fs::path from;
        fs::path to;
    };

    using move_arg = copy_arg;

    struct remove_arg {
        remove_arg(const fs::path& init_target) : target{init_target} {}
        remove_arg(fs::path&& init_target) : target{std::move(init_target)} {}

        fs::path target;
    };

    template<class UnaryPredicate>
    struct find_arg {
        find_arg(const fs::path& init_in, UnaryPredicate&& init_p) : in{init_in}, p{std::move(init_p)} {}
        find_arg(fs::path&& init_in, UnaryPredicate&& init_p) : in{std::move(init_in)}, p{std::move(init_p)} {}

        fs::path in;
        UnaryPredicate p;
    };

    template<class UnaryPredicate>
    using find_recursive_arg = find_arg<UnaryPredicate>;
}


#endif  //MFO_ARG_TYPES_HEADER