
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <jansson.h>

#include "sql.tab.h"
#include "yyl.h"
#include "sql.lex.h"
#include "sql-parser.h"

struct psql_state *psql_new(void)
{
	struct psql_state *st = calloc(1, sizeof(*st));
	if (!st)
		return NULL;

	if (yylex_init_extra(st, &st->scanner)) {
		free(st);
		return NULL;
	}

    st->buffer_state = NULL;

	return st;
}

void psql_free(struct psql_state *pstate)
{
	if (!pstate)
		return;

    if (pstate->buffer_state) {
        yy_delete_buffer(pstate->buffer_state, pstate->scanner);
    }

	yylex_destroy(pstate->scanner);


	free(pstate);
}

void psql_set_input(struct psql_state *pstate, FILE *in_f)
{
	yyset_in(in_f, pstate->scanner);
}

void psql_set_string_input(struct psql_state *pstate, char *in_str)
{
    if (pstate->buffer_state) {
        yy_delete_buffer(pstate->buffer_state, pstate->scanner);
    }

    pstate->buffer_state = yy_scan_string(in_str, pstate->scanner);
}

int psql_parse(struct psql_state *pstate)
{
	return yyparse(pstate->scanner, pstate);
}

