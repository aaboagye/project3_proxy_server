/* Name & E-mail: Celena Tan            tan.celena@gmail.com
 * Name & E-mail: Aseda Aboagye         aseda.aboagye@gmail.com
 * File Name: main.c
 * Course: COMP 177 Computer Networking
 * Project: Web Proxy
 * Created on: November 15, 2012
 * Last Edited: December 3, 2012
 */

#ifndef WPROXY_H_
#define WPROXY_H_
#include <argp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

struct arguments {
    int verbose;
    char *port;
    char *security;
};

struct host_info {
    char *host;
    char *ip;
    int port;
    char *path;
    char *user;
};

static error_t parse_opt (int key, char *arg, struct argp_state *state);

int security (struct host_info *h);

static void parse_request(char *request, struct host_info *h);

char *create_request(char *request, struct host_info *h, struct addrinfo *a);

#endif