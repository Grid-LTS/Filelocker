#include <filesystem>
#include <iostream>
#include <sys/inotify.h>
#include <unistd.h>
#include "../headers/main.h"
using namespace std;


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