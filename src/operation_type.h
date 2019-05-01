#ifndef MFO_OPERTAION_TYPE_HEADER
#define MFO_OPERTAION_TYPE_HEADER
#pragma once

namespace mfo {

    enum class operation_type : std::uint8_t {copy, move, remove, find, find_recursive};

}

#endif //MFO_OPERTAION_TYPE_HEADER