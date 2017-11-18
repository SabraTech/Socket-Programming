#include "utilities.h"

using namespace std;

void sigchld_handler(int s) {
    while (waitid(-1, NULL, WNOHANG) > 0);
}

//get sockaddr, IPbv4 or IPv6
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *) sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}

send_response(int new_fd, char *response) {
    int i;
    if ((i = send(new_fd, response, strlen(response), 0)) == -1) {
        perror("send response");
    }
}

bool file_found(string file_name) {
    return true;
}

void parse_get_request(int new_fd, vector <string> request) {
    string file_format = get_file_format(request);
    int result;
    char *response;
    string file_name = request[1];

    if (!file_found(file_name)) {
        response = ERROR_MSG;
    } else {
        response = OK_MSG;
    }

    send_response(new_fd, response);

    if (file_format.compare("html") == 0) {
        // send html
    } else if (file_format.compare("txt") == 0) {
        // send txt
    } else if (file_format.compare("img") == 0) {
        // send img
    } else {
        printf("Get file type not found !");
    }
}

void parse_post_request(int new_fd, vector <string> request) {

    send_response(new_fd, OK_MSG);

    string file_format = request[4];

    if (file_format.compare("html") == 0) {
        // receive html
    } else if (file_format.compare("txt") == 0) {
        // receive txt
    } else if (file_format.compare("img") == 0) {
        // receive img
    } else {
        // error
    }
}


int main(void) {
    int sockfd, new_fd; // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connection's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                       sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }
        break;
    }

    if (p == NULL) {
        fprintf(stderr, "server: failed to bind\n");
        return 2;
    }

    freeaddrinfo(servinfo); // all done with this structure
    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }
    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    while (1) {  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }
        inet_ntop(their_addr.ss_family,
                  get_in_addr((struct sockaddr *) &their_addr),
                  s, sizeof s);
        printf("server: got connection from %s\n", s);
        if (!fork()) { // this is the child process
            close(sockfd); // child doesn't need the listener
            char data[256];
            int id = recv(new_fd, data, 255, 0);
            if (id < 0) {
                perror("recv");
                exit(1);
            }
            string str(data);
            vector <string> parsed_request = parse_request(str);
            if (parsed_request[0].compare("GET") == 0 || parsed_request[0].compare("get") == 0) {
                parse_get_request(new_fd, parsed_request);
            } else {
                parse_post_request(new_fd, parsed_request);
            }

            close(new_fd);
            exit(0);
        }
        close(new_fd);  // parent doesn't need this
    }
}
