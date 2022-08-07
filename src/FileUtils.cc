#include "FileUtils.hh"
#include <unistd.h>
#include <sys/stat.h>
#include <pwd.h>

namespace {
    std::string const CONFIG_DIR = flagit::details::getConfigDir();
} // close namespace<anonymous>

std::string flagit::details::getConfigDir() {
    char const* homeDir;

    if ((homeDir = getenv("HOME")) == nullptr) {
        homeDir = getpwuid(getuid())->pw_dir;
    }

    std::string result = homeDir;
    result += "/.flagit";
    int status = mkdir(result.c_str(), 0755);
    if (status == -1 && errno != EEXIST) {
        throw std::runtime_error("Could not create config directory");
    }

    return result;
}

// Return fileName corresponding to 'sourceUrl'. We need to have stable fileName that other processes can reuse,
// when url is shared. At the same time, we need to make sure we can actually create a file, so we need to remove
// unsupported characters. Its also good not to mangle the fileName so that people can debug easier.
std::string flagit::details::getFileNameFromUrl(std::string const& url) {
    std::string fileName = url;
    // Remove protocol from fileName.
    fileName.erase(0, fileName.find("://") + 3);
    std::replace(fileName.begin(), fileName.end(), '/', '_');
    std::replace(fileName.begin(), fileName.end(), '?', '_');

    return fileName;
}

std::string flagit::details::getFilePath(std::string const& url) {
    return CONFIG_DIR + "/" + getFileNameFromUrl(url) + ".json";
}