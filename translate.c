#include <stdio.h>
#include <math.h>


#define dr_r0	0x0000
#define dr_r1	0x0200
#define dr_r2	0x0400
#define dr_r3	0x0600
#define dr_r4	0x0800
#define dr_r5	0x0A00
#define dr_r6	0x0C00
#define dr_r7	0x0E00
#define sr1_r0	0x0000
#define sr1_r1	0x0040
#define sr1_r2	0x0080
#define sr1_r3	0x00C0
#define sr1_r4	0x0100
#define sr1_r5	0x0140
#define sr1_r6	0x0180
#define sr1_r7	0x01C0
#define sr2_r0	0x0000
#define sr2_r1	0x0001
#define sr2_r2	0x0002
#define sr2_r3	0x0003
#define sr2_r4	0x0004
#define sr2_r5	0x0005
#define sr2_r6	0x0006
#define sr2_r7	0x0007

int decodeDR(char* arg1){
	if(arg1[0] != 'r' || arg1[2] != NULL){
		exit(4);
	}
	else{
		switch(arg1[1]){
			case 0:
				return dr_r0;
				break;

			case 1:
				return dr_r1;
				break;

			case 2:
				return dr_r2;
				break;

			case 3:
				return dr_r3;
				break;

			case 4:
				return dr_r4;
				break;

			case 5:
				return dr_r5;
				break;

			case 6:
				return dr_r6;
				break;

			case 7:
				return dr_r7;
				break;

			default:
				exit(4);
		}
	}
}

int decodeSR1(char* arg2){
	if(arg2[0] != 'r' || arg2[2] != NULL){
		exit(4);
	}
	else{
		switch(arg2[1]){
			case 0:
				return sr1_r0;
				break;

			case 1:
				return sr1_r1;
				break;

			case 2:
				return sr1_r2;
				break;

			case 3:
				return sr1_r3;
				break;

			case 4:
				return sr1_r4;
				break;

			case 5:
				return sr1_r5;
				break;

			case 6:
				return sr1_r6;
				break;

			case 7:
				return sr1_r7;
				break;

			default:
				exit(4);
		}
	}
}

int decodeSR2(char* arg3, int numBits){
	bool imm = false;
	int output = 0;
	int mask = 0;
	if(arg3[0] != 'r' || arg3[2] != NULL){
		output = toNum(arg3);
		imm = true;
		mask = pow(2.0, numBits) - 1;
		output = output & mask;
		if(imm == true && numBits == 5){
			output += 0x0020;
		}
		return output;
		exit(4);
	}
	else{
		switch(arg3[1]){
			case 0:
				return sr2_r0;
				break;

			case 1:
				return sr2_r1;
				break;

			case 2:
				return sr2_r2;
				break;

			case 3:
				return sr2_r3;
				break;

			case 4:
				return sr2_r4;
				break;

			case 5:
				return sr2_r5;
				break;

			case 6:
				return sr2_r6;
				break;

			case 7:
				return sr2_r7;
				break;

			default:
				exit(4);
		}
	}
}

int	calcOffset(char* arg1, int numBits){
	int output = findLabel(arg1) - (CURRENT_LINE + 1);
	int mask = pow(2.0, numBits) - 1;
	output = output & mask;
	return output;
}

