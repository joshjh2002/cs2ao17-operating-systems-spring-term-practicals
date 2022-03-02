#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct my_msgbuf
{
    long mtype;
    char mtext[200];
};

int main(void)
{
    struct my_msgbuf buf;
    int msqid;
    key_t key;

    int msqid2;
    key_t key2;

    if ((key = ftok("kirk2.c", 'A')) == -1)
    {
        perror("ftok");
        exit(1);
    }

    if ((msqid = msgget(key, 0644 | IPC_CREAT)) == -1)
    {
        perror("msgget");
        exit(1);
    }

    if ((key2 = ftok("kirk2.c", 'B')) == -1)
    {
        perror("ftok");
        exit(1);
    }

    if ((msqid2 = msgget(key2, 0644 | IPC_CREAT)) == -1)
    {
        perror("msgget");
        exit(1);
    }

    printf("Enter lines of text, ^D to quit:\n");

    buf.mtype = 1; /* we don't really care in this case */

    while (fgets(buf.mtext, sizeof buf.mtext, stdin) != NULL)
    {
        int len = strlen(buf.mtext);

        /* ditch newline at end, if it exists */
        if (buf.mtext[len - 1] == '\n')
            buf.mtext[len - 1] = '\0';

        int isUpper = 1;

        for (int i = 0; i < len - 1; i++)
        {
            if (buf.mtext[i] > 97)
            {
                isUpper = 0;
            }
        }

        if (isUpper)
        {
            if (msgsnd(msqid2, &buf, len + 1, 0) == -1) /* +1 for '\0' */
                perror("msgsnd");
        }
        else
        {
            if (msgsnd(msqid, &buf, len + 1, 0) == -1) /* +1 for '\0' */
                perror("msgsnd");
        }
    }

    if (msgctl(msqid, IPC_RMID, NULL) == -1)
    {
        perror("msgctl");
        exit(1);
    }

    return 0;
}