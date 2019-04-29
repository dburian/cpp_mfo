#include "validate.h"

bool is_ancestor(const std::filesystem::path& ancestor, const std::filesystem::path& child) {
    // Expects paths to two directories in canonical format

    if (std::distance(ancestor.begin(), ancestor.end()) > std::distance(child.begin(), child.end())) return false;

    return std::equal(ancestor.begin(), ancestor.end(), child.begin());
}

bool mfo::validate::validate_copy(const mfo::copy_arg& arg, std::filesystem::filesystem_error& er) {
    namespace fs = std::filesystem;
    std::string prefix = "mfo::operate::copy : ";

    if(arg.to.empty()) {
        er = fs::filesystem_error(prefix + "'to' is empty", arg.from, arg.to, std::error_code());
        return false;      
    }

    if(arg.from.empty()) {
        er = fs::filesystem_error(prefix + "'from' is empty", arg.from, arg.to, std::error_code());
        return false;      
    }

    if(!fs::exists(arg.from)) {
        er = fs::filesystem_error(prefix + arg.from.string() + " does not exist", arg.from, arg.to, std::error_code());
        return false;
    }

    if(fs::equivalent(arg.from, arg.to)) {
        er = fs::filesystem_error(prefix + arg.from.string() + " and " + arg.to.string() + " are equivalent", arg.from, arg.to, std::error_code());
        return false;
    }

    if(fs::is_other(arg.from)) {
        er = fs::filesystem_error(prefix + arg.from.string() + " is not regular file, directory or symlink", arg.from, arg.to, std::error_code());
        return false;
    }

    if(fs::is_other(arg.to)) {
        er = fs::filesystem_error(prefix + arg.to.string() + " is not regular file, directory or symlink", arg.from, arg.to, std::error_code());
        return false;
    }

    return true;
}

bool mfo::validate::validate_move(const mfo::move_arg& arg, std::filesystem::filesystem_error& er) {
    namespace fs = std::filesystem;
    std::string prefix = "mfo::operate::move : ";

    if(arg.to.empty()) {
        er = fs::filesystem_error(prefix + "'to' is empty", arg.from, arg.to, std::error_code());
        return false;      
    }

    if(arg.from.empty()) {
        er = fs::filesystem_error(prefix + "'from' is empty", arg.from, arg.to, std::error_code());
        return false;      
    }

    if(arg.to.filename() == "." || arg.to.filename() == "..") {
        er = fs::filesystem_error(prefix + arg.to.string() + " ends with '.' or '..'", arg.from, arg.to, std::error_code());
        return false;
    }

    if(!fs::exists(arg.to.parent_path())) {
        if(arg.to.filename() == "") {
            //to ends on `directory separator`

            er = fs::filesystem_error(prefix + arg.to.string() + " ends with directory seperator and names a non-existent directory", arg.from, arg.to, std::error_code());
            return false;
        }

        er = fs::filesystem_error(prefix + arg.to.string() + " is in a non-existent directory", arg.from, arg.to, std::error_code());
        return false;
    }

    if(arg.from.filename() == "") {
        //from ends with `directory separator`
        er = fs::filesystem_error(prefix + arg.from.string() + " ends with directory seperator", arg.from, arg.to, std::error_code());
        return false;
    }

    if(is_ancestor(fs::canonical(arg.from), fs::canonical(arg.to.parent_path()))) {
        er = fs::filesystem_error(prefix + arg.from.string() + "  is a directory which is ancestor of " + arg.to.string(), arg.from, arg.to, std::error_code());
        return false;
    }

    if(!fs::exists(arg.from)) {
        er = fs::filesystem_error(prefix + arg.from.string() + " does not exist", arg.from, arg.to, std::error_code());
        return false;
    }

    if(fs::is_directory(arg.from) && fs::exists(arg.to) && !fs::is_directory(arg.to)) {
        er = fs::filesystem_error(prefix + arg.from.string() + " is a directory, but " + arg.to.string() + " is not", arg.from, arg.to, std::error_code());
        return false;
    }

    if(!fs::is_directory(arg.from) && fs::is_directory(arg.to)){
        er = fs::filesystem_error(prefix + arg.to.string() + " is a directory, but " + arg.from.string() + " is not", arg.from, arg.to, std::error_code());
        return false;
    }

    return true;
}

bool mfo::validate::validate_remove(const mfo::remove_arg& arg, std::filesystem::filesystem_error& er) {
    namespace fs = std::filesystem;
    std::string prefix = "mfo::operate::remove : ";

    if(arg.target.empty()) {
        er = fs::filesystem_error(prefix + "'target' is empty", arg.target, std::error_code());
        return false;
    }

    if(!fs::exists(arg.target)) {
        er = fs::filesystem_error(prefix + arg.target.string() + " does not exist", arg.target, std::error_code());
        return false;
    }

    return true;
}