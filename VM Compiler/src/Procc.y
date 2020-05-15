%{
#include "Node.h"
#include <iostream>
#include <string>
extern int yylex();
extern void yyerror(const char* s);

// Global for the program
NProgram* gProgram = nullptr;

// Disable the warnings that Bison creates
#pragma warning(disable: 4065)
%}

/* You should not need to change the union */
%union {
    Node* node;
	NProgram* program;
	NData* data;
	NDecl* decl;
	NBlock* block;
	NStatement* statement;
	NNumeric* numeric;
	NExpr* expr;
	NComparison* comparison;
	std::string* string;
	int token;
}

%error-verbose

/* Tokens/Terminal symbols */
%token <token> TDATA TMAIN TLBRACE TRBRACE TSEMI TLPAREN TRPAREN
%token <token> TLBRACKET TRBRACKET TINC TDEC TEQUALS
%token <token> TADD TSUB TMUL TDIV
%token <token> TLESS TISEQUAL
%token <token> TVAR TARRAY
%token <token> TIF TELSE TWHILE
%token <token> TCOMMA TPENUP TPENDOWN TSETPOS TSETCOLOR TFWD TBACK TROT
%token <string> TINTEGER TIDENTIFIER

/* Types/non-terminal symbols */
%type <program> program
%type <data> data decls
%type <block> main block stats
%type <decl> decl
%type <statement> statement
%type <comparison> comparison
%type <expr> expr
%type <numeric> numeric

/* Operator precedence */
%left TADD TSUB
%left TMUL TDIV

%%

program		: data main 
				{ 
					$$ = new NProgram($1, $2);
					gProgram = $$;
					std::cout << "Program\n";
				}
;

data		: TDATA TLBRACE TRBRACE
				{
					$$ = new NData;
					std::cout << "Data (no decls)\n";
				}
			| TDATA TLBRACE decls TRBRACE
				{
					$$ = $3;
					std::cout << "Data\n";
				}

;

decls		: decl 
				{
					$$ = new NData;
					$$->AddDecl($1);
					std::cout << "Single decl\n";
				}
			| decls decl
				{
					$1->AddDecl($2);
					std::cout << "Multiple decls\n";
				}
;

decl		: TVAR TIDENTIFIER TSEMI
				{
					$$ = new NVarDecl(*($2));
					std::cout << "Var declaration " << *($2) << '\n';
				}
			| TARRAY TIDENTIFIER TLBRACKET numeric TRBRACKET TSEMI
				{
					$$ = new NArrayDecl(*($2), $4);
					std::cout << "Array declaration " << *($2) << '\n';
				}
;

main		: TMAIN TLBRACE TRBRACE
				{
					$$ = new NBlock;
					std::cout << "Main (no stmts)\n";
				}
			| TMAIN block
				{
					$$ = $2;
					std::cout << "Main (stmts)\n";
				}
;

block		: TLBRACE stats TRBRACE
				{
					$$ = $2;
					std::cout << "block\n";
				}
;

stats		: statement
				{
					$$ = new NBlock;
					$$->AddStatement($1);
					std::cout << "single statement\n";
				}
			| stats statement
				{
					$1->AddStatement($2);
					std::cout << "multiple statements\n";
				}
;

