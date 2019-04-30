#ifndef MFO_VALIDATE_HEADER
#define MFO_VALIDATE_HEADER
#pragma once

#include <filesystem>
#include "arg_types.h"

namespace mfo {
    namespace validate {
        namespace fs = std::filesystem;

        bool validate_copy(const copy_arg& arg, fs::filesystem_error& er);
        bool validate_move(const move_arg& arg, fs::filesystem_error& er);
        bool validate_remove(const remove_arg& arg, fs::filesystem_error& er);

        template<class UnaryPredicate>
        bool validate_find(const find_arg<UnaryPredicate>& arg, fs::filesystem_error& er);
        template<class UnaryPredicate>
        bool validate_find_recursive(const find_recursive_arg<UnaryPredicate>& arg, fs::filesystem_error& er);
    }
}

template<class UnaryPredicate>
bool mfo::validate::validate_find(const mfo::find_arg<UnaryPredicate>& arg, std::filesystem::filesystem_error& er) {
    namespace fs = std::filesystem;
    std::string prefix = "mfo::operate::find : ";
    std::error_code err_code(-1, std::system_category());

    if(arg.in_dir.empty()) {
        er = fs::filesystem_error(prefix + "in_dir is empty", arg.in_dir, err_code);
        return false;
    }

    if(!fs::exists(arg.in_dir)) {
        er = fs::filesystem_error(prefix + arg.in_dir.string() + "does not exist", arg.in_dir, err_code);
        return false;
    }

    return true;
}

template<class UnaryPredicate>
bool mfo::validate::validate_find_recursive(const mfo::find_recursive_arg<UnaryPredicate>& arg, std::filesystem::filesystem_error& er) {
    namespace fs = std::filesystem;
    std::string prefix = "mfo::operate::find_recursive : ";
    std::error_code err_code(-1, std::system_category());

    if(arg.in_dir.empty()) {
        er = fs::filesystem_error(prefix + "in_dir is empty", arg.in_dir, err_code);
        return false;
    }

    if(!fs::exists(arg.in_dir)) {
        er = fs::filesystem_error(prefix + arg.in_dir.string() + "does not exist", arg.in_dir, err_code);
        return false;
    }

    return true;
}

#endif //MFO_VALIDATE_HEADER