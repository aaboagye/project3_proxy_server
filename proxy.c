/* Name & E-mail: Celena Tan            tan.celena@gmail.com
 * Name & E-mail: Aseda Aboagye         aseda.aboagye@gmail.com
 * File Name: proxy.c
 * Course: COMP 177 Computer Networking
 * Project: Web Proxy
 * Created on: November 15, 2012
 * Last Edited: November 15, 2012
 */

#include <sys/types.h>
#include <stdio.h>			// Provides for printf, etc...
#include <stdlib.h>			// Provides for exit, ...
#include <argp.h>			// Provides GNU argp() argument parser
#include <getopt.h>
#include <errno.h>
#include <string.h>			// Provides for memset, ...
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include "wProxy.h"

#define MAXDATASIZE 1024
#define MAXDATASIZE_buffer 1024*1024
#define DEBUG 1

//Set up the argument parser
const char *argp_program_version = "swget 1.0";
const char *argp_program_bug_address = "<aseda.aboagye@gmail.com> or <tan.celena@gmail.com>";
static char doc[] = "Web Proxy";
static char args_doc[] = "";  // No standard arguments i.e. arguments without "names"

//Options.  Field 1 in ARGP. Order of fields: {NAME, KEY, ARG, FLAGS, DOC, GROUP}.
static struct argp_option options[] = {
		{"port",		'p', 	"Port",     0,  "Port in which is listens on",							0 },
		{"security",	's', 	"Security", 0,  "Limit incoming connections",			 				0 },
		{"verbose",		'v', 	0,          0,  "Provide verbose output, including server headers",	 	0 },
		{ 0, 			0, 		0, 			0, 	0, 														0 } // Last entry should be all zeros in all fields
};

//Our argp parser.
static struct argp argp = {options, parse_opt, args_doc, doc, 0, 0, 0};

int main(int argc, char **argv) {
	struct arguments arguments = { .verbose = 0, .port = 0, .security = "" };

	time_t t;
	time(&t);

	argp_parse (&argp, argc, argv, 0, 0, &arguments);

	printf ("User arguments: \nPort = %i\nSecurity = %s\nVerbose = %s\n", arguments.port, arguments.security, arguments.verbose ? "yes" : "no");

	/* TODO: This is a list of things that I think we need to do.
	 *
	 * 1. Set up the argument parser. (argp stuff)								DONE! (Just need to fix two warnings)
	 * 2. Parse the arguments.													DONE!
	 * 3. Listen for requests and set up socket.(receiving requests)
	 *		For extra credit, we can fork() here.
	 * 4. Check to see if security option is set.
	 *		If so, check to see if client IP is in the given subnet.
	 *		We can calculate this by looking at the given subnet mask.
	 * 5. Parse the browser request.
	 * 6. Create our own request.
	 * 7. Set up socket for talking to server(web page).
	 * 8. Send our request to server.
	 * 9. Receive response from server.
	 * 10. Parse the response.
	 * 11. Save data in a small buffer (~64KB)
	 * 12. Send contents of buffer to web browser via the listening socket.
	 * 13. Goto step 9 until server has nothing left to send.
	 * 14. Close sockets and Goto step 4
	 */



	return (0);
}

// Parser. Field 2 in ARGP. Parse a single option.
// Order of parameters: KEY, ARG, STATE.
static error_t parse_opt (int key, char *arg, struct argp_state *state) {
	//Get the input argument from argp_parse, which we know is a pointer to our arguments structure
	struct arguments *arguments = state -> input;

	//Figure out which option we are parsing, and decide how to store it
	switch (key) {
    	case 'v':
    		arguments -> verbose = 1;
    		break;
    	case 'p':
    		arguments -> port = arg;
    		break;
    	case 's':
    		arguments -> security = arg;
    		break;

    	case ARGP_KEY_END: //Reached the last key.
    		//Check if our url and destdir REQUIRED "options" have been set to non-default values
    		if (strcmp(arguments -> port, 0) == 0 || strcmp(arguments -> security, "") == 0) {
    			argp_usage (state);
    		}
    		break;

    	default:
    		return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

