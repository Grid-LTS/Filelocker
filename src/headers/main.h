#include <filesystem>
using namespace std;

#define EVENT_SIZE (sizeof(struct inotify_event))
#define EVENT_READ_BUFFER (1024 * (EVENT_SIZE + 16))

void listen_notification_events(const filesystem::path &watch_path);