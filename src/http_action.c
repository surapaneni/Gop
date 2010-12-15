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


http_header_t * header = NULL;
static http_request_t * req = NULL;
static int init_done = 0;
static void print_current_line(void) {
	//printf("---- %s: %s ----\n",CURRENT_LINE.field,CURRENT_LINE.value);
	const http_header_t * hp;
	printf(" @@@@@ HTTP REQUEST @@@@@\n");
	hp = (req->headers)->head;
	while(hp) {
		printf("---- %s: %s ----\n",hp->field,hp->value);
		hp = hp->next;
	}
	return;
}

int on_message_complete(http_parser *_) {
	return 0;
}

int on_headers_complete(http_parser *_) {
	init_done = 0;
	print_current_line();
	return 0;
}

int on_path(http_parser *_, const char *buf, size_t len) {
	req->path = strndup(buf,len+1);
	return 0;
}

int on_url(http_parser *_, const char *buf, size_t len) {
	req->url = strndup(buf,len+1);
	return 0;
}

int on_header_field(http_parser * p, const char *buf, size_t len) {
	if ( !init_done ) {
		req = (http_request_t *)(p->data);
		req->headers = (http_header_list_t *)malloc(sizeof(http_header_list_t));
		req->headers->head = req->headers->tail = NULL;
		header = (http_header_t *)malloc(sizeof(http_header_t));
		init_done = 1;
	}
	header->value = NULL;
	header->value_len = 0;
	header->field = strndup(buf,len+1);
	header->field_len = len;
	header->next = NULL;
	return 0;
}

int on_header_value(http_parser * _, const char *buf, size_t len) {
	if(init_done) {
		header->value = strndup(buf,len+1);
		header->value_len = len;
	}

	header_push(req,header);
	request_free(req);
	init_done = 0;
	return 0;
}

int on_body(http_parser * parser, const char *buf, size_t len) {
	return printf("Body: %s\nLength: %zu\n",buf,len);
}
