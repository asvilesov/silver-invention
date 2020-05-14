Alexander Vilesov
Email: asvilesov@gmail.com

Project: Virtual Machine for a drawing application

Description: The program accepts a .psm file (essentially compiled version) which it will first parse into relevant ops. 
Then it will output a output.bmp file of what was drawn. A log.txt file which outputs the state of the registers and flags in the VM at each progam counter step. A Stack.txt file which outputs the contents of the stack at each pc step if it was used.
Example types of register ops are movi, add, mov, div, inc, dec, mul, jmp, cmpeq, jt, jnt... and stack ops are push, pop, load, store, loadi, loadsc... and registers like r0-r9, program counter, stack couter, excpetion code...
Some example of instructions for a possible .psm program are in the 'input' folder.
The template magic happens in 'parser.hpp' ;)

Instructions:
1) cd bin
2) cmake ..
3) make
4) cd ../bin
5) ./main {args}

For example:
1) ./main ../input/draw03.psm
2) cat log.txt

********************
r0=0
r1=0
r2=0
r3=0
r4=0
r5=0
r6=0
r7=0
pc=0
sc=0
ex=0
tx=0
ty=0
tr=0
tc=0
exit=0
test=0
pen=0
Executing: movi
********************
r0=0
r1=0
r2=0
r3=0
r4=0
r5=0
r6=0
r7=0
pc=1
sc=0
ex=0
tx=110
ty=0
tr=0
tc=0
exit=0
test=0
pen=0
Executing: movi
********************
r0=0
r1=0
r2=0
r3=0
r4=0
r5=0
r6=0
r7=0
pc=2
sc=0
ex=0
tx=110
ty=105
tr=0
tc=0
exit=0
test=0
pen=0
Executing: movi
********************
r0=0
r1=0
r2=0
r3=0
r4=0
r5=0
r6=0
r7=0
pc=3
sc=0
ex=0
tx=110
ty=105
tr=0
tc=1
exit=0
test=0
pen=0
Executing: pendown
********************
r0=0
r1=0
r2=0
r3=0
r4=0
r5=0
r6=0
r7=0
pc=4
sc=0
ex=0
tx=110
ty=105
tr=0
tc=1
exit=0
test=0
pen=1
Executing: mov
********************
r0=0
r1=0
r2=0
r3=0
r4=0
r5=0
r6=0
r7=0
pc=5
sc=0
ex=0
tx=110
ty=105
tr=0
tc=1
exit=0
test=0
pen=1
Executing: movi
********************
r0=0
r1=0
r2=0
r3=0
r4=0
r5=0
r6=5
r7=0
pc=6
sc=0
ex=0
tx=110
ty=105
tr=0
tc=1
exit=0
test=0
pen=1
Executing: movi
********************
r0=0
r1=100
r2=0
r3=0
r4=0
r5=0
r6=5
r7=0
pc=7
sc=0
ex=0
tx=110
ty=105
tr=0
tc=1
exit=0
test=0
pen=1
Executing: movi
********************
r0=0
r1=100
r2=144
r3=0
r4=0
r5=0
r6=5
r7=0
pc=8
sc=0
ex=0
tx=110
ty=105
tr=0
tc=1
exit=0
test=0
pen=1
Executing: fwd
********************
r0=0
r1=100
r2=144
r3=0
r4=0
r5=0
r6=5
r7=0
pc=9
sc=0
ex=0
tx=210
ty=105
tr=0
tc=1
exit=0
test=0
pen=1
Executing: add
********************
r0=0
r1=100
r2=144
r3=0
r4=0
r5=0
r6=5
r7=0
pc=10
sc=0
ex=0
tx=210
ty=105
tr=144
tc=1
exit=0
test=0
pen=1
Executing: inc
********************
r0=0
r1=100
r2=144
r3=0
r4=0
r5=0
r6=5
r7=0
pc=11
sc=0
ex=0
tx=210
ty=105
tr=144
tc=2
exit=0
test=0
pen=1
Executing: inc
********************
r0=0
r1=100
r2=144
r3=0
r4=0
r5=0
r6=5
r7=1
pc=12
sc=0
ex=0
tx=210
ty=105
tr=144
tc=2
exit=0
test=0
pen=1
Executing: cmplt
********************

...

...

...

3) cat stack.txt

pc: 0 Stack:
pc: 1 Stack:
pc: 2 Stack:
pc: 3 Stack:
pc: 4 Stack: 17
pc: 5 Stack: 17 11
pc: 6 Stack: 17 11 5
pc: 7 Stack: 17 11 5
pc: 8 Stack: 17 11 5
pc: 9 Stack: 17 11 5
pc: 10 Stack: 17 22 5
pc: 11 Stack: 17 22 5
pc: 12 Stack: 11 22 5
pc: 13 Stack: 11 22 5
pc: 14 Stack: 11 22 5
pc: 15 Stack: 11 50 5
pc: 16 Stack: 11 50 5
pc: 17 Stack: 11 50
pc: 18 Stack: 11
pc: 19 Stack:
pc: 20 Stack:
