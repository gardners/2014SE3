/*
  A test framework for Networks 2013 assignment 1 to allow for
  automated marking and self-assessment.

  Some code derived from serval-dna and used under GPLv2.

  This code is therefore released under the GPLv2.

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/filio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>
#include <signal.h>
#include <netdb.h>
#include <time.h>
#include <errno.h>

pid_t student_pid=-1;

int student_port;

int success=0;

char *gradeOf(int score)
{
  if (score<50) return "F";
  if (score<65) return "P";
  if (score<75) return "CR";
  if (score<85) return "DN";
  return "HD";
}

int create_listen_socket(int port)
{
  int sock = socket(AF_INET,SOCK_STREAM,0);
  if (sock==-1) return -1;

  int on=1;
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) == -1) {
    close(sock); return -1;
  }
  if (ioctl(sock, FIONBIO, (char *)&on) == -1) {
    close(sock); return -1;
  }
  
  /* Bind it to the next port we want to try. */
  struct sockaddr_in address;
  bzero((char *) &address, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);
  if (bind(sock, (struct sockaddr *) &address, sizeof(address)) == -1) {
    close(sock); return -1;
  } 

  if (listen(sock, 20) != -1) return sock;

  close(sock);
  return -1;
}

int accept_incoming(int sock)
{
  struct sockaddr addr;
  unsigned int addr_len = sizeof addr;
  int asock;
  if ((asock = accept(sock, &addr, &addr_len)) != -1) {
    return asock;
  }

  return -1;
}

int connect_to_port(int port)
{
  struct hostent *hostent;
  hostent = gethostbyname("127.0.0.1");
  if (!hostent) {
    return -1;
  }

  struct sockaddr_in addr;  
  addr.sin_family = AF_INET;     
  addr.sin_port = htons(port);   
  addr.sin_addr = *((struct in_addr *)hostent->h_addr);
  bzero(&(addr.sin_zero),8);     

  int sock=socket(AF_INET, SOCK_STREAM, 0);
  if (sock==-1) {
    perror("Failed to create a socket.");
    return -1;
  }

  if (connect(sock,(struct sockaddr *)&addr,sizeof(struct sockaddr)) == -1) {
    perror("connect() to port failed");
    close(sock);
    return -1;
  }
  return sock;
}

int read_from_socket(int sock,unsigned char *buffer,int *count,int buffer_size)
{
  int t=time(0);
  if (*count>=buffer_size) return 0;
  int r=read(sock,&buffer[*count],buffer_size-*count);
  while(r!=0) {
    if (r>0) {
      (*count)+=r;
      t=time(0);
    }
    r=read(sock,&buffer[*count],buffer_size-*count);
    if (r==-1&&errno!=EAGAIN) {
      perror("read() returned error. Stopping reading from socket.");
      return -1;
    }
    // timeout after a few seconds of nothing
    if (time(0)-t>10) break;
  }
  buffer[*count]=0;
  return 0;
}

int getHttpReplyCode(char *reply)
{
  int code=-1;
  if (sscanf(reply,"HTTP/1.0 %d",&code)==1) {
    return code;
  }
  return -1;
}

int getContentLength(char *reply)
{
  int l=strlen(reply);
  int i;
  for(i=0;i<l;i++)
    {
      if (reply[i]=='\r'||reply[i]=='\n') {
	if (reply[i+1]=='\r') {
	  // end of headers - so stop looking
	  // there is no content length field
	  return -1;
	} else if (!strncasecmp(&reply[i+1],"Content-length: ",16)) {
	  int content_length=atoi(&reply[i+1+16]);
	  if (content_length<0) return -5;
	  if (content_length>l) return -6;
	  // now check that content length is correct
	  if (reply[l-content_length-1]!='\r'
	      &&reply[l-content_length-1]!='\n')
	    // content doesn't start immediately after a CR or LF
	    return -2;
	  if (reply[l-content_length-1-1]!='\r'
	      &&reply[l-content_length-1-1]!='\n')
	    // content doesn't start immediately after two CR or LF characters
	    return -3;
	  if (reply[l-content_length-1-1-1]>=' '
	      &&reply[l-content_length-1-1]=='\r'
	      &&reply[l-content_length-1-1]=='\n')
	    // CR+LF for end of header line, but without it being a blank link
	    return -4;
	  return content_length;
	}	
      }
    }
  // no content length field.
  return -1;
}

