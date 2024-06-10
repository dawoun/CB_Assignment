#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    // Retrieve the location of the pipe
    if (argc != 2)
    {
        std::cout << "Please provide pipe name to listen for results on" << std::endl;
        return -1;
    }

    int err = mkfifo(argv[1], 0666);
    if (err)
    {
        std::cout << "Failed to create pipe at " << argv[1] << std::endl;
        return -2;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1)
    {
        std::cout << "Failed to open pipe at " << argv[1] << std::endl;
        return -2;
    }

    char buffer[1024];
    while (true)
    {
        int bytes = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes == -1)
            break;

        if (!bytes)
            continue;

        buffer[bytes] = '\0';
        std::cout << "pipe print " << buffer;
    }

    return 0;
}
