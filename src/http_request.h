#ifndef __GOP_HTTP_REQUEST_H__
#define __GOP_HTTP_REQUEST_H__

#include <stddef.h>

struct http_header {
	const char *field;
	size_t field_len;
	const char *value;
	size_t value_len;
	struct http_header * next;
};

typedef struct http_header http_header_t;

struct http_header_list {
	struct http_header *head, *tail;
};

typedef struct http_header_list http_header_list_t;

struct http_request {
	http_header_list_t * headers;
	size_t nheaders;
	const char * url;
	size_t url_len;
	const char * method;
	const char * path;
	size_t path_len;
	const char * query_string;
	size_t query_string_len;
	const char * data;
};

typedef struct http_request http_request_t;

int header_push(http_request_t * req, http_header_t * hdr);
void request_free(http_request_t * req);
#endif
