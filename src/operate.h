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
    std::vector<fs::path> find_if(const fs::path& in_dir, UnaryPredicate p);

    }
}

#endif // OPERATE_HEADER
