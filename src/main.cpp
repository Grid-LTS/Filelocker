
#include <filesystem>
#include <iostream>
#include <string>
#include <sys/inotify.h>
#include <unistd.h>
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

#define EVENT_SIZE (sizeof(struct inotify_event))
#define EVENT_READ_BUFFER (1024 * (EVENT_SIZE + 16))

void listen_notification_events(const filesystem::path &watch_path)
{
    int fd;
    fd = inotify_init();
    if (fd < 0)
    {
        perror("inotify_init");
    }
    int wdesc;
    wdesc = inotify_add_watch(fd, watch_path.c_str(),
                              IN_OPEN | IN_CLOSE);

    if (wdesc < 0)
    {
        perror("inotify_add_watch");
    }
    char read_buffer[EVENT_READ_BUFFER];
    int len, i = 0;
    len = read(fd, read_buffer, EVENT_READ_BUFFER);

    if (len < 0)
    {
        if (errno == EINTR)
        {
            /* need to reissue system call */
        }
        else
        {
            perror("read");
        }
    }
    else if (!len)
    {
        cout << "Cannot read buffer empty." << endl;
    }
    while (i < len)
    {
        struct inotify_event *event;

        event = (struct inotify_event *)&read_buffer[i];
        /**
         * struct inotify_event {
        __s32 wd;            
        __u32 mask;         
        __u32 cookie;        
        __u32 len;           
        char name[0];        
        };**/
        if (event->name)
        {

            printf("Detected wd=%d, mask=%u, name=%s\n",
                   event->wd, event->mask, event->name);
        }
        i += EVENT_SIZE + event->len;
    }
}

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
    if ("linux" == OS)
    {
        printf("Initialize with Linux API, path %s.\n", watch_path.c_str());
        listen_notification_events(watch_path);
        return 0;
    }
    if ("mac" == OS)
    {
        cout << "Mac OSX is not supported at the moment." << endl;
        return 0;
    }
    cout << "Your operating system is not supported." << endl;
    return 1;
}