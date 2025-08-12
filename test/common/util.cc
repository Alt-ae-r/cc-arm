#include <cassert>
#include <cstdlib>
#include <filesystem>

#include "test/common/util.h"

namespace ccarm::testutil {
std::filesystem::path getTestRoot() {
    std::string path = "UNKNOWN";
    if (auto *dir = std::getenv("TEST_SRCDIR")) {
        path = dir;
    }
    if (auto *dir = std::getenv("RUNFILES_DIR")) {
        path = dir;
    }
    assert(path != "UNKNOWN" && "couldn't find test root");
    
    // In Bazel 7+ with bzlmod, the main repository is always "_main" in runfiles,
    // regardless of the module name set in MODULE.bazel
    return std::filesystem::path{path} / "_main/test";
}
} // namespace ccarm::testutil