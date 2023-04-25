// Nicholas Palmer 04/2023

# pragma once
#include <fstream>
#define MAX_TCP_BUFFER_SIZE 8192

namespace fs = std::filesystem;

namespace config {
    inline fs::path base_path;
}

