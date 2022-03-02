#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    int pfds[2];

    pipe(pfds);

    if (!fork())
    {
        close(1);       /* close normal stdout */
        dup(pfds[1]);   /* make stdout same as pfds[1] */
        close(pfds[0]); /* we don't need this */
        execlp("cat", "cat", "/etc/passwd", NULL);
    }
    else
    {
        int pfds2[2];

        pipe(pfds2);

        if (!fork())
        {
            close(1);        /* close normal stdout */
            dup(pfds2[1]);   /* make stdout same as pfds[1] */
            close(pfds2[0]); /* we don't need this */
            execlp("cut", "cut","-f1", "-d:" ,NULL);
        }
        else
        {
            close(0);        /* close normal stdin */
            dup(pfds2[0]);   /* make stdin same as pfds[0] */
            close(pfds2[1]); /* we don't need this */
            execlp("sort", "sort", NULL);
        }
    }

    return 0;
}