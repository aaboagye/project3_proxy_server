/* Name & E-mail: Celena Tan            tan.celena@gmail.com
 * Name & E-mail: Aseda Aboagye         aseda.aboagye@gmail.com
 * File Name: wProxy.h
 * Course: COMP 177 Computer Networking
 * Project: Web Proxy
 * Created on: November 15, 2012
 * Last Edited: November 15, 2012
 */

#ifndef WPROXY_H_
#define WPROXY_H_
#include <argp.h>

struct arguments {
    int verbose;
    int port;
    char *security;
};

static error_t parse_opt (int key, char *arg, struct argp_state *state);

#endif
