/* Name & E-mail: Celena Tan            tan.celena@gmail.com
 * Name & E-mail: Aseda Aboagye         aseda.aboagye@gmail.com
 * File Name: proxy.c
 * Course: COMP 177 Computer Networking
 * Project: Web Proxy
 * Created on: November 15, 2012
 * Last Edited: December 3, 2012
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
#define MAX_DATA_SIZE 1024
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
		int status, len;
		int optval = 1;
		int sockfd = 0, sockfd_client = 0, sockfd1 = 0;
		char *nRequest = malloc(MAX_BUF_SIZE);
		char response[MAX_BUF_SIZE];
		char request[MAX_BUF_SIZE];

		struct arguments arguments = { .verbose = 0, .port = 0, .security = "" };
		struct addrinfo hints;
		struct addrinfo *res;

		struct addrinfo peer;
		struct addrinfo *peerinfo;

		memset(&hints, 0, sizeof hints);

		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;

	    peer.ai_family = AF_UNSPEC;     //IPv4 or IPv6
	    peer.ai_socktype = SOCK_STREAM; //TCP stream sockets
	    peer.ai_flags = AI_CANONNAME;   //Fill in my IP for me
	    peer.ai_protocol = 0;

		time_t t;
		time(&t);

		//Parse our arguments; every option seen by parse_opt will be reflected in arguments
		struct host_info host_info = { .host = "", .path = "", .port = 80 , .ip = ""}; // Default values

		argp_parse (&argp, argc, argv, 0, 0, &arguments);

#if DEBUG
	printf ("User arguments: \nPort = %s\nSecurity = %s\nVerbose = %s\n", arguments.port, arguments.security, arguments.verbose ? "yes" : "no");
#endif
		/* TODO: Write subnet verifer */

		/* TODO: This is a list of things that I think we need to do.
		 *
		 * 1. Set up the argument parser. (argp stuff)								DONE!
		 * 2. Parse the arguments.													DONE!
		 * 3. Listen for requests and set up socket.(receiving requests)
		 *		For extra credit, we cans fork() here. (LATER)
		 * 4. Check to see if security option is set.								1/2 DONE!
		 *		If so, check to see if client IP is in the given subnet.
		 *		We can calculate this by looking at the given subnet mask.
		 * 5. Parse the browser request.											DONE!
		 * 6. Create our own request.												DONE!
		 * 7. Set up socket for talking to server(web page).						DONE!
		 * 8. Send our request to server											DONE!
		 * 9. Receive response from server
		 * 10. Parse the response
		 * 11. Save data in a small buffer (~64KB)
		 * 12. Send contents of buffer to web browser via the listening socket
		 * 13. Goto step 9 until server has nothing left to send
		 * 14. Close sockets														DONE!
		 * 15. Goto step 4
		 */

		//Set up socket (for receiving request)
		status = getaddrinfo(NULL, arguments.port, &hints, &res);
		//First arg should be the site??
		//Second arg is the port so argv[2]

		if(status !=0){
			fprintf(stderr, "getaddrinfo ERROR: %s\n", gai_strerror(status));
			return 1;
		}

		sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

		if(sockfd < 0){
			perror("server: socket");
			return 1;
		}	/* rerun the server immediately after we kill it otherwise we have to wait about 20 secs.
			 * Eliminates "ERROR on binding: Address already in use" error. */

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

		//IMPLEMENT SECURITY AT THE END SINCE ITS ONLY WORTH 4 POINTS
		//If security is checked
		// if(arguments.security == 1) {
		// 	//call security();
		// 	//use getaddrinfo to get clients IP address
		// 	//use subnet mask given
		// 	//use subnet address given
		// }

		// if(arguments.security == 0) {
		// 	//If no security check, proceed on
		// }

		//Call parse_request(); to parse the browser request
		parse_request(request, &host_info); //Should only give path so we can do

		//Create own request by calling create_request();
		strcpy(nRequest, create_request(request, nRequest, &host_info, &hints));

		printf("Request created\n");

		//Set up socket with the given port and address
		if ((status = getaddrinfo(host_info.host, arguments.port, &peer, &peerinfo)) != 0) {
			fprintf(stderr, "Getaddrinfo ERROR1: %s\n", gai_strerror(status)); //GAI
		    exit(EXIT_FAILURE);
		}

		printf("Getaddrinfo succeeded\n");

		//Creating socket
		if((sockfd1 = socket(peerinfo -> ai_family, peerinfo -> ai_socktype, peerinfo -> ai_protocol)) < 0){
		    perror("Socket()");
		    exit(EXIT_FAILURE);
		}

		printf("Socket established\n");

		//Connecting to the server
		if(connect(sockfd1, peerinfo -> ai_addr, peerinfo -> ai_addrlen)) {
		    perror("Connect()");
		    exit(EXIT_FAILURE);
		}

		printf("Connection established\n");

		len = strlen(nRequest);

		//Send() to server
		if (sendall(sockfd1, nRequest, &len) == -1) {
		    perror("Sendall()");
		    printf("We only sent %d bytes because of the error!\n", len);
		}

		//Receiving response... Read back from server
		while(1) { //Loop until its done receiving
			//Receive() and save into MAX_BUF_SIZE();
			//Call parse_response();
			//Send buffer to web browser via listening socket
		}

		//Close sockets and free
		close(sockfd);
		close(sockfd1);
		close(sockfd_client);
		freeaddrinfo(res);
		free(nRequest);
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
    		arguments -> port = arg;
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

