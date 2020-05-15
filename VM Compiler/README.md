Alexander Vilesov
Email: asvilesov@gmail.com

Project: ProCC Compiler
Dependencies: Windows - download 32 bit version of Cygwin and install flex and bison. Linux - sudo apt-get flex bison

Description: This program compiles high level instructions in a madeup language 'ProCC' into assembly level language for a virtual machine drawing application in another project.
Uses Bison as the lexicon and grammer parser. Creates an AST abstract syntaxt tree. Then allocates registers to virtual registers using Linear Scan algorithm. 

Instructions:
1) cd bin
2) cmake ..
3) make
4) cd ../bin
5) ./main {.pcc} {flag}*

Example:
If just a string of a .pcc file is provided, it will only generate a AST.
If a {.pcc} and {flag} is provided extra files will be generated.
 'ast' flag generates ast.txt file. 
 'emit' flag generates emit.txt file which is the compiled program with virtual registers.
 'reg' flag generates emit.txt file which is the compiled program with allocated registers and reg.txt file with register allocation table.

1) ./main ../input/star.pcc reg
2) cat ast.txt

Program
--Data
----VarDecl: i
----VarDecl: sides
----VarDecl: starSize
----ArrayDecl: myarr
------Numeric: 5
--Block
----AssignVarStmt: sides
------NumericExpr
--------Numeric: 5
----AssignVarStmt: i
------NumericExpr
--------Numeric: 0
----AssignVarStmt: starSize
------NumericExpr
--------Numeric: 100
----SetPosStmt
------NumericExpr
--------Numeric: 110
------NumericExpr
--------Numeric: 105
----PenDownStmt
----WhileStmt
------Comparison: <
--------VarExpr: i
--------VarExpr: sides
------Block
--------SetColorStmt
----------BinaryExpr: +
------------VarExpr: i
------------NumericExpr
--------------Numeric: 1
--------FwdStmt
----------VarExpr: starSize
--------RotStmt
----------NumericExpr
------------Numeric: 144
--------IncStmt: i
----PenUpStmt
----BackStmt
------NumericExpr
--------Numeric: 0

3) cat emit.txt
push r0
push r0
push r0
push r0
push r0
push r0
push r0
push r0
movi r1,5
storei 1,r1
movi r1,0
storei 0,r1
movi r1,100
storei 2,r1
movi r1,110
movi r2,105
mov tx,r1
mov ty,r2
pendown
loadi r1,0
loadi r2,1
cmplt r1,r2
movi r1,37
jnt r1
loadi r1,0
movi r2,1
add r1,r1,r2
mov tc,r1
loadi r1,2
fwd r1
movi r1,144
add tr,tr,r1
loadi r1,0
inc r1
storei 0,r1
movi r1,19
jmp r1
penup
movi r1,0
back r1
exit

4) cat reg.txt
INTERVALS:
%0:8,9
%1:10,11
%2:12,13
%3:14,16
%4:15,17
%5:19,21
%6:20,21
%7:22,23
%8:24,26
%9:25,26
%10:26,27
%11:28,29
%12:30,31
%13:32,34
%14:35,36
%15:38,39
ALLOCATION:
%0:r1
%1:r1
%2:r1
%3:r1
%4:r2
%5:r1
%6:r2
%7:r1
%8:r1
%9:r2
%10:r1
%11:r1
%12:r1
%13:r1
%14:r1
%15:r1
