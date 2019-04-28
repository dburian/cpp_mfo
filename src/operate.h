#ifndef MFO_OPERATE_HEADER
#define MFO_OPERATE_HEADER
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
    std::vector<fs::directory_entry> find_recursive(const fs::path& in_dir, UnaryPredicate p);
    template<class UnaryPredicate>
    std::vector<fs::directory_entry> find(const fs::path& in_dir, UnaryPredicate p);

    }
}


template<class UnaryPredicate>
std::vector<std::filesystem::directory_entry> mfo::operate::find_recursive(const fs::path& in_dir, UnaryPredicate p) {
    std::filesystem::recursive_directory_iterator files{in_dir};

    std::vector<std::filesystem::directory_entry> filesFound;

    // fs::end(fs::recursive_directory_iterator) returns default-constructed fs::recursive_directory_iterator which serves as an end iterator.
    std::find_if(files, std::filesystem::end(files), [p, &filesFound](const std::filesystem::directory_entry& d) {
        if(p(d)) filesFound.push_back(d);
        return false;
    }); 

    return filesFound;
}

template<class UnaryPredicate>
std::vector<std::filesystem::directory_entry> mfo::operate::find(const fs::path& in_dir, UnaryPredicate p) {
    std::filesystem::directory_iterator files{in_dir};

    std::vector<std::filesystem::directory_entry> filesFound;

    std::find_if(files, std::filesystem::end(files), [p, &filesFound](const std::filesystem::directory_entry& d) {
        if(p(d)) filesFound.push_back(d);
        return false;
    });

    return filesFound;
}


#endif // MFO_OPERATE_HEADER
