#include <assert.h>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <optional>
#include <print>
#include <string>
#include <unistd.h>

#include "common/util.h"
#include "parser/lexer/lexer.h"

namespace cli {

enum CliMode {
    DEFAULT,
    LEX_ONLY,
};

struct CliConfig {
    std::filesystem::path inputFile = "";
    std::string inputName = "";
    CliMode mode = CliMode::DEFAULT;
};

CliConfig parseOptions(int argc, char *argv[]) {
    auto config = CliConfig{};
    for (auto i = 0; i < argc; i++) {
        auto arg = std::string(argv[i]);
        if (util::matchStr(arg, "--lex-only")) {
            config.mode = LEX_ONLY;
        }
        config.inputFile = std::filesystem::path{argv[i]};
    }
    assert(config.inputFile.has_parent_path() &&
           std::filesystem::is_regular_file(config.inputFile));

    config.inputName = config.inputFile.stem().string();

    return config;
}

class TempFileManager {
  public:
    std::filesystem::path dir;

    std::filesystem::path file(std::string &name) { return dir / name; }

    TempFileManager(std::optional<std::filesystem::path> debugOutputRoot = std::nullopt) {
        if (debugOutputRoot) {
            dir = *debugOutputRoot;
        } else {
            dir = std::filesystem::temp_directory_path();
        }
        dir = dir / ("ccarm-ir-" + std::to_string(getpid()));
    }

    ~TempFileManager() {
        std::error_code error;
        std::filesystem::remove_all(dir, error);
        // TODO handle errors?
    }
};

int runCliApp(int argc, char *argv[]) {
    auto config = parseOptions(argc, argv);

    if (config.mode == DEFAULT) {
        TempFileManager tmpManager{config.inputFile.parent_path()};

        // invoke preprocessor
        auto preProcessedFilePath = (tmpManager.dir / (config.inputName + "_preprocessed.c"));
        auto preprocessCommand = std::format("clang -E {} -o {}", config.inputFile.c_str(),
                                             preProcessedFilePath.c_str());
        std::system(preprocessCommand.c_str());

        // TODO: invoke lexer + parser
        std::string sFile = "path/to/assembly.s";

        // invoke assembler + linker
        auto assembleCommand = std::format("clang {}", sFile);
        std::system(assembleCommand.c_str());
    } else if (config.mode == LEX_ONLY) {
        auto programText = util::readEntireFile(config.inputFile.string());
        auto tokens = lexer::tokenizeProgram(programText);

        for (auto token : tokens) {
            std::cout << token.string() << "\n";
        }
    }

    return 0;
}

} // namespace cli

int main(int argc, char *argv[]) { return cli::runCliApp(argc, argv); }