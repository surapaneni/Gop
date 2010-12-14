#ifndef __GOP_HTTP_ACTION_H__
#define __GOP_HTTP_ACTION_H__

int on_header_field(http_parser *,const char *,size_t);
int on_body(http_parser *, const char *, size_t);

#endif
