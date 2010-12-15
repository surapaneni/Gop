#ifndef __GOP_HTTP_ACTION_H__
#define __GOP_HTTP_ACTION_H__

struct header_line {
	char *field;
	size_t field_len;
	char *value;
	size_t value_len;
};

int on_path(http_parser *, const char *, size_t);
int on_url(http_parser *, const char *, size_t);
int on_header_field(http_parser *,const char *,size_t);
int on_header_value(http_parser *,const char *,size_t);
int on_body(http_parser *, const char *, size_t);
int on_headers_complete(http_parser *);
int on_message_complete(http_parser *);
#endif
