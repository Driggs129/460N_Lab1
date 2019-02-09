
// Input integers for register and immediate numbers
// Output binary machine code for each line
int add(char* arg1, char* arg2, char* arg3);
int and(char* arg1, char* arg2, char* arg3);
int br(char* arg1);
int brn(char* arg1);
int brz(char* arg1);
int brp(char* arg1);
int brnz(char* arg1);
int brzp(char* arg1);
int brnp(char* arg1);
int brnzp(char* arg1);
int halt();
int jmp(char* arg1);
int jsr(char* arg1);
int jsrr(char* arg1);
int ldb(char* arg1, char* arg2, char* arg3);
int ldw(char* arg1, char* arg2, char* arg3);
int lea(char* arg1, char* arg2);
int nop();
int not(char* arg1, char* arg2);
int ret();
int lshf(char* arg1, char* arg2, char* arg3);
int rshfl(char* arg1, char* arg2, char* arg3);
int rshfa(char* arg1, char* arg2, char* arg3);
int rti();
int stb(char* arg1, char* arg2, char* arg3);
int stw(char* arg1, char* arg2, char* arg3);
int trap(char* arg1);
int xor(char* arg1, char* arg2, char* arg3);
int fill(char* arg1);
int orig(char* arg1);


int decodeDR(char* arg1);
int decodeSR1(char* arg2);
int decodeSR2(char* arg3, int numBits);
int calcOffset(char* arg1, int numBits);