int launch_student_programme(const char *executable)
{
  // Find a free TCP port for the student programme to listen on
  // that is not currently in use.
  student_port=(getpid()|0x8000)&0xffff;
  int portclear=0;
  while(!portclear&&(student_port<65536)) {
    int sock=connect_to_port(student_port);
    if (sock==-1) portclear=1; 
    else close(sock);
  }
  fprintf(stderr,"Port %d is available for use by student programme.\n",
	  student_port);

  pid_t child_pid = fork();

  if (child_pid==-1) {
    perror("fork"); return -1;
  }
  char port[128];
  snprintf(port,128,"%d",student_port);
  const char *const args[]={executable,port,NULL,NULL};

  if (!child_pid) {
    // as the child: so exec() to the student's program
    execv(executable,(char **)args);
    /* execv doesn't return if it is successful */
    perror("execv");
    return -1;
  }
  /*
    parent: just remember the PID so that we can kill it later
  */
  student_pid=child_pid;
  return 0;
}

int test_listensonport()
{
  /* Test that student programme accepts a connection on the port */
  int i;
  for(i=0;i<10;i++) {
    int sock=connect_to_port(student_port);
    if (sock>-1) {
      close(sock);
      printf("SUCCESS: Accepts connections on specified TCP port\n");
      success++;
      return 0;
    }
    // allow some time for the student programme to get sorted.
    // 100ms x 10 times should be enough
    usleep(100000);
  }
  printf("FAIL: Accepting connection on a TCP port.\n");
  return -1;
}

int test_acceptmultipleconnections()
{
  /* Test that student programme accepts 1,000 successive connections.
     Further test that it can do so within a minute. */
  int start_time=time(0);
  int i;
  for(i=0;i<20;i++) {
    int sock=connect_to_port(student_port);
    // Be merciful with student programs that are too slow to take 1,000 connections
    // coming in really fast.
    if (sock==-1) {
      if (i<10) usleep(100000); else usleep(1000);
      sock=connect_to_port(student_port);
    }
    if (sock==-1) {
      printf("FAIL: Accepting multiple connections on a TCP port (failed on attempt %d).\n",i);
      return -1;
    }
    close(sock);
    // allow upto 5 minutes to handle the 1,000 connections.
    if (time(0)-start_time>300) break;
  }
  
  int end_time=time(0);
  
  if (i==1000)
    { printf("SUCCESS: Accepting multiple connections on a TCP port\n"); success++; }
  else
    printf("FAIL: Accepting multiple connections on a TCP port. Did not complete 1,000 connections in less than 5 minutes.\n");

  if (end_time-start_time>60)
    printf("FAIL: Accept 1,000 connections in less than a minute.\n");
  else 
    { printf("SUCCESS: Accepted 1,000 connections in less than a minute.\n");
      success++; }

  return 0;
}

struct query_reply_pair {
  char *query;
  int expected_http_code;
};

char pbuf[1024];
char *p(char *s)
{
  int i;
  int olen=0;
  for(i=0;s[i]&&(olen<1024-2);i++)
    {
      if (s[i]<' ') {
	switch(s[i]) {
	case '\r': pbuf[olen++]='\\'; pbuf[olen++]='r'; break;
	case '\n': pbuf[olen++]='\\'; pbuf[olen++]='n'; break;
	default: pbuf[olen++]='?'; break;
	}
      } else 
	pbuf[olen++]=s[i];
    }
  pbuf[olen]=0;  return pbuf;
}

