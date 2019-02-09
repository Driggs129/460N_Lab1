#include <stdio.h> /* standard input/output library */
#include <stdlib.h> /* Standard C Library */
#include <string.h> /* String operations library */
#include <ctype.h> /* Library for useful character operations */
#include <limits.h> /* Library for definitions of common variable type characteristics */


#define MAX_LINE_LENGTH 255

FILE* infile = NULL;
FILE* outfile = NULL;

int labelTableLength = 0;
typedef struct labelTable{
    int offset;
    char* label;
} labelTable;
labelTable labels[MAX_LINE_LENGTH];

enum
{
    DONE, OK, EMPTY_LINE
};

int toNum( char* pStr);

int readAndParse( FILE * pInfile, char * pLine, char ** pLabel, char ** pOpcode,
                  char ** pArg1, char ** pArg2, char ** pArg3, char ** pArg4);
void buildLabelTable(FILE * lInfile);



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
        return 0;
    }
    if(0==strcmp(lptr,"br")){
        return 0;
    }
    if(0==strcmp(lptr,"brn")){
        return 0;
    }
    if(0==strcmp(lptr,"brz")){
        return 0;
    }
    if(0==strcmp(lptr,"brp")){
        return 0;
    }
    if(0==strcmp(lptr,"brnz")){
        return 0;
    }
    if(0==strcmp(lptr,"brzp")){
        return 0;
    }
    if(0==strcmp(lptr,"brnp")){
        return 0;
    }
    if(0==strcmp(lptr,"brnzp")){
        return 0;
    }
    if(0==strcmp(lptr,"halt")){
        return 0;
    }
    if(0==strcmp(lptr,"jmp")){
        return 0;
    }
    if(0==strcmp(lptr,"jsr")){
        return 0;
    }
    if(0==strcmp(lptr,"jsrr")){
        return 0;
    }
    if(0==strcmp(lptr,"ldb")){
        return 0;
    }
    if(0==strcmp(lptr,"ldw")){
        return 0;
    }
    if(0==strcmp(lptr,"lea")){
        return 0;
    }
    if(0==strcmp(lptr,"nop")){
        return 0;
    }
    if(0==strcmp(lptr,"not")){
        return 0;
    }
    if(0==strcmp(lptr,"ret")){
        return 0;
    }
    if(0==strcmp(lptr,"lshf")){
        return 0;
    }
    if(0==strcmp(lptr,"rshfl")){
        return 0;
    }
    if(0==strcmp(lptr,"rshfa")){
        return 0;
    }
    if(0==strcmp(lptr,"rti")){
        return -0;
    }
    if(0==strcmp(lptr,"stb")){
        return 0;
    }
    if(0==strcmp(lptr,"stw")){
        return 0;
    }
    if(0==strcmp(lptr,"trap")){
        return 0;
    }
    if(0==strcmp(lptr,"xor")){
        return 0;
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
            offset+=2;
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