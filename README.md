# rufcpu
A fake, software-based instruction set for experimenting with assemblers, compilers, etc.

# Goals
1. To have an instruction set elaborate enough to experiment with various aspects of translators
2. To develop translators (assemblers, compilers, etc.) for this particular instruction set
3. To develop "higher-level" languages, as necessary, for the instruction set.

# Instruction Set and 'CPU' (V0.1)

Every instruction is 16 bits wide. The first four bits always indicate the command. The remaining
bits vary depending on the instruction.

There are 16 all-purpose "registers." They are accessed via hex-code such as "r1", "R2", "rF", "rc", etc.
As you can see from the previous example, register codes are not case-sensitive.

The program counter cannot be accessed directly and always begins at 0.

The "memory" contains 2^16 (0 - 65535 inclusive) 16-bit wide addresses. Thus, you may not have more than
65536 instructions in your programs!

## The Instructions

0000 NOP [4-15] 0's

 - No Operation. Does nothing.

0001 ADD [4-7] destination register [8-11] first register [12-15] second register
0010 ADD [4-7] destination register [8-11] register to add to [12-15] 4 bit number

 - Two ways to add. The first (0001) is to specify two registers and the second
 (0010) is to specify a register and then give a 4-bit number (two's compliment).

0011 SET [4-7] destination register [8-15] 8 bit number
0100 CPY [4-7] destination register [8-11] register to copy number from [12-15] 0's
0101 JMP [4-7] register holding address to jump to [8-15] 0's
0110 LD  [4-7] destination register [8-11] register holding address to load from
0111 SV  [4-7] register with value we want to save [8-11] register holding address we want to save to
1000 BCH [4-7] register to test [8-11] register holding address we jump to if test register is not 0 [12-15] 0's
1001 NOT [4-7] destination register [8-11] register with value for 'not-ing'
1010 XOR [4-7] destination register [8-11] first register [12-15] second register
1011 OR  [4-7] destination register [8-11] first register [12-15] second register
1100 AND [4-7] destination register [8-11] first register [12-15] second register
1101 PRT [4-7] register holding character to print

1110 ---

 - Invalid command. Nothing has been assigned to this value, so please don't use
 it in your programs!

1111 STP [4-15] 1's

 - Stop Execution. This stops your program's execution.

# Versions

## V0.* Goals

### V0.1
 - Be able to run an arbitrary instruction set via a command-line program.
### V0.2
 - Create a two-pass assembler for the arbitrary instruction set.
### V0.3
 - Create a 'high-level' language (why not 'ruf-C?') and an appropriate compiler.
 - Only handle one higher-language program at a time
### V0.4
 - Create linkers, loaders, etc. for higher-level language.
### V0.5 ?
 - Create a more dynamic high-level language for instruction set.
 (Lisp-y? or Python-y?)

## V1.* Goals

 - Create a new 'rufcpu2' program that behaves more like an actual single-pipeline
 processor.
 - Create a new 'rufcpu2' instruction set (keep the old one too) for the new
 'rufcpu2'.
 -
