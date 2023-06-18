#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

char buffer[256], output[256];
int n;
struct in_addr addr_target;
struct sockaddr_in serv_addr, cli_addr;
struct hostent *host;

void handleClient(int newsockfd)
{
    while (1)
    {
        bzero(output, sizeof(output));
        strcpy(output, "What's your requirement? 1.DNS 2.QUERY 3.QUIT :");
        write(newsockfd, output, sizeof(output));
        bzero(buffer, sizeof(buffer));
        n = read(newsockfd, buffer, sizeof(buffer));
        if (buffer[0] == '1')
        {
            bzero(output, sizeof(output));
            strcpy(output, "Input URL address :");
            write(newsockfd, output, sizeof(output));
            bzero(buffer, sizeof(buffer));
            read(newsockfd, buffer, sizeof(buffer));
            struct addrinfo hints, *res;
            memset(&hints, 0, sizeof(hints));
            hints.ai_family = AF_INET;
            // get IP address
            int status = getaddrinfo(buffer, NULL, &hints, &res);
            char ip[INET_ADDRSTRLEN];
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
            inet_ntop(AF_INET, &(ipv4->sin_addr), ip, INET_ADDRSTRLEN);
            freeaddrinfo(res);
            bzero(output, sizeof(output));
            strcat(output, ip);
            write(newsockfd, output, sizeof(output));
        }
        else if (buffer[0] == '2')
        {
            bzero(output, sizeof(output));
            strcpy(output, "Input student ID :");
            write(newsockfd, output, sizeof(output));
            bzero(buffer, sizeof(buffer));
            read(newsockfd, buffer, sizeof(buffer));
            bzero(output, sizeof(output));
            // read txt & check
            vector<string> mails;
            ifstream ifs("query.txt", ios::in);
            string num, mail;
            while (ifs >> num >> mail)
            {
                if (num == buffer)
                {
                    strcpy(output, mail.c_str());
                    break;
                }
            }
            ifs.close();

            if (strlen(output) > 0)
            {
                write(newsockfd, output, sizeof(output));
            }
            else
            {
                strcpy(output, "No such student ID");
                write(newsockfd, output, sizeof(output));
            }
        }
        else if (buffer[0] == '3')
        {
            break;
        }
        else
        {
            bzero(output, sizeof(output));
            strcpy(output, "Wrong Command!");
            write(newsockfd, output, sizeof(output));
        }
    }
}

int main()
{
    int sockfd, newsockfd, portno, clilen;
    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }
    /* Initialize socket structure */
    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = 1234;

    serv_addr.sin_family = PF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in)) < 0)
    {
        perror("ERROR on binding");
        exit(1);
    }
    listen(sockfd, 10); // return 0 success, -1 error

    while (1)
    {
        clilen = sizeof(cli_addr);
        /* Accept actual connection from the client */
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);
        if (newsockfd < 0)
        {
            perror("ERROR on accept");
            exit(1);
        }
        handleClient(newsockfd);
    }
    return 0;
}