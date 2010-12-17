#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "http_request.h"

int header_push(http_request_t * req, http_header_t * hdr) {
	return 0; //http_header_list_t * list;
}

void request_push_field(http_header_list_t * list, const char * field, size_t len) {
	http_header_t * hdr;

	hdr = (http_header_t *)calloc(1,sizeof(http_header_t));

	if(!hdr)
		return;

	hdr->field = strndup(field,len);
	hdr->field_len = len;
	hdr->value = NULL;
	hdr->value_len = 0;

	if((list->head == NULL) || (list->tail == NULL)) {
		list->head = hdr;
	} else {
		if(list->tail) {
			http_header_t * h = (list->tail);
			h->next = hdr;
		}
	}

	list->tail = hdr;
}

void request_push_value(http_header_list_t * list, const char * value, size_t len) {
	if(!(list->tail))
		return;
	
	http_header_t * hdr = (list->tail);
	hdr->value = strndup(value, len);
	hdr->value_len = len;
}

void request_free(http_request_t * request) {
	http_header_list_t * list = request->headers;
	http_header_t *tmp,*hdr = list->head;
	while(hdr) {
		tmp = hdr->next;
		free((void *)(hdr->field));
		free((void *)(hdr->value));
		free(hdr);
		hdr = tmp;
	}

	free(list);
	request->headers = NULL;
	free((void *)(request->path));
	free((void *)(request->url));
}