statement	: TIDENTIFIER TEQUALS expr TSEMI
				{
					$$ = new NAssignVarStmt(*($1), $3);
					std::cout << "statement for assignment\n";
				}
			| TIDENTIFIER TLBRACKET expr TRBRACKET TEQUALS expr TSEMI
				{
					$$ = new NAssignArrayStmt(*($1), $3, $6);
					std::cout << "statement for array index assignment\n";
				}
			| TINC TIDENTIFIER TSEMI
				{
					$$ = new NIncStmt(*($2));
					std::cout << "increment statement\n";
				}
			| TDEC TIDENTIFIER TSEMI 
				{
					$$ = new NDecStmt(*($2));
					std::cout << "decrement statement\n";
				}
			| TIDENTIFIER TINC TSEMI
				{
					$$ = new NIncStmt(*($1));
					std::cout << "increment after statement\n";
				}
			| TIDENTIFIER TDEC TSEMI 
				{
					$$ = new NDecStmt(*($1));
					std::cout << "decrement after statement\n";
				}
			| TPENUP TLPAREN TRPAREN TSEMI
				{
					$$ = new NPenUpStmt;
					std::cout << "pen up statement\n";
				}
			| TPENDOWN TLPAREN TRPAREN TSEMI
				{
					$$ = new NPenDownStmt;
					std::cout << "pen down statement\n";
				}
			| TSETPOS TLPAREN expr TCOMMA expr TRPAREN TSEMI
				{
					$$ = new NSetPosStmt($3, $5);
					std::cout << "set position statement\n";
				}
			| TSETCOLOR TLPAREN expr TRPAREN TSEMI
				{
					$$ = new NSetColorStmt($3);
					std::cout << "set color statement\n";
				}
			| TFWD TLPAREN expr TRPAREN TSEMI
				{
					$$ = new NFwdStmt($3);
					std::cout << "forward statement\n";
				}
			| TBACK TLPAREN expr TRPAREN TSEMI
				{
					$$ = new NBackStmt($3);
					std::cout << "backward statement\n";
				}
			| TROT TLPAREN expr TRPAREN TSEMI
				{
					$$ = new NRotStmt($3);
					std::cout << "rotate statement\n";
				}
			| TIF comparison TLBRACE stats TRBRACE
				{
					$$ = new NIfStmt($2, $4, nullptr);
					std::cout << "if statement\n";
				}
			| TIF comparison TLBRACE stats TRBRACE TELSE TLBRACE stats TRBRACE
				{
					$$ = new NIfStmt($2, $4, $8);
					std::cout << "if else statement\n";
				}
			| TWHILE comparison TLBRACE stats TRBRACE
				{
					$$ = new NWhileStmt($2, $4);
					std::cout << "while loop\n";
				}
			
;

comparison	: expr TLESS expr
				{
					$$ = new NComparison($1, $2, $3);
					std::cout << "less than comparison\n";
				}
			| expr TISEQUAL expr
				{
					$$ = new NComparison($1, $2, $3);
					std::cout << "equals comparison\n";
				}
;

expr		: expr TADD expr
				{
					$$ = new NBinaryExpr($1, $2, $3);
					std::cout << "binary expression with + \n";
				}
			| expr TSUB expr
				{
					$$ = new NBinaryExpr($1, $2, $3);
					std::cout << "bin expression with - \n";
				}
			| expr TMUL expr
				{
					$$ = new NBinaryExpr($1, $2, $3);
					std::cout << "bin expression with * \n";
				}
			| expr TDIV expr
				{
					$$ = new NBinaryExpr($1, $2, $3);
					std::cout << "bin expression with / \n";
				}
			| TLPAREN expr TRPAREN
				{
					$$ = $2;
					std::cout << "expression within parenthesis\n";
				}
			| TIDENTIFIER
				{
					$$ = new NVarExpr(*($1));
					std::cout << "identifier expression\n";
				}
			| TIDENTIFIER TLBRACKET TIDENTIFIER TRBRACKET
				{
					std::cout << "array index access with identifier\n";
				}
			| TIDENTIFIER TLBRACKET expr TRBRACKET
				{
					$$ = new NArrayExpr(*($1), $3);
					std::cout << "array index access with numeric\n";
				}
			| numeric
				{
					$$ = new NNumericExpr($1);
					std::cout << "Numeric expression\n";
				}
;

numeric		: TINTEGER
				{
					$$ = new NNumeric(*($1));
					std::cout << "Numeric value of " << *($1) << '\n';
				}
;

%%
