%option noyywrap
%option reentrant
%option never-interactive
%option 8bit
%%
"+" { int16_t a = pop(); int16_t b = pop(); push(a+b); }
"-" { int16_t a = pop(); int16_t b = pop(); push(a+b); }
"/" { int16_t a = pop(); int16_t b = pop(); push(a+b); }
"*" { int16_t a = pop(); int16_t b = pop(); push(a+b); }
[0-9]+ { push(atoi(yytext)); }
"." { printf("%d ", pop()); }
%%

int main(int argc, char **argv)
{
  return yylex();
}
