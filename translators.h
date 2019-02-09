
// Input integers for register and immediate numbers
// Output binary machine code for each line
char*   add(int dr, int sr1, int sr2);
char*   addIm(int dr, int sr, int imm);
char*   and(int dr, int sr1, int sr2);
char*   andIm(int dr, int sr, int imm);
char*   br(int n, int z, int p, int offset9);
char*   jmp(int base);
char*   jsr(int offset11);
char*   jsrr(int base);



