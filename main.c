#include <stdio.h> /* standard input/output library */
#include <stdlib.h> /* Standard C Library */
#include <string.h> /* String operations library */
#include <ctype.h> /* Library for useful character operations */
#include <limits.h> /* Library for definitions of common variable type characteristics */
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


#define MAX_LINE_LENGTH 255

FILE* infile = NULL;
FILE* outfile = NULL;

int CURRENT_LINE = 0;
int labelTableLength = 0;
typedef struct labelTable{
    int offset;
    char* label;
} labelTable;
labelTable labels[MAX_LINE_LENGTH];

int findLabel(char *inputLabel) {
    for (int i = 0; i < labelTableLength; i++) {
        if (0 == strcmp(labels[i].label, inputLabel)) {
            return labels[i].offset;
        }
    }
    exit(1);
}

enum
{
    DONE, OK, EMPTY_LINE
};

int toNum( char* pStr);

int readAndParse( FILE * pInfile, char * pLine, char ** pLabel, char ** pOpcode,
                  char ** pArg1, char ** pArg2, char ** pArg3, char ** pArg4);
void buildLabelTable(FILE * lInfile);

void assembleCode(FILE * lInfile, FILE* lOutFile);

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





int main(int argc, char* argv[]) {
    for(int i =0;i<MAX_LINE_LENGTH;i++){
        labels[i].label = (char *) malloc(20);
        labels[i].offset = 0;
    }
    /* open the source file */
    infile = fopen(argv[1], "r");
    outfile = fopen(argv[2], "w");

    if (!infile) {
        printf("Error: Cannot open file %s\n", argv[1]);
        exit(4);
    }
    if (!outfile) {
        printf("Error: Cannot open file %s\n", argv[2]);
        exit(4);
    }
    //labelTable labels[MAX_LINE_LENGTH] ;
    buildLabelTable(infile);
    for(int i =0;i<labelTableLength;i++){
        printf("Label is: %s with offset %x\n",labels[i].label,labels[i].offset);
    }
    printf("%x",findLabel("a"));
    printf("%x",findLabel("y"));
    printf("%x",findLabel("zztop_theman"));
    printf("%x",findLabel("ahiko"));


    /* Do stuff with files */

    fclose(infile);
    fclose(outfile);
}



