#ifndef FLAGIT_FILEUTILS_HH
#define FLAGIT_FILEUTILS_HH

#include <string>

/*
 * This class contains a collection of methods for working with files.
 */
namespace flagit::details {
    std::string getConfigDir();
    std::string getFileNameFromUrl(std::string const& url);
    std::string getFilePath(std::string const& url);
} // close namespace flagit::details

#endif //FLAGIT_FILEUTILS_HH
