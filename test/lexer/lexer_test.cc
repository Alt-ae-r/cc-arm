
#include <cassert>
#include <filesystem>

#include "common/util.h"
#include "parser/lexer/lexer.h"
#include "test/common/util.h"

namespace fs = std::filesystem;

namespace ccarm::test {

void runTestCase(fs::path &inputFile, fs::path &outputFile) {
    auto actualTokens = lexer::tokenizeProgram(util::readEntireFile(inputFile));
    std::string actual = "";
    for (auto token : actualTokens) {
        actual += std::format("{}\n", token.string());
    }
    auto expected = util::readEntireFile(outputFile);
    assert(actual == expected);
}

int testLexer() {
    // TODO: most of this code can be reused across test types
    auto testRoot = testutil::getTestRoot() / "lexer";
    for (auto &testCase : fs::directory_iterator(testRoot)) {
        auto testName = testCase.path().filename().string();
        if (!util::matchStr(testName, "test_")) {
            continue;
        }
        const auto inputExt = ".cc";
        const auto outputExt = ".txt";
        std::optional<fs::path> inputFile = std::nullopt;
        std::optional<fs::path> outputFile = std::nullopt;
        for (auto &testFile : fs::directory_iterator(testCase.path())) {
            if (testFile.path().extension() == inputExt) {
                inputFile = testFile.path();
            } else if (testFile.path().extension() == outputExt) {
                outputFile = testFile.path();
            }
        }
        assert(inputFile && outputFile &&
               "tests need input and output files with correct extensions for this test type.");
    }

    return 0;
}
} // namespace ccarm::test

int main() { return ccarm::test::testLexer(); }