#include <algorithm>

#include "operate.h"

namespace mfo {
    namespace operate {
        
        bool copy(const fs::path& from, const fs::path& to) {
            fs::copy(from, to);
            return true;
        }

        bool move(const fs::path& from, const fs::path& to) {
            fs::rename(from, to);
            return true;
        }

        std::uintmax_t remove(const fs::path& target) {
            return fs::remove_all(target);
        }

    }
}