int sendall(int s, char *buf, int *len) {
    int total = 0;        	// how many bytes we've sent
    int bytesleft = *len; 	// how many we have left to send
    int n;

    while(total < *len) {
        n = send(s, buf+total, bytesleft, 0);
        if (n == -1) { break; }
        total += n;
        bytesleft -= n;
    }

    *len = total; // return number actually sent here

    return n == -1?-1:0; // return -1 on failure, 0 on success
}

int security (struct host_info *h) { //Check if client IP is in subnet with the subnet mask

	return 0;
}

static void parse_request(char *request, struct host_info *h) { //parse the broswer request
	char *it1, *it2;
	int len;

	it1 = request + 0; //Set the pointer to the beginning of the request

// host
	for(it2 = it1; *it2 != 0; it2++)
		if(*it2 == '/')
			break;

	len = it2 - it1;

// path
	it1 = it2;
	for(; *it2 != 0; it2++)	{}
	len = it2 - it1;
	h -> path = (char *)malloc(len + 1);
	strncpy(h -> path, it1, len);
	h -> path[len] = 0;
	printf("%s\n", h -> path);
}

char *create_request(char *request, char *nRequest, struct host_info *h, struct addrinfo *a) {
	//Call this after call parse_request, parse_request gets the hostname
	//Take out the GET line and add in new GET line
	//Append the rest of the response on except the GET line
	//TODO: Take out the "Proxy-Connection: Close" line
	//Add the Via header line
	//Add the X-Forwarded-For header
	char *it1, *it2;
	int count = 0, len;

	//Create GET header here
	strcpy(nRequest, "GET ");
	strcat(nRequest, h->host);
	if(h->path[0] != '/')
		strcat(request, "/");		//In order to not put an extra '/'
	strcat(nRequest, h->path);
	strcat(nRequest, " HTTP/1.1\r\n");  //This should take care of the GET line
										//Now to append the rest of the request
	it1 = request + 0;

	for(it2 = it1; *it2 != 0; it2++)
		if(*it2 == 'H')
			if(strncmp(it2, "Host:", 5) == 0) //This means its at the Host header
				break;

	it1 = it2; //Reset it1 to the begin at Host since we dont want their GET

	for(; *it2 != 0; it2++) {
		if(*it2 == 'P') {
			if(strncmp(it2, "Proxy-Connection:", 17) == 0) { //We need to take this header out
				count++;
				break;
			}
		}
	}

	len = it2 - it1;
	strncat(nRequest, it1, len); //Copy everything from Host to before the Proxy-Connection

	//If there was a Proxy-Connection header, it should've broke out of the for loop
	if(count > 0) {
		for(; *it2 != 0; it2++) {
			if(strncmp(it2, "\r\n", 2) == 0) { //Now look for the end of the Proxy-Connection header & skip over it
				break;
			}
		}

		it2 += 4; //Reinitialize the pointer to after the \r\n
		it1 = it2;
	}

	for(; *it2 != 0; it2++) {} //Now to the end of the request

	len = it2 - it1;
	strncat(nRequest, it1, len);

	strcat(nRequest, "Via: "); //Via: the protocol version, proxy IP, and proxy name
	strcat(nRequest, a->ai_canonname); //We need to somehow get our IP and insert here
	strcat(nRequest, "\r\n");

	strcat(nRequest, "X-Forwarded-For: "); //X-Forwarded-For: Client IP
	strcat(nRequest, h->ip);
	strcat(nRequest, "\r\n");


	return nRequest;
}

void parse_response() {

}
