#include <sys/stat.h>
#include <stdbool.h>

bool file_exists(char *filename)
{
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}
