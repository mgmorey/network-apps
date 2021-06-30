/*
 * File unix-server.c
 */

#include "unix-common.h"

#include <sys/socket.h>
#include <sys/un.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int connection_socket = -1;

static void clean_up(void)
{
    /* Close the socket. */
    if (connection_socket >= 0) {
        fprintf(stderr, "Closing socket %d.\n", connection_socket);
        close(connection_socket);
        connection_socket = -1;
    }

    /* Unlink the socket. */
    fprintf(stderr, "Removing file %s.\n", SOCKET_NAME);
    unlink(SOCKET_NAME);
}

int main(void)
{
    struct sockaddr_un name;
    int down_flag = 0;
    int ret;
    int data_socket;
    int result;
    char buffer[BUFFER_SIZE];

    /* Create local socket. */

    connection_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if (connection_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /*
     * For portability clear the whole structure, since some
     * implementations have additional (nonstandard) fields in
     * the structure.
     */

    memset(&name, 0, sizeof(name));

    /* Bind socket to socket name. */

    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);

    ret = bind(connection_socket, (const struct sockaddr *) &name,
               sizeof(name));
    if (ret == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    ret = atexit(clean_up);
    if (ret == -1) {
        perror("atexit");
        exit(EXIT_FAILURE);
    }
    
    /*
     * Prepare for accepting connections. The backlog size is set
     * to 20. So while one request is being processed other requests
     * can be waiting.
     */

    ret = listen(connection_socket, 20);
    if (ret == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    /* This is the main loop for handling connections. */

    for (;;) {

        /* Wait for incoming connection. */

        data_socket = accept(connection_socket, NULL, NULL);
        if (data_socket == -1) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        result = 0;
        for (;;) {

            /* Wait for next data packet. */

            ret = read(data_socket, buffer, sizeof(buffer));
            if (ret == -1) {
                perror("read");
                exit(EXIT_FAILURE);
            }

            /* Ensure buffer is 0-terminated. */

            buffer[sizeof(buffer) - 1] = 0;

            /* Handle commands. */

            if (!strncmp(buffer, "DOWN", sizeof(buffer))) {
                down_flag = 1;
                break;
            }

            if (!strncmp(buffer, "END", sizeof(buffer))) {
                break;
            }

            /* Add received summand. */

            result += atoi(buffer);
        }

        if (!down_flag) {
            /* Send result. */

            sprintf(buffer, "%d", result);
            ret = write(data_socket, buffer, sizeof(buffer));
            if (ret == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }

        /* Close socket. */

        close(data_socket);

        /* Quit on DOWN command. */

        if (down_flag) {
            break;
        }
    }

    exit(EXIT_SUCCESS);
}
