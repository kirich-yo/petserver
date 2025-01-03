#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "logger/logger.h"

#define DIE(s...) {\
	Pet_FmtLog(logger, PL_FATAL, s);\
	exit(1);\
}

int server_socket;
FILE * index_html;
PetLogger * logger;

void close_server_socket(void) {
	close(server_socket);
}

void close_index_html(void) {
	if (index_html) fclose(index_html);
}

void close_logger(void) {
	if (logger) Pet_FreeLogger(logger);
}

void ctrl_c_handler(int sig) {
	exit(0);
}

int main(int argc, char ** argv) {
	atexit(close_index_html);
	atexit(close_server_socket);
	atexit(close_logger);
	signal(SIGINT, ctrl_c_handler);

	logger = Pet_NewStdoutLogger(PL_DEBUG);
	logger->log_formatter = Pet_PrettyLogFormatter;

	Pet_Log(logger, PL_DEBUG, "Test");
	Pet_Log(logger, PL_INFO, "Test");
	Pet_Log(logger, PL_WARN, "Test");
	Pet_Log(logger, PL_ERROR, "Test");
	Pet_Log(logger, PL_FATAL, "Test");

	if (argc < 2) {
		fprintf(stderr, "PetServer is a web-server pet project.\n\nUsage:\n  %s [port]\n  Example: %s 8080\n\nUse \"%s [command] --help\" for more information about a command.\n\n", argv[0], argv[0], argv[0]);
		exit(1);
	}

	in_port_t server_port = atoi(argv[1]);

	index_html = fopen("srv/index.html", "r");
	if (!index_html) 
		DIE("fopen(\"srv/index.html\") failed: %s", strerror(errno));

	struct sockaddr_in server_addr;

	server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server_socket < 0)
		DIE("socket() failed: %s", strerror(errno));

	const int optenable = 1;
	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &optenable, sizeof(int)) < 0)
		DIE("setsockopt(SO_REUSEADDR) failed: %s", strerror(errno));

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(server_port);

	if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		DIE("bind() failed: %s", strerror(errno));
	}

	//Pet_FmtLog(logger, PL_INFO, "Server running at \033[93mhttp://127.0.0.1:\033[33m%d", server_port);
	Pet_FmtLog(logger, PL_INFO, "Server running at http://127.0.0.1:%d", server_port);
	for (;;) {
		if (listen(server_socket, SOMAXCONN) < 0) {
			close(server_socket);
			DIE("listen() failed: %s", strerror(errno));
		}
	
		int client_socket;
		struct sockaddr_in client_addr;
		socklen_t client_addr_len = sizeof(client_addr);
		memset(&client_addr, 0, sizeof(client_addr));
	
		if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len)) < 0) {
			DIE("accept() failed: %s", strerror(errno));
		}
	
		char client_addr_str[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &client_addr, client_addr_str, INET_ADDRSTRLEN);
		//Pet_FmtLog(logger, PL_INFO, "New request from \033[93m%s", client_addr_str);
		Pet_FmtLog(logger, PL_INFO, "New request from %s", client_addr_str);
		char request_buffer[512];
		ssize_t http_request_length;
		do {
			if ((http_request_length = recv(client_socket, request_buffer, 512, 0)) < 0) {
				DIE("recv() failed: %s\n", strerror(errno));
			}
	
			printf("%.*s", http_request_length, request_buffer);
		} while (http_request_length == 512);
		printf("\n");
	
		fseek(index_html, 0, SEEK_END);
		long index_html_size = ftell(index_html) + 1;
		fseek(index_html, 0, SEEK_SET);
	
		char * index_html_data = (char *)malloc(index_html_size);
		fread(index_html_data, sizeof(char), index_html_size, index_html);
		fseek(index_html, 0, SEEK_SET);
	
		const char * http_response_header = "HTTP/1.1 200 OK\r\n"
			"Content-Type: text/html; charset=UTF-8\r\n"
			"\r\n";
	
		send(client_socket, http_response_header, strlen(http_response_header), 0);
		send(client_socket, index_html_data, index_html_size, 0);

		free(index_html_data);	
		close(client_socket);
	}
	return 0;
}
