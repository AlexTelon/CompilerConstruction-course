%{

#include <stdlib.h>
#include <string.h>


#include "scanner.hh"
// This is where you put #include directives as needed for later labs.
// include "ast.hh", "parser.hh" in that order

int column = 0;

extern YYLTYPE yylloc; // Used for position information, see below.

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
   DCOMMENT \{.*\} // works now, is implemented
   IDENTIFIER [a-zA-Z_][a-zA-Z_0-9]* // works but needs implementation
   DINTEGER [0-9]+ // is overridden by reals, implemented needed
   REALS [0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?$ // good, but overrides integer, nees implementation
   WHITESPACE [ \t\r\n\f]+ // no idea, should be good.

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
                            
                         }
if                       {
                            
                         }
do                       {
                            
                         }
or                       {
                            
                         }
var                      {
                            
                         }
end                      {
                            
                         }
and                      {
                            
                         }
div                      {
                            
                         }
mod                      {
                            
                         }
not                      {
                            
                         }
then                     {
                            
                         }
else                     {
                            
                         }
const                    {
                            
                         }
array                    {
                            
                         }
begin                    {
                            
                         }
while                    {
                            
                         }
elsif                    {
                            
                         }
return                   {
                            
                         }
program                  {
                            
                         }
function                 {
                            
                         }
procedure                {
                            
                         }


{DCOMMENT}               {
                            column += yyleng;
                         }


\/\/.*$                  column = 0; /* Skip single-line comment */
"/\*"                    {
                            column += yyleng;
                            BEGIN(c_comment);
                         }

[\'][.*^\n][^(\'\')\']   {
			    yylval.pool_p = sym_tab->pool_install(sym_tab->fix_string(yytext));
			    column += yyleng;
                         }




{IDENTIFIER}             {
                            yylval.pool_p = sym_tab->pool_install(yytext);
                            sym_tab->pool_lookup(yylval.pool_p);
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_IDENT;
                         }

{DINTEGER}               {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
			    printf("INTEGER DUUUDE\n");
                            return T_INTNUM;
                         }
{REALS}                  {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
			    printf("DUUUDE ITS SO REAL\n");
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