int toNum( char * pStr ) {
    char * t_ptr;
    char * orig_pStr;
    int t_length,k;
    int lNum, lNeg = 0;
    long int lNumLong;

    orig_pStr = pStr;
    if( *pStr == '#' )				/* decimal */
    {
        pStr++;
        if( *pStr == '-' )				/* dec is negative */
        {
            lNeg = 1;
            pStr++;
        }
        t_ptr = pStr;
        t_length = strlen(t_ptr);
        for(k=0;k < t_length;k++)
        {
            if (!isdigit(*t_ptr))
            {
                printf("Error: invalid decimal operand, %s\n",orig_pStr);
                exit(4);
            }
            t_ptr++;
        }
        lNum = atoi(pStr);
        if (lNeg)
            lNum = -lNum;

        return lNum;
    }
    else if( *pStr == 'x' )	/* hex     */
    {
        pStr++;
        if( *pStr == '-' )				/* hex is negative */
        {
            lNeg = 1;
            pStr++;
        }
        t_ptr = pStr;
        t_length = strlen(t_ptr);
        for(k=0;k < t_length;k++)
        {
            if (!isxdigit(*t_ptr))
            {
                printf("Error: invalid hex operand, %s\n",orig_pStr);
                exit(4);
            }
            t_ptr++;
        }
        lNumLong = strtol(pStr, NULL, 16);    /* convert hex string into integer */
        lNum = (lNumLong > INT_MAX)? INT_MAX : lNumLong;
        if( lNeg )
            lNum = -lNum;
        return lNum;
    }
    else
    {
        printf( "Error: invalid operand, %s\n", orig_pStr);
        exit(4);  /* This has been changed from error code 3 to error code 4, see clarification 12 */
    }
}
int isOpcode(char *lptr) {
    //check to see if it is an opcode...
    if(0==strcmp(lptr,"add")){
        return 0;
    }
    if(0==strcmp(lptr,"and")){
        return 1;
    }
    if(0==strcmp(lptr,"br")){
        return 2;
    }
    if(0==strcmp(lptr,"brn")){
        return 3;
    }
    if(0==strcmp(lptr,"brz")){
        return 4;
    }
    if(0==strcmp(lptr,"brp")){
        return 5;
    }
    if(0==strcmp(lptr,"brnz")){
        return 6;
    }
    if(0==strcmp(lptr,"brzp")){
        return 7;
    }
    if(0==strcmp(lptr,"brnp")){
        return 8;
    }
    if(0==strcmp(lptr,"brnzp")){
        return 9;
    }
    if(0==strcmp(lptr,"halt")){
        return 10;
    }
    if(0==strcmp(lptr,"jmp")){
        return 11;
    }
    if(0==strcmp(lptr,"jsr")){
        return 12;
    }
    if(0==strcmp(lptr,"jsrr")){
        return 13;
    }
    if(0==strcmp(lptr,"ldb")){
        return 14;
    }
    if(0==strcmp(lptr,"ldw")){
        return 15;
    }
    if(0==strcmp(lptr,"lea")){
        return 16;
    }
    if(0==strcmp(lptr,"nop")){
        return 17;
    }
    if(0==strcmp(lptr,"not")){
        return 18;
    }
    if(0==strcmp(lptr,"ret")){
        return 19;
    }
    if(0==strcmp(lptr,"lshf")){
        return 20;
    }
    if(0==strcmp(lptr,"rshfl")){
        return 21;
    }
    if(0==strcmp(lptr,"rshfa")){
        return 22;
    }
    if(0==strcmp(lptr,"rti")){
        return 23;
    }
    if(0==strcmp(lptr,"stb")){
        return 24;
    }
    if(0==strcmp(lptr,"stw")){
        return 25;
    }
    if(0==strcmp(lptr,"trap")){
        return 26;
    }
    if(0==strcmp(lptr,"xor")){
        return 27;
    }
    if(0==strcmp(lptr,".orig")){
        return 28;
    }
    if(0==strcmp(lptr,".end")){
        return 29;
    }
    if(0==strcmp(lptr,".fill")){
        return 30;
    }
    return -1;
}

int readAndParse( FILE * pInfile, char * pLine, char ** pLabel, char
** pOpcode, char ** pArg1, char ** pArg2, char ** pArg3, char ** pArg4
)
{
    char * lRet, * lPtr;
    int i;
    if( !fgets( pLine, MAX_LINE_LENGTH, pInfile ) )
        return( DONE );
    for( i = 0; i < strlen( pLine ); i++ )
        pLine[i] = tolower( pLine[i] );

    /* convert entire line to lowercase */
    *pLabel = *pOpcode = *pArg1 = *pArg2 = *pArg3 = *pArg4 = pLine + strlen(pLine);

    /* ignore the comments */
    lPtr = pLine;

    while( *lPtr != ';' && *lPtr != '\0' &&
           *lPtr != '\n' )
        lPtr++;

    *lPtr = '\0';
    if( !(lPtr = strtok( pLine, "\t\n ," ) ) )
        return( EMPTY_LINE );

    if( isOpcode( lPtr ) == -1 && lPtr[0] != '.' ) /* found a label */
    {
        *pLabel = lPtr;
        if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );
    }

    *pOpcode = lPtr;

    if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );

    *pArg1 = lPtr;

    if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );

    *pArg2 = lPtr;
    if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );

    *pArg3 = lPtr;

    if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );

    *pArg4 = lPtr;
    printf("%s %s %s %s %s %s\n", pOpcode[0], pLabel[0], pArg1[0],pArg2[0],pArg3[0],pArg4[0]);
    return( OK );

}