int test_understandshttp1()
{
  /* Test that student programme accepts a connection on the port */

  struct query_reply_pair q[]={
    // Valid requests
    {"GET / HTTP/1.0\r\n\r\n",200},
    {"GET / HTTP/1.0\r\r",200},
    // Incomplete request (should eventually timeout?)
    {"GET / HTTP/1.0",0},
    {"GET / HTTP/1.0\r\n",0},
    // Invalid protocol or version
    {"GET / FTP/1.0\r\n\r\n",-1},
    {"GET / HTTP/1.1\r\n\r\n",-1},
    // no such page request
    {"GET /this_page_should_not_exist HTTP/1.0\r\n\r\n",404},

    {NULL,0}
  };

  int i;
  for(i=0;q[i].query;i++) {
    int sock=connect_to_port(student_port);
    if (sock==-1) {
      printf("FAIL: Could not connect to test acceptance of HTTP/1.0 request\n");
      return -1;
    }
    write(sock,q[i].query,strlen(q[i].query));
    char buffer[1024];
    int count=0;
    read_from_socket(sock,(unsigned char *)buffer,&count,sizeof(buffer));
    if (q[i].expected_http_code>0) {
      // expect the provided http response code
      if (!count) printf("FAIL: HTTP request '%s' did not return HTTP response code %d (got nothing at all)\n",p(q[i].query),q[i].expected_http_code);
      else {
	int code=getHttpReplyCode(buffer);
	if (code!=q[i].expected_http_code) {
	  if (code==-1) 
	    printf("FAIL: HTTP request '%s' did not return HTTP response code %d (got %d bytes, which don't seem to be a HTTP response)\n",p(q[i].query),q[i].expected_http_code,count);
	  else
	    printf("FAIL: HTTP request '%s' did not return HTTP response code %d (returned HTTP response code %d instead)\n",p(q[i].query),q[i].expected_http_code,code);
	} else
	  { printf("SUCCESS: HTTP request '%s' returned HTTP response code %d\n",p(q[i].query),q[i].expected_http_code);
	    success++;
	  }
	  
      }
    } else if (q[i].expected_http_code<0) {
      // expect an http error
      int code=getHttpReplyCode(buffer);
      if (code<200||code>=300) {
	printf("SUCCESS: HTTP request '%s' correctly reported an HTTP error\n",p(q[i].query));
	success++;
      } else if (!count) printf("FAIL:HTTP request '%s' did not return an HTTP error (got nothing at all)\n",p(q[i].query));
      else {
	printf("FAIL: HTTP request '%s' did not return an HTTP error (got %d bytes)\n",p(q[i].query),count);
      }
    } else {
      // expect timeout or, dropped connection without
      // a valid (non-timeout indicating) http reply
      if (!count) { printf("SUCCESS: HTTP request '%s' returned nothing, a dropped connection or HTTP request timeout indication.\n",p(q[i].query)); success++; }
      else {
	printf("FAIL: HTTP request '%s' was supposed to return nothing (or an HTTP request timeout), but output %d bytes.\n",p(q[i].query),count);
      }
    }
  }      
  return 0;
}

int test_contentlengthfield()
{
  int sock=connect_to_port(student_port);
  if (sock==-1) {
    printf("FAIL: Could not connect to test acceptance of HTTP/1.0 request\n");
    return -1;
  }
  write(sock,"GET / HTTP/1.0\r\n\r\n",strlen("GET / HTTP/1.0\r\n\r\n"));
  char buffer[1024];
  int count=0;
  read_from_socket(sock,(unsigned char *)buffer,&count,sizeof(buffer));
  int content_length=getContentLength(buffer);
  if (content_length>0) 
    { printf("SUCCESS: Response includes valid and correct content-length field\n");
      success++; }
  else
    printf("FAIL: Response does not include valid and correct content-length field (error code %d)\n",content_length);
  return 0;
}

int httpReply(int s,int code,char *reply_msg,char *headers,char *body)
{
  char buffer[strlen(body)+strlen(headers)+strlen(reply_msg)+1024];

  sprintf(buffer,"HTTP/1.0 %03u %s\r\nContent-length: %u\r\n%s\r\n\r\n%s",
	  code,reply_msg,(int)strlen(body),headers,body);

  int l=strlen(buffer);
  int o=0;
  while(o<l) {
    int w=write(s,&buffer[o],l-o);
    if (w>0) o+=w;
    if (w<0&&errno!=EAGAIN) break;
  }

  if (o==l) return 0; else return -1;
}

unsigned int token;
int readToken;
int allowTokenFetch=1;

