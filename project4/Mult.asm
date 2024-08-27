// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
// The algorithm is based on repetitive addition.

@R0
D=M
@n
M=D
@R2
M=0

(ADD)
// n==0
@n
D=M
@END
D;JEQ

// R2 = R2 + R1
@R1
D=M
@R2
M=D+M
@n
M=M-1

// loop back
@ADD
0;JMP

(END)
0;JMP

