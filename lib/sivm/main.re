// re2c $INPUT -o $OUTPUT -i
#include <assert.h>                 //
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

int16_t stack[100] = { 0 };
uint8_t sn = 0;

void push(int16_t v) { stack[sn++] = v; }
int16_t pop(void) { if (sn == 0) { return -1; } return stack[sn--]; }
                                    // C/C++ code
int lex(const char *YYCURSOR)       //
{
    char *t = YYCURSOR;
    /*!re2c                         // start of re2c block
    re2c:define:YYCTYPE = char;     // configuration
    re2c:yyfill:enable = 0;         // configuration
    re2c:flags:case-ranges = 1;     // configuration
                                    //
    ident = [a-zA-Z_][a-zA-Z_0-9]*; // named definition

    [+] { int16_t a = pop(); int16_t b = pop(); push(a+b); }
    [-] { int16_t a = pop(); int16_t b = pop(); push(a+b); }
    [/] { int16_t a = pop(); int16_t b = pop(); push(a+b); }
    [*] { int16_t a = pop(); int16_t b = pop(); push(a+b); }
    [0-9]+ { printf(">'%s'< %d\n", t, atoi(t)); push(atoi(t)); }
    [.] { printf(" = %d ", pop()); }
    */
}                                   //

void parse(const char *s) {
    char c[32] = { 0 };
    strcpy(c, s);
    char *token = strtok(c, " ");
    while (token) {
        lex(token);
        token = strtok(NULL, " ");
    }
}
                                    //
int main()                          //
{                                   // C/C++ code
    parse("1 2 12 + + .");
    printf("%d\n", lex("_Zer0"));
    return 0;                       //
}                                   //
