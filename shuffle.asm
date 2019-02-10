.ORIG x3000
       LEA R0, Y
       AND R6, R6, #0
       ADD R6, R6, x04
Loop   LDB R1, R0, #0
       LDB R2, R0, #1
       STB R1, R0, #1
       STB R2, R0, #0
       ADD R0, R0, #2
       ADD R6, R6, #-1
       BRp Loop
       HALT
       

Y .FILL  x3050
.END