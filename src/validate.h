#ifndef MFO_VALIDATE_HEADER
#define MFO_VALIDATE_HEADER

#include <filesystem>

#include "arg_types.h"
#include "operation_type.h"

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

        template<class ArgType>
        bool validate_arbitrary(const ArgType& arg, fs::filesystem_error& er);
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

template<class ArgType>
bool mfo::validate::validate_arbitrary(const ArgType& arg, std::filesystem::filesystem_error& er) {
    if constexpr (std::is_same<ArgType, mfo::copy_arg>::value)
        return mfo::validate::validate_copy(arg, er);

    else if constexpr (std::is_same<ArgType, mfo::move_arg>::value)
        return mfo::validate::validate_move(arg, er);

    else if constexpr (std::is_same<ArgType, mfo::remove_arg>::value)
        return mfo::validate::validate_remove(arg, er);

    else if constexpr (std::is_same<ArgType, mfo::find_arg<typename ArgType::predicate_t>>::value)
        return mfo::validate::validate_find(arg, er);

    else if constexpr (std::is_same<ArgType, mfo::find_recursive_arg<typename ArgType::predicate_t>>::value)
        return mfo::validate::validate_find_recursive(arg, er);
}
#endif //MFO_VALIDATE_HEADER