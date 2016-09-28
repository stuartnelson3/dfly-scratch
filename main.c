#include <sys/param.h>
#include <sys/mount.h>

#include<stdio.h>
#include<errno.h>

// This function prints out all data given about fs at path available from
// statfs(s)
void
print_fs(const char *path)
{
        struct statfs buf;
        int res = statfs(path, &buf);
        if (-1 == res)
        {
                int errsv = errno;
                printf("failed to stat %s: %s", path, errno);
                return;
        }


        printf("%ld", buf.f_bavail);
}

int main()
{
        print_fs("/tmp");
        return 0;
}
