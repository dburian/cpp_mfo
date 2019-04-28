#include <algorithm>

#include "operate.h"

namespace mfo {
    namespace operate {
        
        bool copy(const mfo::copy_arg& arg) {
            fs::copy(arg.from, arg.to);
            return true;
        }

        bool move(const mfo::move_arg& arg) {
            fs::rename(arg.from, arg.to);
            return true;
        }

        std::uintmax_t remove(const mfo::remove_arg arg) {
            return fs::remove_all(arg.target);
        }

    }
}