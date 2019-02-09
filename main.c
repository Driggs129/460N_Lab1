#include <stdio.h> /* standard input/output library */
#include <stdlib.h> /* Standard C Library */
#include <string.h> /* String operations library */
#include <ctype.h> /* Library for useful character operations */
#include <limits.h> /* Library for definitions of common variable type characteristics */


#define MAX_LINE_LENGTH 255

FILE* infile = NULL;
FILE* outfile = NULL;

enum
{
    DONE, OK, EMPTY_LINE
};

int toNum( char* pStr);
int readAndParse( FILE * pInfile, char * pLine, char ** pLabel, char ** pOpcode,
                  char ** pArg1, char ** pArg2, char ** pArg3, char ** pArg4);
void buildLabelTable(FILE * pInfile, char** labelTable[]);

int main(int argc, char* argv[]) {

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
    char **labelTable[MAX_LINE_LENGTH];
    buildLabelTable(infile, labelTable);




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
    if(strcmp(lptr,"add")){
        return -1;
    }
    if(strcmp(lptr,"and")){
        return -1;
    }
    if(strcmp(lptr,"br")){
        return -1;
    }
    if(strcmp(lptr,"brn")){
        return -1;
    }
    if(strcmp(lptr,"brz")){
        return -1;
    }
    if(strcmp(lptr,"brp")){
        return -1;
    }
    if(strcmp(lptr,"brnz")){
        return -1;
    }
    if(strcmp(lptr,"brzp")){
        return -1;
    }
    if(strcmp(lptr,"brnp")){
        return -1;
    }
    if(strcmp(lptr,"brnzp")){
        return -1;
    }
    if(strcmp(lptr,"halt")){
        return -1;
    }
    if(strcmp(lptr,"jmp")){
        return -1;
    }
    if(strcmp(lptr,"jsr")){
        return -1;
    }
    if(strcmp(lptr,"jsrr")){
        return -1;
    }
    if(strcmp(lptr,"ldb")){
        return -1;
    }
    if(strcmp(lptr,"ldw")){
        return -1;
    }
    if(strcmp(lptr,"lea")){
        return -1;
    }
    if(strcmp(lptr,"nop")){
        return -1;
    }
    if(strcmp(lptr,"not")){
        return -1;
    }
    if(strcmp(lptr,"ret")){
        return -1;
    }
    if(strcmp(lptr,"lshf")){
        return -1;
    }
    if(strcmp(lptr,"rshfl")){
        return -1;
    }
    if(strcmp(lptr,"rshfa")){
        return -1;
    }
    if(strcmp(lptr,"rti")){
        return -1;
    }
    if(strcmp(lptr,"stb")){
        return -1;
    }
    if(strcmp(lptr,"stw")){
        return -1;
    }
    if(strcmp(lptr,"trap")){
        return -1;
    }
    if(strcmp(lptr,"xor")){
        return -1;
    }
    return 0;
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

void buildLabelTable(FILE * lInfile, char** labelTable[])
{
    char lLine[MAX_LINE_LENGTH + 1], *lLabel, *lOpcode, *lArg1,
            *lArg2, *lArg3, *lArg4;

    int lRet;

    //lInfile = fopen( "data.in", "r" );	/* open the input file */

    do
    {
        lRet = readAndParse( lInfile, lLine, &lLabel,
                             &lOpcode, &lArg1, &lArg2, &lArg3, &lArg4 );
        if( lRet != DONE && lRet != EMPTY_LINE )
        {

        }
    } while( lRet != DONE );
}


/* Note: MAX_LINE_LENGTH, OK, EMPTY_LINE, and DONE are defined values */