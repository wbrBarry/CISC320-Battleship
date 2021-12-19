/**
 * These tests ensure that the program's architecture does not regress, attempting to catch
 * any deleted or renamed required files
 */

#include <gtest/gtest.h>
#include <sys/stat.h>

using std::string;

TEST(Architecture, requiredFiles) {
    // Ensures required files are present
    const string files[] = {
            // Directory structure
            "build", "include", "res", "src", "tests",

            // Build files
            "CMakeLists.txt", "build/assets", "build/assets/.keep",
            "build/cmake_modules", "build/cmake_modules/FindSFML.cmake",

            // Test files
            "tests/CMakeLists.txt", "tests/googletest", "tests/main.cpp",
            "tests/assertions.h", "tests/unit", "tests/integration", "tests/system",

            // VCS files
            ".git", ".gitignore", "README.md", "LICENSE"};


    // Note: The current working directory of the tests is the build/ folder; the same as the actual
    // application. This means we don't have to change the relative paths when referring to files
    for (const string &file : files) {
        if (access(("../" + file).c_str(), F_OK) == -1) {
            throw std::invalid_argument("File/directory not found in repository root: " + file);
        }
    }
}