void buildLabelTable(FILE * lInfile) {
    char lLine[MAX_LINE_LENGTH + 1], *lLabel, *lOpcode, *lArg1,
            *lArg2, *lArg3, *lArg4;

    int lRet;
    int offset = 0;
    int i =0;
    //lInfile = fopen( "data.in", "r" );	/* open the input file */
    do
    {
        lRet = readAndParse( lInfile, lLine, &lLabel,
                             &lOpcode, &lArg1, &lArg2, &lArg3, &lArg4 );
        if( lRet != DONE && lRet != EMPTY_LINE )
        {
            if(0!=strcmp(lLabel,"")){
                strcpy(labels[i].label,lLabel);
                labels[i].offset = offset;
                labelTableLength++;
                i++;
            }
        }
        if(0==strcmp(".orig",lOpcode)){
            offset = 0;
        }
        else{
            offset+=1;
        }
        lLabel="";
        lOpcode="";
        lArg1="";
        lArg1="";
        lArg2="";
        lArg3="";
        lArg4="";
    } while( lRet != DONE );
}

void assembleCode(FILE * lInfile, FILE* lOutFile){
    char lLine[MAX_LINE_LENGTH + 1], *lLabel, *lOpcode, *lArg1,
            *lArg2, *lArg3, *lArg4;

    int lRet;
    //lInfile = fopen( "data.in", "r" );	/* open the input file */
    do {
        lRet = readAndParse( lInfile, lLine, &lLabel,
                             &lOpcode, &lArg1, &lArg2, &lArg3, &lArg4 );
        if( lRet != DONE && lRet != EMPTY_LINE ) {
            int outputLine;
            switch(isOpcode(lOpcode)){
                case 0:
                    outputLine= add(lArg1,lArg2,lArg3);
                    break;
                case 1:
                    outputLine= and(lArg1,lArg2,lArg3);
                    break;
                case 2:
                    outputLine= br(lArg1);
                    break;
                case 3:
                    outputLine= brn(lArg1);
                    break;
                case 4:
                    outputLine= brz(lArg1);
                    break;
                case 5:
                    outputLine= brp(lArg1);
                    break;
                case 6:
                    outputLine= brnz(lArg1);
                    break;
                case 7:
                    outputLine= brnp(lArg1);
                    break;
                case 8:
                    outputLine= brzp(lArg1);
                    break;
                case 9:
                    outputLine= brnzp(lArg1);
                    break;
                case 10:
                    outputLine= halt();
                    break;
                case 11:
                    outputLine= jmp(lArg1);
                    break;
                case 12:
                    outputLine= jsr(lArg1);
                    break;
                case 13:
                    outputLine= jsrr(lArg1);
                    break;
                case 14:
                    outputLine= ldb(lArg1,lArg2,lArg3);
                    break;
                case 15:
                    outputLine= ldw(lArg1,lArg2,lArg3);
                    break;
                case 16:
                    outputLine= lea(lArg1,lArg2);
                    break;
                case 17:
                    outputLine= nop();
                    break;
                case 18:
                    outputLine= not(lArg1,lArg2);
                    break;
                case 19:
                    outputLine= ret();
                    break;
                case 20:
                    outputLine= lshf(lArg1,lArg2,lArg3);
                    break;
                case 21:
                    outputLine= rshfl(lArg1,lArg2,lArg3);
                    break;
                case 22:
                    outputLine= rshfa(lArg1,lArg2,lArg3);
                    break;
                case 23:
                    outputLine= rti();
                    break;
                case 24:
                    outputLine= stb(lArg1,lArg2,lArg3);
                    break;
                case 25:
                    outputLine= stw(lArg1,lArg2,lArg3);
                    break;
                case 26:
                    outputLine= trap(lArg1);
                    break;
                case 27:
                    outputLine= xor(lArg1,lArg2,lArg3);
                    break;
                case 28:
                    outputLine= orig(lArg1);
                    break;
                case 29:
                    exit(0);
                case 30:
                    outputLine= fill(lArg1);
                    break;
                default:
                    exit(4);
            }
            fprintf(lOutFile,"0x%x",outputLine);
            
        }
        if(0==strcmp(".orig",lOpcode)){
            CURRENT_LINE = 0;
        }
        else{
            CURRENT_LINE+=1;
        }
        lLabel="";
        lOpcode="";
        lArg1="";
        lArg1="";
        lArg2="";
        lArg3="";
        lArg4="";
    } while( lRet != DONE );
}


/* Note: MAX_LINE_LENGTH, OK, EMPTY_LINE, and DONE are defined values */