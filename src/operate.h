#ifndef OPERATE_HEADER
#define OPERATE_HEADER
#pragma once

#include <filesystem>
#include <vector>

namespace mfo {
    namespace operate {

    namespace fs = std::filesystem;

    bool copy(const fs::path& from, const fs::path& to);
    bool move(const fs::path& from, const fs::path& to);
    std::uintmax_t remove(const fs::path& target);

    template<class UnaryPredicate>
    std::vector<fs::directory_entry> find_if(const fs::path& in_dir, UnaryPredicate p);
    template<class UnaryPredicate>
    std::vector<fs::directory_entry> find_if_local(const fs::path& in_dir, UnaryPredicate p);

    }
}

template<class UnaryPredicate>
std::vector<std::filesystem::directory_entry> mfo::operate::find_if(const fs::path& in_dir, UnaryPredicate p) {
    std::filesystem::recursive_directory_iterator files{in_dir};

    std::vector<std::filesystem::directory_entry> files_found;

    // fs::end(fs::recursive_directory_iterator) returns default-constructed fs::recursive_directory_iterator which serves as an end iterator.
    std::find_if(files, std::filesystem::end(files), [p, &files_found](const std::filesystem::directory_entry& d) {
        if(p(d)) files_found.push_back(d);
        return false;
    }); 

    return files_found;
}

template<class UnaryPredicate>
std::vector<std::filesystem::directory_entry> mfo::operate::find_if_local(const fs::path& in_dir, UnaryPredicate p) {
    std::filesystem::directory_iterator files{in_dir};

    std::vector<std::filesystem::directory_entry> files_found;

    std::find_if(files, std::filesystem::end(files), [p, &files_found](const std::filesystem::directory_entry& d) {
        if(p(d)) files_found.push_back(d);
        return false;
    });

    return files_found;
}


#endif // OPERATE_HEADER
