#include "fio.hpp"

#include <fstream>
#include <sstream>

#include <rf/core/error.hpp>

namespace rf {

std::string FIO::ReadFile(const std::string & filename) {
    std::ifstream file(filename);
    if (!file)
        throw RF_LOCATED_ERROR("Couldn't open file \"{}\" to read", filename);

    std::stringstream stream;
    stream << file.rdbuf();
    return stream.str();
}

void FIO::WriteFile(const std::string& filename, const std::string& contents) {
    std::ofstream file(filename, std::ios::trunc);
    if (!file)
        throw RF_LOCATED_ERROR("Couldn't open file \"{}\" to write", filename);
    file << contents;
}

} // namespace rf
