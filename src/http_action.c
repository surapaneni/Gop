#include <stdio.h>
/*
//	Some parts of the code in this file are taken 
//	verbatim from https://github.com/ry/http-parser
//	Example Gist
//	
*/

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "http_request.h"
#include "http_parser.h"
#include "http_action.h"

#define HL (request.headers)

http_header_t * header = NULL;
//static http_request_t * req = NULL;
extern http_request_t request;
static void print_current_request(void) {
	http_header_list_t * list = request.headers;
	http_header_t * h = (list->head);
	while(h) {
		printf("---- %s: %s ----\n",h->field,h->value);
		h = h->next;
	}
	return;
}

int on_message_complete(http_parser *_) {
	return 0;
}

int on_headers_complete(http_parser *_) {
	print_current_request();
	request_free(&request);
	return 0;
}

int on_path(http_parser *_, const char *buf, size_t len) {
	request.path = strndup(buf,len+1);
	return 0;
}

int on_url(http_parser *_, const char *buf, size_t len) {
	request.url = strndup(buf,len+1);
	return 0;
}

int on_header_field(http_parser * p, const char *buf, size_t len) {
	if(!request.headers) {
		request.headers = (http_header_list_t *)calloc(1,sizeof(http_header_list_t));
#if DEBUG
		printf("New header list inited at %p\n",request.headers);
#endif
	}	

	request_push_field(request.headers, buf, len); 
	return 0;
}

int on_header_value(http_parser * _, const char *buf, size_t len) {
	request_push_value(request.headers, buf, len);
	return 0;
}

int on_body(http_parser * parser, const char *buf, size_t len) {
	return 0;
}
