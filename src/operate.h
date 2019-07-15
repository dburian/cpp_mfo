#ifndef MFO_OPERATE_HEADER
#define MFO_OPERATE_HEADER

#include <algorithm>
#include <filesystem>
#include <vector>

#include "arg_types.h"

namespace mfo {
    namespace operate {

    namespace fs = std::filesystem;

    bool copy(const copy_arg& arg);
    bool move(const move_arg& arg);
    std::uintmax_t remove(const remove_arg& arg);

    template<class UnaryPredicate>
    std::vector<fs::directory_entry> find_recursive(const find_recursive_arg<UnaryPredicate>& arg);
    template<class UnaryPredicate>
    std::vector<fs::directory_entry> find(const find_arg<UnaryPredicate>& arg);

    }
}


template<class UnaryPredicate>
std::vector<std::filesystem::directory_entry> mfo::operate::find_recursive(const mfo::find_recursive_arg<UnaryPredicate>& arg) {
    std::filesystem::recursive_directory_iterator files{arg.in_dir};

    std::vector<std::filesystem::directory_entry> filesFound;

    // fs::end(fs::recursive_directory_iterator) returns default-constructed fs::recursive_directory_iterator which serves as an end iterator.
    std::find_if(files, std::filesystem::end(files), [&arg, &filesFound](const std::filesystem::directory_entry& d) {
        if((*(arg.predicate))(d)) filesFound.push_back(d);
        return false;
    }); 

    return filesFound;
}

template<class UnaryPredicate>
std::vector<std::filesystem::directory_entry> mfo::operate::find(const find_arg<UnaryPredicate>& arg) {
    std::filesystem::directory_iterator files{arg.in_dir};

    std::vector<std::filesystem::directory_entry> filesFound;

    std::find_if(files, std::filesystem::end(files), [&arg, &filesFound](const std::filesystem::directory_entry& d) {
        if((*(arg.predicate))(d)) filesFound.push_back(d);
        return false;
    });

    return filesFound;
}


#endif // MFO_OPERATE_HEADER
