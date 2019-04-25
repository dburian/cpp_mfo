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

        template<class UnaryPredicate>
        fs::recursive_directory_iterator find_if(const fs::path& in_dir, UnaryPredicate p) {
            fs::recursive_directory_iterator files{in_dir};

            return std::find_if(files, fs::end(files), p);  // fs::end(fs::recursive_directory_iterator) returns default-constructed fs::recursive_directory_iterator which serves as an end iterator.
        }

    }
}