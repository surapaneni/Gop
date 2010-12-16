#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "event.h"
#include "http_parser.h"
#include "http_request.h"
#include "http_action.h"

http_request_t request;

int handle_request_response(int efd, int fd) {
	http_parser_settings settings;
	char request_buf[HTTP_MAX_HEADER_SIZE];
	int rv;

	memset(&settings, 0, sizeof(http_parser_settings));
	settings.on_header_field = on_header_field;
	settings.on_header_value = on_header_value;
	//settings.on_url = on_url;
	//settings.on_path = on_path;
	settings.on_headers_complete = on_headers_complete;
	settings.on_message_complete = on_message_complete;
	
	http_parser * parser = (http_parser *)malloc(sizeof(http_parser));
	if(!parser)
		return 0;
	parser->data = (http_request_t *) &request;	
	http_parser_init(parser, HTTP_REQUEST);

	rv = read(fd,request_buf,HTTP_MAX_HEADER_SIZE);

	if(rv < 0) {
		if ( errno == EPIPE ) {
			event_deregister(efd,fd);
			close(fd);
		}
	}

	http_parser_execute(parser, &settings, request_buf, rv);

	char reply[] = "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Length: 14\r\n\r\nPrudhvi Krishna Surapaneni";
	rv = write(fd,reply,strlen(reply));
	free(parser);
	return 1;
}

