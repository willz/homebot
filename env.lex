%{
#include "domain.h"
%}

number ([0-9])+
word ([A-Za-z])+

%%

"(" { return OPEN_PAREN; }
")" { return CLOSE_PAREN; }
":"domain { return DOMAIN_TOK; }
at  { return AT_TOK; }
sort { return SORT_TOK; }
size { return SIZE_TOK; }
color { return COLOR_TOK; }
type { return TYPE_TOK; }
inside { return INSIDE_TOK; }
hold { return HOLD_TOK; }
plate { return PLATE_TOK; }
closed { return CLOSED_TOK; }
opened { return OPENED_TOK; }
on { return ON_TOK; }
near { return NEAR_TOK; }

":"ins { return INS_TOK; }
":"task { return TASK_TOK; }
":"cond { return COND_TOK; }
":"cons_not { return CONS_NOT_TOK; }
":"cons_notnot { return CONS_NOTNOT_TOK; }
":"info { return INFO_TOK; }


{number} { yylval.number = unsigned(atoi(yytext)); return NUMBER; }
{word}   { yylval.stringVal = strdup(yytext); return WORD; }

. { }

%%
int envwrap() { return 1; }