int parseAndRespondToHTTPRequest(int s,char *buffer, int count)
{
  int ret_code=0;

  // Parse request
  int i;
  int l1=0;
  int hl=0;
  int state=0;
  char line1[128];
  for(i=0;i<count;i++) {
    switch(state) {
    case 0: // first line
      if (buffer[i]=='\r') {
	if (buffer[i+1]=='\n') { i++; }
	line1[l1]=0;
	state=1;
      } else if (buffer[i]=='\n') {
	line1[l1]=0;
	state=1;
      } else {
	if (l1<128) line1[l1++]=buffer[i];
      }
      break;
    case 1: // header lines (to be ignored until we find an empty one
      if (buffer[i]=='\r') {
	if (buffer[i+1]=='\n') { i++; }
	if (!hl) state=2;
	hl=0;
      } else if (buffer[i]=='\n') {
	if (!hl) state=2;
	hl=0;
      } 
      if (state!=2) break;
      if (buffer[i]=='\n'&&(i==count-1)) i++;
      if (buffer[i]=='\r'&&(i==count-1)) i++;
    case 2:
      // end of headers.
      if (i<count) {
	// junk at end of request
	httpReply(s,599,"Malformed request (junk after end of request)","","");
	state=3;
	ret_code=-1;
      } else {
	// We have a request
	char method[1024], url[1024], protocol[1024];
	int vmajor,vminor;
	if (sscanf(line1,"%s %s %[^/]/%d.%d",method,url,protocol,&vmajor,&vminor)
	    !=5) {
	  httpReply(s,590,
		    "Malformed request (could not parse HTTP request)","","");
	  ret_code=-2;
	}
	else if (strcasecmp(protocol,"http")) {
	  httpReply(s,591,"Unsupported protocol","","");
	  ret_code=-3;
	} else if (vmajor!=1||vminor!=0) {
	  httpReply(s,592,"Unsupported protocol version.","","");
	  ret_code=-4; 
	} else if (strcasecmp(method,"get")) {
	  httpReply(s,593,"Unsupported method","","");
	  ret_code=-5;
	} else if (!strcasecmp(url,"/gettoken")) {
	  char token_string[1024];
	  if (allowTokenFetch) {
	    sprintf(token_string,"%08x\n",token);
	    httpReply(s,200,"OK","Content-type: text/html",token_string);
	  }
	  else
	    httpReply(s,503,"No fish today","Content-type: text/html","Sorry, no fish today.\n");
	  readToken=1;
	}
	else {
	  httpReply(s,404,"Page not found","Content-type: text/html",
		    "Page not found");
	  ret_code=-6;
	}
	state=3;
      }
      break;
    case 3:
      // Request handled
      break;
    }
  }
  close(s);      
  
  return ret_code;
}

