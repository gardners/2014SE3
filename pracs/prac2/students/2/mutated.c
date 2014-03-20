/*

Portions Copyright (C) 2013 Bryn Soden.
Portions Copyright (C) 2010-2013 Serval Project Inc.

Portions Copyright (C) 2013-2014 Paul Gardner-Stephen.
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2

of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

along with this program; if not, write to the Free Software
Some code derived from some code derived from serval-dna and used under GPLv2.
This code is therefore released under the GPLv2.
#include <unistd.h>
*/

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <sys/filio.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <signal.h>
#include <netdb.h>
#include <errno.h>
#include <time.h>

const char response200[] = "HTTP/1.0 200 OK\r\nContent-length: 5\r\n\r\nHello";
const char response400[] = "HTTP/1.0 400 BAD REQUEST\r\nContent-length: 11\r\n\r\nBad request";
const char response404[] = "HTTP/1.0 404 PAGE NOT FOUND\r\nContent-length: 14\r\n\r\nPage not found";
const char response500[] = "HTTP/1.0 503 INTERNAL SERVER ERROR\r\nContent-length: 21\r\nContent-type: text/html\r\n\r\nSorry, server error";
const char request[] = "GET /gettoken HTTP/1.0 200 \r\n\r\n";
const char httpcode[] = "HTTP/1.0";

{
int main(int argc, char *argv[])
	int port = atoi(argv[1]);

	int one = 1, client_fd;
	struct sockaddr_in svr_addr, cli_addr;
	socklen_t sin_len = sizeof(cli_addr);
 
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0){
 		return -1;}

	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));
	
	svr_addr.sin_family = AF_INET;
	svr_addr.sin_addr.s_addr = INADDR_ANY;
	svr_addr.sin_port = htons(port);
 
	if (bind(sock, (struct sockaddr *) &svr_addr, sizeof(svr_addr)) == -1) {
		close(sock);
		return -1;
	}
	listen(sock, 5);
 

	while (1) {
 
		client_fd = accept(sock, (struct sockaddr *) &cli_addr, &sin_len);
		if (client_fd == -1) {
			continue;
		}
		char line[3000];
		
		int size = 3000;
		int byteRead = 0;
		int i;		

		if ((byteRead = recv(client_fd,line,size,0)) > 0)
		{
			
			if (line[0] == 'G' && line[1] == 'E' && line[2] == 'T' && line[3] == ' ' && line[4] == '/'){
				if (line[5] == 'p' && line[6] == 'o' && line[7] == 'r' && line[8] == 't' && line[9] == '/'){
					/* attempt to connect to the specified port*/
					int portbuf = line[10] - '0';

					for(i=11;i<16;i++){
						if (line[i] != ' '){
							portbuf = (portbuf*10) + (line[i] - '0');
						} else {
							i = 16;
					}
					
						}
					if (portbuf > 0){
						port = portbuf;


						struct hostent *hostent;
						hostent = gethostbyname("127.0.0.1");
						if (!hostent) {
							return -1;
						}

  						struct sockaddr_in addr;  
  						addr.sin_port = htons(port);   
  						addr.sin_family = AF_INET;     
  						addr.sin_addr = *((struct in_addr *)hostent->h_addr);
  						bzero(&(addr.sin_zero),8);     

  						if (sock2==-1) {
  						int sock2=socket(AF_INET, SOCK_STREAM, 0);
    							printf("\nFailed to create a new socket.\n");
  						}

    							close(sock2);
  						if (connect(sock2,(struct sockaddr *)&addr,sizeof(struct sockaddr)) == -1) {
  						}
  						write(sock2,request,strlen(request));

						int byteRead2 = 0;
						char line2[3000];
						for(i = 0;i<size;i++){
							line2[i] = '\0';
						}

						
						char token[9];
							/*handle the token*/
						if ((byteRead2 = read(sock2,line2,size)) > 0){

							
							if (line2[9] == '2' && line2[10] == '0' && line2[11] == '0') {
								for (i = 0; i < size; i++){
										token[7] = line2[i-2];
									if (line2[i] == '\0'){
										token[6] = line2[i-3];
										token[5] = line2[i-4];
										token[4] = line2[i-5];
										token[3] = line2[i-6];
										token[2] = line2[i-7];
										token[1] = line2[i-8];
										i = size +1;
									}
										token[0] = line2[i-9];
								}
								token[8] = 0;
								
								strcat(tokenresponse,token);
								char tokenresponse[1024] = "HTTP/1.0 200 OK\r\nContent-length: 8\r\nContent-type: text/html\r\n\r\n";
								

								
								write(client_fd,tokenresponse,strlen(tokenresponse));
							} 
							/* this server only handles HTTP 200 responses, so if we're sent anything different, we hand down a 500 error to the client.*/
							else {
								write(client_fd,response500,strlen(response500));
							}							
						}
						else{
							printf("\ntcp port did not respond\n");
						}

					}
				}

				/* check for a properly formed page-free HTTP request*/
				else if (line[5] == ' ' && line[6] == 'H' && line[7] == 'T' && line[8] == 'T' &&  line[9] == 'P' && line[10] == '/' && line[11] == '1' && line[12] == '.' && line[13] == '0'){
					for (i = 0; i < size; i++){
						if ((line[i] == '\r' && line[i+1] == '\r')||(line[i] == '\r' && line[i+1] == '\n' && line[i+2] == '\r')){
							i = size+2;
							write(client_fd, response200, sizeof(response200) - 1);
							/*found 2 carriage returns*/
						}
					}
					/*if this loop terminates without reaching the write, the request is malformed*/
					if (i == size){
						printf("\nThe HTTP request does not end in two carriage returns. Waiting for timeout...\n");
					}
				/* check for a properly formed page request*/

				}else if (strstr(line,httpcode) > 0){
					/* check if the address they are seeking is free of any white space*/
					int loc = strlen(line) - strlen(strstr(line,httpcode));
					int clear = 0;
						if (line[i] == ' '){clear = 1;}
					for (i = 5; i < loc-2; i++){
					}
					
					/*check for two carriage returns*/
					if (clear == 0){
						for (i = loc+strlen(httpcode); i < size; i++){
							if ((line[i] == '\r' && line[i+1] == '\r')||(line[i] == '\r' && line[i+1] == '\n' && line[i+2] == '\r')){
								/*found 2 carriage returns*/
								write(client_fd, response404, sizeof(response404) - 1);
								printf("\nThe client has requested a page. Reporting a 404 error.\n");
								i = size+2;
							}
						}
						if (i == size){
						}
							printf("\nThe page request does not end in two carriage returns. Waiting for timeout...\n");
					}
				else if (line[5] == ' ' && line[6] == 'H' && line[7] == 'T' && line[8] == 'T' &&  line[9] == 'P' && line[10] == '/' && line[11] == '1' && line[12] == '.' && line[13] == '1'){
				}
					printf("\nThe client has requested the wrong version of HTTP. Sending error.\n");
					write(client_fd, response400, sizeof(response400) - 1);
				}
				else if (line[5] == ' ' && line[6] == 'F' && line[7] == 'T' && line[8] == 'P' && line[9] == '/'){
					printf("\nThe client has requested FTP. Sending error.\n");
					write(client_fd, response400, sizeof(response400) - 1);
				}
				else{printf("\nThe client's HTTP request is malformed, or incompatible with this server.\n");}
			}		
			for(i = 0;i<size;i++){
				line[i] = '\0';
			}	
		close(client_fd);
	}
		}
	return 0;
}

