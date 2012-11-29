/* Name & E-mail: Celena Tan            tan.celena@gmail.com
 * Name & E-mail: Aseda Aboagye         aseda.aboagye@gmail.com
 * File Name: proxy.c
 * Course: COMP 177 Computer Networking
 * Project: Web Proxy
 * Created on: November 15, 2012
 * Last Edited: November 16, 2012
 */

#include <sys/types.h>
#include <stdio.h>			// Provides for printf, etc...
#include <stdlib.h>			// Provides for exit, ...
#include <argp.h>			// Provides GNU argp() argument parser
#include <getopt.h>
#include <errno.h>
#include <string.h>			// Provides for memset, ...
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include "wProxy.h"

//#define MAXDATASIZE 1024
//#define MAXDATASIZE_buffer 1024*1024
#define DEBUG 1
#define QUEUE 10
#define MAX_BUF_SIZE 1024
#define MAX_NAME_SIZE 255

//Set up the argument parser
const char *argp_program_version = "proxy 1.0";
const char *argp_program_bug_address = "<aseda.aboagye@gmail.com> or <tan.celena@gmail.com>";
static char doc[] = "Web Proxy";
static char args_doc[] = "";  // No standard arguments i.e. arguments without "names"

//Options.  Field 1 in ARGP. Order of fields: {NAME, KEY, ARG, FLAGS, DOC, GROUP}.
static struct argp_option options[] = {
		{"port",		'p', 	"Port",     0,  "REQUIRED: Port in which is listens on",							0 },
		{"security",	's', 	"subnet,subnet mask", 0,  "Limit incoming connections",			 				0 },
		{"verbose",		'v', 	0,          0,  "Provide verbose output, including server headers",	 	0 },
		{ 0, 			0, 		0, 			0, 	0, 														0 } // Last entry should be all zeros in all fields
};

//Our argp parser.
static struct argp argp = {options, parse_opt, args_doc, doc, 0, 0, 0};

int main(int argc, char **argv) {


	while(1) {
		int status;
		int sockfd = 0, sockfd_client=0;
		struct arguments arguments = { .verbose = 0, .port = 0, .security = "" };
		struct addrinfo hints;
		struct addrinfo *res;
		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;

		time_t t;
		time(&t);

		//Parse our arguments; every option seen by parse_opt will be reflected in arguments
		struct host_info host_info = { .host = "", .path = "", .port = 80 }; // Default values

		argp_parse (&argp, argc, argv, 0, 0, &arguments);

#if DEBUG
	printf ("User arguments: \nPort = %i\nSecurity = %s\nVerbose = %s\n", arguments.port, arguments.security, arguments.verbose ? "yes" : "no");
#endif
		/* TODO: Write subnet verifer */


		/* TODO: This is a list of things that I think we need to do.
		 *
		 * 1. Set up the argument parser. (argp stuff)								DONE!
		 * 2. Parse the arguments.													DONE!
		 * 3. Listen for requests and set up socket.(receiving requests)
		 *		For extra credit, we can fork() here. (LATER)
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
		 * 12. Send contents of buffer to web browser via the listening socket
		 * 13. Goto step 9 until server has nothing left to send
		 * 14. Close sockets														DONE!
		 * 15. Goto step 4
		 */

		//Set up socket (for receiving request)
		status = getaddrinfo(NULL, argv[1], &hints, &res);
		//First arg should be the site??
		//Second arg is the port so argv[2]


		if(status !=0){
			fprintf(stderr, "getaddrinfo ERROR: %s\n",gai_strerror(status));
			return 1;
		}

		sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

		if(sockfd < 0){
			perror("server: socket");
			return 1;
		}	/* rerun the server immediately after we kill it otherwise we have to wait about 20 secs.
			 * Eliminates "ERROR on binding: Address already in use" error. */

		int optval = 1;
		setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));
		status = bind(sockfd, res->ai_addr, res->ai_addrlen);

		if(status == -1){
			perror("server: bind");
			return 1;
		}

		//Listen for request
		status = listen(sockfd, QUEUE);

		if(status == -1){
			perror("server: listen");
			return 1;
		}

		struct sockaddr_storage their_addr;
		socklen_t addr_size = sizeof their_addr;
		sockfd_client = accept(sockfd, (struct sockaddr *) &their_addr, &addr_size);
		if(sockfd_client == -1){
			perror("server: accept");
			return 1;
		}

		//If security is checked
		if(arguments.security == 1) {
			//call security();
			//use getaddrinfo to get clients IP address
			//use subnet mask given
			//use subnet address given
		}

		if(arguments.security == 0) {
			//If no security check, proceed on
		}

		//Call parse_request(); to parse the browser request

		//Create own request by calling create_request();
		//All we need to do is append to the request given or if its not there, create the whole request

		//Set up socket with the given port and address

		//Send()

		while(1) { //Loop until its done receiving
			//Receive() and save into MAX_BUF_SIZE();

			//Call parse_response();

			//send buffer to web browser via listening socket
		}

		//Close sockets and free
		close(sockfd);
		close(sockfd_client);
		freeaddrinfo(res);
	}
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
    		arguments -> port = atoi(arg);
    		break;
    	case 's':
    		arguments -> security = arg;
    		break;

    	case ARGP_KEY_END: //Reached the last key.
    		//Check if our url and destdir REQUIRED "options" have been set to non-default values
    		//if (strcmp(arguments -> port, 0) == 0 || strcmp(arguments -> security, "") == 0) {
            if (arguments -> port == 0) { //A port number is required.
    			argp_usage (state);
    		}
    		break;

    	default:
    		return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

int security (struct host_info *h) { //Check if client IP is in subnet with the subnet mask

	return 0;
}

void parse_request() {

}

int create_request() {

	return 0;
}

void parse_response() {

}
