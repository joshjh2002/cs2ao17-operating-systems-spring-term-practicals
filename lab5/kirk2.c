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
    int msqid_spock;
    key_t key_spock;

    int msqid_starfleet;
    key_t key_starfleet;

    if ((key_spock = ftok("kirk2.c", 'A')) == -1) // Give it the ID A to talk to kirk2.c A. This links to spock
    {
        perror("ftok");
        exit(1);
    }
    if ((msqid_spock = msgget(key_spock, 0644 | IPC_CREAT)) == -1)
    {
        perror("msgget");
        exit(1);
    }

    if ((key_starfleet = ftok("kirk2.c", 'B')) == -1) // Give it the ID B to talk to kirk2.c B. This links to the starfleet
    {
        perror("ftok");
        exit(1);
    }

    if ((msqid_starfleet = msgget(key_starfleet, 0644 | IPC_CREAT)) == -1)
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

        // checks if all characters are uppercase. If it detects a lowercase character, then isUpper is false
        for (int i = 0; i < len - 1; i++)
        {
            if (buf.mtext[i] > 97)
            {
                isUpper = 0;
            }
        }

        // if isUpper is true, then it is an urgent message so we send to the starfleet (msqid_starfleet)
        if (isUpper)
        {
            if (msgsnd(msqid_starfleet, &buf, len + 1, 0) == -1) /* +1 for '\0' */
                perror("msgsnd");
        }
        else // otherwise, send to spock
        {
            if (msgsnd(msqid_spock, &buf, len + 1, 0) == -1) /* +1 for '\0' */
                perror("msgsnd");
        }
    }

    if (msgctl(msqid_spock, IPC_RMID, NULL) == -1)
    {
        perror("msgctl");
        exit(1);
    }

    return 0;
}