int add(char* arg1, char* arg2, char* arg3){
	int output = 0x0000;
	output += 0x1000;
	output += decodeDR(arg1);
	output += decodeSR1(arg2);
	output += decodeSR2(arg3, 5);
	return output;
}
int and(char* arg1, char* arg2, char* arg3){
	int output = 0x0000;
	output += 0x5000;
	output += decodeDR(arg1);
	output += decodeSR1(arg2);
	output += decodeSR2(arg3, 5);
	return output;
}
int br(char* arg1){
	int output = 0x0000;
	output += 0x0000;
	output += calcOffset(arg1, 9);
	return output;
}
int brn(char* arg1){
	int output = 0x0000;
	output += 0x0800;
	output += calcOffset(arg1, 9);
	return output;
}
int brz(char* arg1){
	int output = 0x0000;
	output += 0x0400;
	output += calcOffset(arg1, 9);
	return output;
}
int brp(char* arg1){
	int output = 0x0000;
	output += 0x0200;
	output += calcOffset(arg1, 9);
	return output;
}
int brnz(char* arg1){
	int output = 0x0000;
	output += 0x0C00;
	output += calcOffset(arg1, 9);
	return output;
}
int brzp(char* arg1){
	int output = 0x0000;
	output += 0x0600;
	output += calcOffset(arg1, 9);
	return output;
}
int brnp(char* arg1){
	int output = 0x0000;
	output += 0x0A00;
	output += calcOffset(arg1, 9);
	return output;
}
int brnzp(char* arg1){
	int output = 0x0000;
	output += 0x0E00;
	output += calcOffset(arg1, 9);
	return output;
}
int halt(){
	return trap("x25");
}
int jmp(char* arg1){
	int output = 0x0000;
	output += 0xC000;
	output += decodeSR1(arg1);
	return output;
}
int jsr(char* arg1){
	int output = 0x0000;
	output += 0x4800;
	output += calcOffset(arg1, 11);
	return output;
}
int jsrr(char* arg1){
	int output = 0x0000;
	output += 0x4000;
	output += decodeSR1(arg1);
	return output;
}
int ldb(char* arg1, char* arg2, char* arg3){
	int output = 0x0000;
	output += 0x2000;
	output += decodeDR(arg1);
	output += decodeSR1(arg2);
	output += decodeSR2(arg3, 6);
	return output;
}
int ldw(char* arg1, char* arg2, char* arg3){
	int output = 0x0000;
	output += 0x6000;
	output += decodeDR(arg1);
	output += decodeSR1(arg2);
	output += decodeSR2(arg3, 6);
	return output;
}
int lea(char* arg1, char* arg2){
	int output = 0x0000;
	output += 0xE000;
	output += decodeDR(arg1);
	output += calcOffset(arg2, 9);
	return output;
}
int nop(){
	return 0x0000;
}
int not(char* arg1, char* arg2){
	int output = 0x0000;
	output += 0x903F;
	output += decodeDR(arg1);
	output += decodeSR1(arg2);
	return output;
}
int ret(){
	return 0xC1C0;
}
int lshf(char* arg1, char* arg2, char* arg3){
	int output = 0x0000;
	output += 0xD000;
	output += decodeDR(arg1);
	output += decodeSR1(arg2);
	output += decodeSR2(arg3, 4);
	return output;
}
int rshfl(char* arg1, char* arg2, char* arg3){
	int output = 0x0000;
	output += 0xD010;
	output += decodeDR(arg1);
	output += decodeSR1(arg2);
	output += decodeSR2(arg3, 4);
	return output;
}
int rshfa(char* arg1, char* arg2, char* arg3){
	int output = 0x0000;
	output += 0xD030;
	output += decodeDR(arg1);
	output += decodeSR1(arg2);
	output += decodeSR2(arg3, 4);
	return output;
}
int rti(){
	int output = 0x0000;
	output += 0x8000;
	return output;
}
int stb(char* arg1, char* arg2, char* arg3){
	int output = 0x0000;
	output += 0x3000;
	output += decodeDR(arg1);
	output += decodeSR1(arg2);
	output += decodeSR2(arg3, 6);
	return output;
}
int stw(char* arg1, char* arg2, char* arg3){
	int output = 0x0000;
	output += 0x7000;
	output += decodeDR(arg1);
	output += decodeSR1(arg2);
	output += decodeSR2(arg3, 6);
	return output;
}
int trap(char* arg1){
	int output = 0x0000;
	output += 0xF000;
	output += decodeSR2(arg1, 8);
	return output;
}
int xor(char* arg1, char* arg2, char* arg3){
	int output = 0x0000;
	output += 0x9000;
	output += decodeDR(arg1);
	output += decodeSR1(arg2);
	output += decodeSR2(arg3, 5);
	return output;
}
int fill(char* arg1){
	int output = 0x0000;
	output += decodeSR2(arg1, 16);
	return output;
}
int orig(char* arg1){
	int output = 0x0000;
	output += decodeSR2(arg1, 16);
	return output;
}