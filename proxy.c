/* Name & E-mail: Celena Tan            tan.celena@gmail.com
 * Name & E-mail: Aseda Aboagye         aseda.aboagye@gmail.com
 * File Name: proxy.c
 * Course: COMP 177 Computer Networking
 * Project: proxy server
 * Created on: November 15, 2012
 * Last Edited: November 15, 2012 */

#include <stdio.h>

int main(int argc, char **argv){
	/* TODO: This is a list of things that I think we need to do.
	 * 
	 * 1. Set up the argument parser. (argp stuff)
	 * 2. Parse the arguments.
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
	return 0;
}