int test_outboundConnection()
{
  // Run through twice, once allowing fetching of the token, and once
  // returning an error, and making sure that the student's program
  // reacts appropriately to both.
  for(allowTokenFetch=0;allowTokenFetch<2;allowTokenFetch++)
    {
      // Create a listen socket for them to connect to.
      int i;
      int listener_sock=-1;
      for(i=32768;i<65535;i++) {
	listener_sock=create_listen_socket(i);
	if (listener_sock!=-1) break;
      }
      if (listener_sock==-1) {
	printf("FATAL: Couldn't listen to a socket to complete some tests.\n");
	exit(-1);
      }
      
      int sock=connect_to_port(student_port);
      if (sock==-1) {
	printf("FAIL: Could not connect to test acceptance of HTTP/1.0 request\n");
	return -1;
      }
      char request[1024];
      
      // Send request to fetch from us
      snprintf(request,1024,"GET /port/%d HTTP/1.0\r\n\r\n",i);
      write(sock,request,strlen(request));
      
      // Now wait a few seconds for them to fetch from us.
      int timeout=time(0)+3;
      int s=-1;
      while(time(0)<timeout) {
	s=accept_incoming(listener_sock);
	if (s>-1) {
	  // We have a connection from them
	  printf("SUCCESS: Makes outbound TCP connection on specified port.\n");
	  success++;
	  break;
	}
      }
      if (s==-1) {
	printf("FAIL: Fails to make outbound TCP connection on specified port.\n");
	printf("FAIL: Other tests suppressed due to dependence on the above.\n");
	return -1;
      }
      
      // Ok, so they have connected to us.  Let's check out their request and
      // respond to it.
      char buffer[8192];
      int count=0;
      read_from_socket(s,(unsigned char *)buffer,&count,sizeof(buffer)); 
      if (count<1) {
	printf("FAIL: Fails to write to outbound TCP connection.\n");
	close(s);
	close(sock);
	return -1;
      } else {
	printf("SUCCESS: Wrote to outbound TCP connection.\n");
	success++;
      }
      
      // use a random token to prevent students hardwiring the token
      srandom(getpid()^time(0));
      token=random();
      readToken=0;
      
      if (parseAndRespondToHTTPRequest(s,buffer,count)) {
	printf("FAIL: Did not write valid HTTP request to outbound TCP connection.\n");
	close(s);
	close(sock);
	return -1;
      } else {
	printf("SUCCESS: Wrote a valid HTTP request to outbound TCP connection.\n");
	success++;
      }
      if (readToken) {
	printf("SUCCESS: Wrote valid HTTP request to /gettoken to retrieve token.\n");
	success++;
      }
      else
	printf("FAIL: Did not write valid HTTP request to /gettoken to retrieve token.\n");
      close(s);
      
      // Now read response from client, make sure it is okay, and that it contains the token.
      count=0;
      read_from_socket(sock,(unsigned char *)buffer,&count,sizeof(buffer)); 
      if (count<1) 
	printf("FAIL: Did not write response for token challenge request.\n");
      else {
	printf("SUCCESS: Did write response for token challenge request.\n");
	success++; 
      }
      
      int response_code=getHttpReplyCode(buffer);
      int content_length=getContentLength(buffer);
      if (allowTokenFetch) {
	// If we gave the token, then expect to get it back.
	if (response_code!=200) 
	  printf("FAIL: Did not return a HTTP 200 OK response to token challenge request.\n");
	else {
	  printf("SUCCESS: Returned a HTTP 200 OK response to token challenge request.\n");
	  success++;
	}
	if (content_length<1) 
	  printf("FAIL: Did not return a valid HTTP body and/or content-length header to token challenge request.\n");
	else {
	  printf("SUCCESS: Returned a valid HTTP body and/or content-length header to token challenge request.\n");
	  success++; 
	}
      
	unsigned int returned_token=-1;
	if (content_length<count&&content_length>=0) 
	  returned_token=strtoll(&buffer[count-content_length],NULL,16);
	if (returned_token!=token) 
	  printf("FAIL: Failed to return token from token challenge request.\n");
	else {
	  printf("SUCCESS: Returned token from token challenge request.\n");
	  success++;
	}
      } else {
	// We refused to give them the token, so expect to get an error back.
	if (response_code<500||response_code>599) 
	  printf("FAIL: Did not return an HTTP error code 5xx when token fetching fails.\n");
	else {
	  printf("SUCCESS: Did return an HTTP error code 5xx when token fetching fails.\n");
	  success++;
	}
      }
      
      close(sock);
      close(listener_sock);
    }
  return 0;
}  


  int main(int argc,char **argv)
{
  if (argc<2) {
    fprintf(stderr,"usage: %s <student programme>\n",argv[0]);
    return -1;
  }

  launch_student_programme(argv[1]);
  if (student_pid<0) {
    perror("Failed to launch student programme.");
    return -1;
  }

  test_listensonport();
  test_acceptmultipleconnections();
  test_understandshttp1();
  test_contentlengthfield();
  test_outboundConnection();

  int score=success*84/25;
  printf("Passed %d of 25 tests.\n"
	 "Score for functional aspects of assignment 1 will be %02d%%.\n"
	 "Score for style (0%% -- 16%%) will be assessed manually.\n"
	 "Therefore your grade for this assignment will be in the range %02d%% -- %02d%% (%s -- %s)\n",
	 success,score,score,score+16,gradeOf(score),gradeOf(score+15));

  if (student_pid>100) {
    fprintf(stderr,"About to kill student process %d\n",(int)student_pid);
    int r=kill(student_pid,SIGKILL);
    fprintf(stderr,"Seeing how that went.\n");
    if (r) perror("failed to kill() student process.");
  } else {
    fprintf(stderr,"Successfully cleaned up student process.\n");
  }

  return 0;
}
