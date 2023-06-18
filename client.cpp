#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <iostream>
using namespace std;

int main()
{
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int sockfd, status, n;
    char buffer[256], output[256], input[256];

    /* Create a socket point */
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }
    // server = gethostbyname(argv[1]);

    // setup server address
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = PF_INET;
    serv_addr.sin_port = htons(1234);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

    // connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in));
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in)) < 0)
    {
        perror("ERROR connecting");
        exit(1);
    }

    while (1)
    {
        bzero(output, sizeof(output));
        n = read(sockfd, output, sizeof(output));
        cout << output;
        bzero(input, sizeof(input));
        cin >> input;
        write(sockfd, input, sizeof(input));
        if (input[0] == '1')
        {
            bzero(output, sizeof(output));
            read(sockfd, output, sizeof(output));
            cout << output;
            bzero(input, sizeof(input));
            cin >> input;
            write(sockfd, input, sizeof(input));
            bzero(output, sizeof(output));
            read(sockfd, output, sizeof(output));
            cout << "Address get from domain name : " << output << endl;
        }
        else if (input[0] == '2')
        {
            bzero(output, sizeof(output));
            read(sockfd, output, sizeof(output));
            cout << output;
            bzero(input, sizeof(input));
            cin >> input;
            write(sockfd, input, sizeof(input));
            bzero(output, sizeof(output));
            read(sockfd, output, sizeof(output));
            cout << "Email get from server : " << output << endl;
        }
        else if (input[0] == '3')
        {
            close(sockfd);
            return 0;
        }
        else
        {
            bzero(output, sizeof(output));
            read(sockfd, output, sizeof(output));
            cout << output << endl;
        }
    }
    return 0;
}