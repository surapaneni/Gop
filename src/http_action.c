#include <stdio.h>

#include "http_parser.h"

int on_header_field(http_parser * parser, const char *buf, size_t len) {
	//return printf("Header Field: %s\nLength: %zu\n",buf,len);	
	return 0;
}

int on_body(http_parser * parser, const char *buf, size_t len) {
	return printf("Body: %s\nLength: %zu\n",buf,len);
}
