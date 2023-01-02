//--@Author: Eliyahu Yishaiovizh 

******HTTP client**********

//--Description
This class accept argument in main ,URL and arguments -p , -r . build request by protocol http and connect to server and print the web.

//--Arguments
 You need to type this the format:
[–p n <text>] [–r n <pr1=value1 pr2=value2 …>] <URL>

//--files
client.c  

//--assumes
I assumed that if they entered a URL without '\' after the host
 (in case there is no path) I add '\' to the end and the request is correct.


//--how to compile
gcc client.c -o client
./client
