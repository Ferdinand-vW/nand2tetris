// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

(LOOP)
@KBD
D=M
@KEYPRESS
M=D

@KEYPRESS
D=M
@PRINT_BLACK
D;JNE // if > 0 then key is pressed

@PRINT_WHITE
0;JMP 

(PRINT_BLACK)
@COLOR
M=-1
@PRINT
0;JMP

(PRINT_WHITE)
@COLOR
M=0

(PRINT)
@SCREEN
D=A
@addr
M=D
@row
M=0

(PRINT_LOOP)
@addr
D=M

@KBD
D=A-D

// @addr has reached out of screen memory map
@LOOP
D;JEQ

@COLOR
D=M
@addr
A=M
M=D

@addr
M=M+1
@PRINT_LOOP
0;JMP



