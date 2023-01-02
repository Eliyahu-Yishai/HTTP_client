#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

void print_erorr(char **, int);
char* order_to_line(char * , int ,char ** );
char** build_request(int ,char ** , int*, int *);
int n_in_str(char *, int * , int * , int *);

int size_arg = 0;  //Global integer

#define RESPONSE_LEN 5000

int main(int argc , char **argv ) {
    char *request = NULL , **res = NULL;
    int len_request = 0 , port = 0 , lenread = 0;
    char *host = NULL;
    unsigned char response[RESPONSE_LEN] = "";
    int fd_sock = 0;
    struct sockaddr_in peeraddr;
    struct hostent *hp;
    res = build_request(argc, argv  ,&port, &len_request);
    request =  res[0];
    host = res[1] ;
    printf("\nhost_t = %s\n" , host);
    printf("HTTP request =\n%s\nLEN = %ld\n" , request , strlen(request));
    if ((hp = gethostbyname(host)) == NULL) {
        herror("gethostbyname");
        free(request);
        free(host);
        free(res);
        exit(1);
    }
    memset(&peeraddr,0,sizeof(peeraddr));
    peeraddr.sin_family = AF_INET;
    peeraddr.sin_port = htons(port);
    peeraddr.sin_addr.s_addr = ((struct in_addr*)hp->h_addr_list[0])->s_addr;
    memcpy(&peeraddr.sin_addr.s_addr,hp->h_addr,hp->h_length);

    if ((fd_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket");
        free(request);
        free(host);
        free(res);
        exit(1);
    }

    // Connect to server
    if (connect(fd_sock, (struct sockaddr*)&peeraddr, sizeof(peeraddr) ) < 0) {
        herror("Connection");
        free(request);
        free(host);
        free(res);
        exit(1);
    }

    if ( write(fd_sock , request , len_request) < 0) {
        perror("write");
        free(request);
        free(host);
        free(res);
        exit(1);
    }

    int nread = 1;

    /* Get the response from the server */
    while(nread != 0){
        if ((nread = read(fd_sock, response, RESPONSE_LEN - 1)) < 0) {
            perror("Receive failed");
            free(request);
            free(host);
            free(res);
            exit(1);
        }
        if(!nread) break;
        lenread += nread;
        printf("%s\n" , response);
    }

    printf("\n Total received response bytes: %d\n" , lenread);
    free(request);
    free(host);
    free(res);
}



//tofree is arr of *char taht need to free
void print_erorr(char ** tofree, int size){
    if(tofree[0] != NULL) {
        printf("\nfailed:%s\n", tofree[0]);
        free(tofree[0]);
    }
    for(int i =  1 ; i < size ; i ++)
        free(tofree[i]);
    free(tofree);
    exit(1);
}



int n_in_str(char * str , int * i , int * n , int *num_c) {
    int  k = 0;
    char * num = (char *) malloc(sizeof (char)* size_arg);
    if(num == NULL)
        return 1;
    while (isdigit(str[*i]) > 0 && str[*i] != ' ' && str[*i] != '\0') {
        num[k] = str[*i];
        k++;
        *i = *i + 1;
        if(num_c != NULL)
            *num_c = *num_c + 1;
    }
    num[k] = '\0';
    *n = atoi(num);
    free(num);
    return 0;
}



char* order_to_line(char* argv_line , int argc ,  char ** argv) {
    int count = 0;  // count chars in argv
    //char * argv_line = NULL;
    int n  = 0;
    int j , i ;

    for( j = 1 ; j < argc  ; j ++)
        for (i = 0; i < strlen(argv[j]); i++)
            count++;
    char *line = (char*) malloc(sizeof (char)*(count + argc));
    if(line == NULL){
        printf("the malloc is failed");
        exit(1);
    }

    for( j = 1 ; j < argc ; j ++ , n++) {
        for(i = 0 ; i < strlen(argv[j]) ; i ++ , n ++)
            line[n] = argv[j][i];
        if( n != count + argc - 2 )
            line[n] = ' ';
    }
    size_arg = count + argc - 2;
    line[count + argc - 1] = '\0';
    argv_line = line;
    return argv_line;
}



char ** build_request(int argc , char **argv ,int *port ,int * len_request){//todo
    if(argc == 1){
        printf("argv = NULL");
        exit(1);
    }
    char * argv_line = NULL ,**res = NULL,  *str_r = NULL , *host = NULL , *str_p = NULL , *path = NULL , **hold_pointers = NULL;
    int i = 0, j = 0,  count_r = 0 , lenP = 0, count_p = 0, n, count = 0 , count_arg = 0 , count2 = 0 ,has_https = 0 , num_c = 0 , num = 0;
    const int size = 5;
    unsigned int k = 0;
    char help_h[] = "http://";
    hold_pointers = (char**) malloc(sizeof (char*)*size);
    if(hold_pointers == NULL){
        printf("The alocated failed");
        exit(1);
    }

    argv_line = order_to_line(argv_line , argc , argv);

    hold_pointers[0] = argv_line;
    path = (char*) malloc(sizeof (char )* size_arg);
    if(path == NULL)
        print_erorr(hold_pointers , size);
    path[0] =  ' ';
    hold_pointers[1] = path;
    str_r = (char *) malloc(sizeof(char)* size_arg);
    if(str_r == NULL)
        print_erorr(hold_pointers , size);
    str_r[0] = ' ';
    hold_pointers[2] = str_r;
    str_p = (char *) malloc(sizeof(char)* size_arg);
    if(str_p == NULL)
        print_erorr(hold_pointers , size);
    str_p[0] = ' ';
    hold_pointers[3] = str_p;
    host = (char*) malloc(sizeof (char)* size_arg);
    if(host == NULL)
        print_erorr(hold_pointers , size);
    host[0] = ' ';
    hold_pointers[4] = host;
    *port = 80;
    i = 0;
    n = 0;
    count = 0;
    int count_host = 0;
    while( i < size_arg ) {
        if (argv_line[i] == ' ') {
            i++;
            continue;
        }
        if (argv_line[i] == '-') {
            i++;
            if (argv_line[i] == 'p') {
                if (count_p > 1) print_erorr(hold_pointers, size);
                count_p = 1;
                i++;
                if (argv_line[i] != ' ') print_erorr(hold_pointers, size);
                i++;
                if (isdigit(argv_line[i]) <= 0) print_erorr(hold_pointers, size);
                if (n_in_str(argv_line, &i, &n , &num_c)) print_erorr(hold_pointers, size);

                if (argv_line[i] != ' ') print_erorr(hold_pointers, size);
                i++;
                k = 0;
                while (argv_line[i] != ' ' && argv_line[i] != '\0') {
                    if (k < n) {
                        str_p[k] = argv_line[i];
                        k++;
                    }
                    i++;
                    if (k > n)
                        break;
                }
                lenP = (int)k;
                if (n < k) print_erorr(hold_pointers, size);
                str_p[k] = '\0';
            } else if (argv_line[i] == 'r') {
                if (count_r > 0) print_erorr(hold_pointers, size);
                count_r = 1;
                i++;
                if (argv_line[i] != ' ') print_erorr(hold_pointers, size);
                i++;
                if (n_in_str(argv_line, &i, &n , NULL)) print_erorr(hold_pointers, size);
                if (n == 0) {
                    i++;
                    continue;
                }
                if (argv_line[i] != ' ') print_erorr(hold_pointers, size);
                i++;
                if (argv_line[i] == '/' || argv_line[i] == '=') print_erorr(hold_pointers, size);
                k = 0;
                while (argv_line[i] != '\0' && count_arg <= n) {
                    if (argv_line[i] == ' ' && count_arg == n)
                        break;
                    if (argv_line[i] == '=') {
                        if (i > size_arg && (argv_line[i - 1] == ' ' || argv_line[i + 1] == ' '))
                            print_erorr(hold_pointers, size);
                        count_arg++;
                    }
                    if (argv_line[i] == ' ')
                        str_r[k] = '&';
                    else
                        str_r[k] = argv_line[i];
                    i++;
                    k++;
                    if (count_arg > n) print_erorr(hold_pointers, size);
                }
                if (n < count_arg) print_erorr(hold_pointers, size);
                str_r[k] = '\0';
            } else print_erorr(hold_pointers, size);
        }else if (argv_line[i] == 'h') {
            if ( i != 0 && argv_line[i - 1] != ' ' )
                print_erorr(hold_pointers, size);
            k = 0;
            count = 0;
            while (k < 7) {
                if (argv_line[i] != help_h[k]) {
                    print_erorr(hold_pointers, size);
                }
                k++;
                i++;
            }
            k = 0;
            while ((i == 0) || (argv_line[i] != ' ' && argv_line[i] != '\0')) {
                if (argv_line[i] == '/') break;
                if (argv_line[i] == ':') break;
                host[k] = argv_line[i];
                i++;
                k++;
                count_host++;
            }
            host[k] = '\0';
            if (argv_line[i] == ':') {
                i++;
                if (n_in_str(argv_line, &i, &n , NULL)) print_erorr(hold_pointers, size);
                if(n != 80)
                    *port = n;
                if (n > pow(2, 16))
                    print_erorr(hold_pointers, size);
            }
            if (argv_line[i] == '/') {
                k = 0;
                while (argv_line[i] != ' ' && argv_line[i] != '\0') {
                    path[k] = argv_line[i];
                    i++;
                    k++;
                }
                path[k] = '\0';
            }
            else { path[0] = '/'; path[1] = '\0';}
            has_https = 1;
        } else if (argv_line[i] == '\0') {
            break;
        } else
            print_erorr(hold_pointers, size);

    }
    if(has_https == 0)
        print_erorr(hold_pointers , size);
    char * request = (char*) malloc(sizeof (char)*(size_arg + 100));
    num = 100 + size_arg;
    res = (char**)malloc(sizeof (char*)*2);
    if(res == NULL)  print_erorr(hold_pointers , size);
    res[0] = (char*) malloc(sizeof(char)*num);
    if(res[0] == NULL){
        free(res);
        print_erorr(hold_pointers , size);
    }
    res[1] = (char*)malloc(sizeof(char)*(1 + count_host));
    if(res[1] == NULL){
        free(res[0]);
        free(res);
        print_erorr(hold_pointers , size);
    }
    k = 0;
    if(count_p == 1 && (count_r == 1 || count_r == 0)){
        strcat(request , "POST ");
        k+= strlen("POST ");
    }
    else if(count_r == 1 && count_p == 0 || (count_r == 0 && count_p == 0) ) {
        strcat(request, "GET ");
        k += strlen("GET ");
    }

    strcat(request , path);

    k+= strlen(path);
    if(count_arg > 0){
        strcat(request , "?");
        strcat(request , str_r);
        k += strlen("?") + strlen(str_r);
    }
    if(count_r == 0 && strlen(path) == 0 && strlen(str_p) == 0)
        strcat(request, "HTTP/1.0\r\nHost: ");
    else {
        strcat(request, " HTTP/1.0\r\nHost: ");
        k++;
    }
    strcat(request , host);
    k += strlen("HTTP/1.0\r\nHost: ") + strlen(host);

    if(count_p == 1 && (count_r == 1 || count_r == 0)) {
        char *hold_len = (char*)malloc(sizeof(char)*num_c);
        sprintf(hold_len ,"%d" , lenP);
        hold_len[num_c] = '\0';
        strcat(request, "\r\nContent-length:");
        strcat(request , hold_len);
        k += strlen("\r\nContent-length:") + num_c;
        k += 1;
        free(hold_len);
    }

    strcat(request, "\r\n\r\n");
    k += strlen("\r\n\r\n ");

    request[k] = '\0';
    *len_request = (int)k ;
    for(i = 0 ; i < strlen(request) ; i ++)
        res[0][i] = request[i];

    for( i = 0 ; i < strlen(host) ; i ++)
        res[1][i] = host[i];
    res[1][count_host] = '\0';

    for( i =  0 ; i < size  ; i ++)
        free(hold_pointers[i]);
    free(hold_pointers);
    free(request);
    return res;
}
