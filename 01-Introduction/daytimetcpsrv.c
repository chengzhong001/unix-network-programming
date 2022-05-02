#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE 4096
// #define LISTENQ 1024
#define SA struct sockaddr

// #define bzero(ptr, n) memset(ptr, 0, n)

static void err_doit(int errnoflag, const char *fmt, va_list ap)
{
    int errno_save;
    char buf[MAXLINE];

    errno_save = errno; /* value caller might want printed */
    vsprintf(buf, fmt, ap);
    if (errnoflag)
        sprintf(buf + strlen(buf), ": %s", strerror(errno_save));
    strcat(buf, "\n");
    fflush(stdout); /* in case stdout and stderr are the same */
    fputs(buf, stderr);
    fflush(stderr); /* SunOS 4.1.* doesn't grok NULL argument */
    return;
}

/* $f err_quit $ */
void err_quit(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(0, fmt, ap);
    va_end(ap);
    exit(1);
}

/* $f err_sys $ */
void err_sys(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(1, fmt, ap);
    va_end(ap);
    exit(1);
}

/* include Socket */
int Socket(int family, int type, int protocol)
{
    int n;
    if ((n = socket(family, type, protocol)) < 0)
        err_sys("socket error");
    return (n);
}

void Bind(int fd, const struct sockaddr *sa, socklen_t salen)
{
    if (bind(fd, sa, salen) < 0)
        err_sys("bind error");
}

void Listen(int fd, int backlog)
{
    char *ptr;
    /*4can override 2nd argument with environment variable */
    if ((ptr = getenv("LISTENQ")) != NULL)
        backlog = atoi(ptr);

    if (listen(fd, backlog) < 0)
        err_sys("listen error");
}

// int snprintf(char *buf, size_t size, const char *fmt, ...)
// {
//     int n;
//     va_list ap;

//     va_start(ap, fmt);
//     vsprintf(buf, fmt, ap); /* Sigh, some vsprintf's return ptr, not length */
//     n = strlen(buf);
//     va_end(ap);
//     if (n >= size)
//         err_quit("snprintf: '%s' overflowed array", fmt);
//     return (n);
// }

int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
    int n;

again:
    if ((n = accept(fd, sa, salenptr)) < 0)
    {
#ifdef EPROTO
        if (errno == EPROTO || errno == ECONNABORTED)
#else
        if (errno == ECONNABORTED)
#endif
            goto again;
        else
            err_sys("accept error");
    }
    return (n);
}

void Write(int fd, void *ptr, size_t nbytes)
{
    if (write(fd, ptr, nbytes) != nbytes)
        err_sys("write error");
}

void Close(int fd)
{
    if (close(fd) == -1)
        err_sys("close error");
}

int main(int argc, char const *argv[])
{

    int listenfd, connfd;
    int LISTENQ = 1024;
    struct sockaddr_in servaddr;
    char buff[MAXLINE];
    char *ptr;

    time_t ticks;
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    // bzero(&servaddr, sizeof(servaddr));
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(13); /* daytime server */

    // Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));
    if (bind(listenfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
        err_sys("bind error");

    // Listen(listenfd, LISTENQ);
    /*4can override 2nd argument with environment variable */
    if ((ptr = getenv("LISTENQ")) != NULL)
        LISTENQ = atoi(ptr);
    if (listen(listenfd, LISTENQ) < 0)
        err_sys("listen error");

    while (1)
    {
        connfd = Accept(listenfd, (SA *)NULL, NULL);
        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));

        // Write(connfd, buff, strlen(buff));
        if (write(connfd, buff, strlen(buff)) != strlen(buff))
            err_sys("write error");

        // Close(connfd);
        if (close(connfd) == -1)
            err_sys("close error");
    }

    return 0;
}
