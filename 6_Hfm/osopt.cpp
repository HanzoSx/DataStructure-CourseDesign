#include <osopt.hpp>

#ifdef __linux__

#include <dirent.h> // For directory handling
#include <cstring> // For string manipulation
#include <iostream>
#include <list>

bool get_files_in_path(const std::string &path, std::list<std::string> &filename)
{
    filename.clear();
    struct dirent *ent;
    if (DIR *dir = opendir(path.c_str()); dir != nullptr)
    {
        while ((ent = readdir(dir)) != nullptr)
            if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
                filename.push_back(ent->d_name);

        closedir(dir);
        return true;
    }
    else 
        return false;
}

#elif _WIN32

#include <windows.h>
#include <iostream>
#include <list>

bool get_files_in_path(const std::string &path, std::list<std::string> &filename)
{
    filename.clear();
    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile((path + "/*").c_str(), &findData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0) {
                filename.push_back(findData.cFileName);
            }
        } while (FindNextFile(hFind, &findData) != 0);
        FindClose(hFind);
        return true;
    } else {
        std::cerr << "Error finding files in directory: " << path << std::endl;
        return false;
    }
}

#endif
