#include <stdlib.h>

#include "http_request.h"

int header_push(http_request_t * req, http_header_t * hdr) {
	http_header_list_t * list;
	if(req)
		list = req->headers;
	if((list->tail == NULL) || (list->head == NULL)) {
		list->tail = list->head = hdr;
		return 1;
	} else {
		if(!((list->tail)->next)) {
		(list->tail)->next = hdr;
		list->tail = hdr;
		return 1;
		}
	}
	return 0;
}

void request_free(http_request_t * req) {
	free(req->headers);
	free(req);
}
