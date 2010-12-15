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

#include "http_parser.h"
#include "http_action.h"

#define CURRENT_LINE (header[nlines-1])
#define MAX_HEADER_LINES 80

struct header_line header[MAX_HEADER_LINES];
static int nlines = 0;

static void print_current_line(void) {
	printf("---- %s: %s ----\n",CURRENT_LINE.field,CURRENT_LINE.value);
}

int on_header_field(http_parser * _, const char *buf, size_t len) {

	nlines++;
	CURRENT_LINE.value = NULL;
	CURRENT_LINE.value_len = 0;

	CURRENT_LINE.field_len = len;
	CURRENT_LINE.field = malloc(sizeof(char)*(len+1));
	//printf("C->Field at %p\n", CURRENT_LINE->field);
	assert(CURRENT_LINE.field != NULL);
	strncpy(CURRENT_LINE.field, buf, len);

	CURRENT_LINE.field[CURRENT_LINE.field_len] = '\0';

	return 0;
}

int on_header_value(http_parser * _, const char *buf, size_t len) {
	CURRENT_LINE.value_len = len;
	CURRENT_LINE.value = malloc(sizeof(char)*(len+1));
	strncpy(CURRENT_LINE.value, buf, len);
	CURRENT_LINE.value[CURRENT_LINE.value_len] = '\0';
	
	print_current_line();
	return 0;
}

int on_body(http_parser * parser, const char *buf, size_t len) {
	return printf("Body: %s\nLength: %zu\n",buf,len);
}
