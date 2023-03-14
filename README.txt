
HTTP client
Authored by Eliyahu Yishaiovizh 

# Description

HTTP stands for Hyper Text Transfer Protocol and is used for communication among web clients and servers. HTTP has a simple stateless client/server paradigm. The web client initiates a conversation by opening a connection to the server. Once a connection is set up, the client sends an HTTP request to the server. Upon receiving the HTTP request from the client, the server sends an HTTP response back to the client. An HTTP request consists of two parts: a header and a body (In this project, the basic HTTP request from a client doesn’t contain a body). 
this project will implement the following content: An HTTP client that constructs an HTTP request based on the user’s command line input, sends the request to a Web server, receives the reply from the server, and displays the reply message on the screen. 

# Program D.B.
The main databases of this program are dynamic arrays. the program will allocate the host, text, path. finally all of the allocations will create the request, that will be allocated too as allocated array.

# Input
Command line usage: client [–p n <text>] [–r n <pr1=value1 pr2=value2 …>] <URL>. The flags and the URL can come in any order, the only limitation is that the parameters should come right after the flag –r and the text should come right after the flag –p. 

# Output
an HTTP request based on the options specified in the command line
connection to the server,  sending the HTTP request to the server, receiving  an HTTP response and displaying the response on the screen.

1.in any case of a failure in one of the system calls, it will be "perror(<sys_call>)" and exiting the program. for errors on gethostbyname it will be herror ).
2. In any case of wrong command usage, the program will print "Usage: client [-p n <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>" 
  
# Program Files
client.c

# How to compile?
compile: gcc -Wall –o client client.c
  
run: ./client 





