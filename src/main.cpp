#include <filesystem>
#include <iostream>
#include "headers/main.h"
using namespace std;

#ifdef __APPLE__
#define OS "mac"
#endif

#ifdef __MACH__
#define OS "mac"
#endif

#ifdef __linux__
#define OS "linux"
#endif

int main(int argc, char *argv[])
{
    filesystem::path watch_path;
    if (argv[1])
    {
        watch_path = argv[1];
    }
    else
    {
        watch_path = filesystem::current_path();
    }
    if (!filesystem::exists(watch_path))
    {
        printf("Path %s does not exist.\n", watch_path.c_str());
        return 1;
    }
    if ("linux" == OS || "mac" == OS)
    {
        printf("Initialize with %s API, path %s.\n", OS, watch_path.c_str());
        listen_notification_events(watch_path);
        return 0;
    }
    cout << "Your operating system is not supported." << endl;
    return 1;
}