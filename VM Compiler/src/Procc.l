%option nounistd
%option nounput
%option never-interactive

%{
// Disable warnings
#pragma warning(disable: 4018)
#pragma warning(disable: 4996)
#pragma warning(disable: 4005)

#include <string>
#include <iostream>
#include "Node.h"
#include "parser.hpp"
extern YYSTYPE procclval;
#define SAVE_TOKEN procclval.string = new std::string(yytext, yyleng)
#define TOKEN(t) (procclval.token = t)
extern "C" int yywrap() { return 1; }
int gLineNumber = 1;

%}

%%

[ \t]					{ }

"main"					{ return TOKEN(TMAIN); }
"data"					{ return TOKEN(TDATA); }
"{"						{ return TOKEN(TLBRACE); }
"}"						{ return TOKEN(TRBRACE); }
";"						{ return TOKEN(TSEMI); }
"("						{ return TOKEN(TLPAREN); }
")"						{ return TOKEN(TRPAREN); }
"["						{ return TOKEN(TLBRACKET); }
"]"						{ return TOKEN(TRBRACKET); }
"++"					{ return TOKEN(TINC); }
"--"					{ return TOKEN(TDEC); }
"="						{ return TOKEN(TEQUALS); }
"+"						{ return TOKEN(TADD); }
"-"						{ return TOKEN(TSUB); }
"*"						{ return TOKEN(TMUL); }
"/"						{ return TOKEN(TDIV); }

"<"						{ return TOKEN(TLESS); }
"=="					{ return TOKEN(TISEQUAL); }

"var"					{ return TOKEN(TVAR); }
"array"					{ return TOKEN(TARRAY); }

"if"					{ return TOKEN(TIF); }
"else"					{ return TOKEN(TELSE); }
"while"					{ return TOKEN(TWHILE); }

","						{ return TOKEN(TCOMMA); }
"penUp"					{ return TOKEN(TPENUP); }
"penDown"				{ return TOKEN(TPENDOWN); }
"setPosition"			{ return TOKEN(TSETPOS); }
"setColor"				{ return TOKEN(TSETCOLOR); }
"forward"				{ return TOKEN(TFWD); }
"back"					{ return TOKEN(TBACK); }
"rotate"				{ return TOKEN(TROT); }

%{
/* Do not add any of your own tokens below this!!!! */
%}

"//".*\n				{ gLineNumber++; }

0|([1-9][0-9]*)			{ SAVE_TOKEN; return TINTEGER; }
[a-zA-Z_][a-zA-Z0-9_]*	{ SAVE_TOKEN; return TIDENTIFIER; }

"\n"					{ gLineNumber++; }

.						{ std::cout << "ERROR: Unknown token '" << yytext << "' on line " << gLineNumber << std::endl; yyterminate(); }

%%
