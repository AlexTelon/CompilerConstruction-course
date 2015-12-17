%{

#include <stdlib.h>
#include <string.h>


//#include "scanner.hh" // - not needed after lab2. (edited out in lab3)
#include "ast.hh"
#include "parser.hh"
// This is where you put #include directives as needed for later labs.
// include "ast.hh", "parser.hh" in that order

int column = 0;

extern YYLTYPE yylloc; // Used for position information, see below.


// used to create a very common few lines
int type_nr;
#define SPAM(type_nr)   			\
 yylloc.first_line = yylineno;			\
  yylloc.first_column = column;			\
  column += yyleng;				\
  return (type_nr);

%}

%option yylineno
%option 8bit
%option noyywrap
%option caseless
%option outfile = "scanner.cc"

/* If you want to include any flex declarations, this is where to do it. */

%x string
%x disel_comment
%x c_comment
DIGIT [0-9]

/*STRINGCONSTANT '[.^'\n]*'$ */
/*DCOMMENT \{((.|\n)^\})*\}*/
IDENTIFIER [a-zA-Z_][a-zA-Z_0-9]*
INTEGER [0-9]+ 
REALS (([0-9]*\.[0-9]+([eE][-+]?[0-9]+)?)|([0-9]*\.?[0-9]+([eE][-+]?[0-9]+)))
WHITESPACE [ \t\r\f]+

/* {DIGIT}+"."{DIGIT}*
 ID [a-z][a-z0-9]* */

/* Position information is entered into the yylloc variable, so we will be
   able to generate good error messages later in later phases. */
/* Your code should be entered below the %%. Expressions to handle the
   following: Diesel comments, Diesel string constants, Diesel
   identifiers, integers, reals, and whitespace. */

/*
regular expressions for the DISEL stuff.

*/

%%

\.                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_DOT;
                         }
;                        {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_SEMICOLON;
                         }
=                        {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_EQ;
                         }
\:                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_COLON;
                         }
\(                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_LEFTPAR;
                         }
\)                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_RIGHTPAR;
                         }
\[                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_LEFTBRACKET;
                         }
\]                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_RIGHTBRACKET;
                         }
,                        {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_COMMA;
                         }
\<                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_LESSTHAN;
                         }
\>                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_GREATERTHAN;
                         }
\+                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_ADD;
                         }
\-                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_SUB;
                         }
\*                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_MUL;
                         }
\/                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_RDIV;
                         }
":="                     {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_ASSIGN;
                         }
"<>"                     {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_NOTEQ;
                         }
of                       {
                            SPAM(T_OF);
                         }
if                       {
                            SPAM(T_IF);
                         }
do                       {
                            SPAM(T_DO);
                         }
or                       {
                            SPAM(T_OR);                            
                         }
var                      {
                            SPAM(T_VAR);                            
                         }
end                      {
                            SPAM(T_END);                                                        
                         }
and                      {
                            SPAM(T_AND);
                         }
div                      {
                            SPAM(T_IDIV);
                         }
mod                      {
                            SPAM(T_MOD);
                         }
not                      {
                            SPAM(T_NOT);
                         }
then                     {
                            SPAM(T_THEN);
                         }
else                     {
                            SPAM(T_ELSE);
                         }
const                    {
                            SPAM(T_CONST);
                         }
array                    {
                            SPAM(T_ARRAY);
                         }
begin                    {
                            SPAM(T_BEGIN);
                         }
while                    {
                            SPAM(T_WHILE);
                         }
elsif                    {
                            SPAM(T_ELSIF);
                         }
return                   {
                            SPAM(T_RETURN);
                         }
program                  {
                            SPAM(T_PROGRAM);
                         }
function                 {
                            SPAM(T_FUNCTION);
                         }
procedure                {
                            SPAM(T_PROCEDURE);
                         }
\{                       {
                            column += yyleng;
			    BEGIN(disel_comment);
                         }

\/\/.*$                  column = 0; /* Skip single-line comment */
"/\*"                    {
                            column += yyleng;
                            BEGIN(c_comment);
                         }

\n                       {
                            column = 0;
                         }

[\'] {
  // we get the whole line, make sure to break at next ' but not on ''
  // check for '' -> ' and skipp it
  // check for ' and end.
  yylloc.first_line = yylineno;
  yylloc.first_column = column;
  column += yyleng;
  yymore();
  BEGIN(string);
}



<string>
{

 "''"                    {
                            yymore();
                            column += yyleng;
                         }

 "'"                     {
                            column++;
			    yylval.pool_p = sym_tab->pool_install(sym_tab->fix_string(yytext));
			    BEGIN(INITIAL);
			    return T_STRINGCONST;
                         }
 \n                      {
                            yyerror("Newline in string");
			    column = 0;
			    BEGIN(INITIAL);
                         }
 <<EOF>>                 {
                            yyerror("Unterminated string");
                            yyterminate();
                         }
 .                       {
                            yymore();
                            column++;
                         }
}


{IDENTIFIER}             {
                            yylval.pool_p = sym_tab->pool_install(sym_tab->capitalize(yytext));
                            sym_tab->pool_lookup(yylval.pool_p);
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_IDENT;
                         }

{INTEGER}               {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            yylval.ival = atol(yytext);
                            return T_INTNUM;
                         }

{REALS}                  {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            yylval.rval = stod(yytext);
                            return T_REALNUM;
                         }

{WHITESPACE}             {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
			    BEGIN(INITIAL);
                         }



<disel_comment>
{
    "}"                 {
                            column += 1;
                            BEGIN(INITIAL);
                        }
    
    "{"                 {
                            column += 1;
			    yyerror("Suspicious comment");
                        }

    [^\n]                column++; /* Skip stuff in comments */
    \n                   column = 0;
    <<EOF>>              {
                            yyerror("Unterminated comment");
                            yyterminate();
                         }
}

<c_comment>
{
    "\*/"                {
                            column += 2;
                            BEGIN(INITIAL);
                         }
    "/\*"                {
                            column += 2;
                            yyerror("Suspicious comment");
                         }
    [^\n]                column++; /* Skip stuff in comments */
    \n                   column = 0;
    <<EOF>>              {
                            yyerror("Unterminated comment");
                            yyterminate();
                         }
}

 

<<EOF>>                  yyterminate();
.                        yyerror("Illegal character");
