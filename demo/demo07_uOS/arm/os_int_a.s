
                PRESERVE8
                

        AREA  UCOS_ARM_ASM_INT, CODE, READONLY
           

;*********************************************************************************************************
;                                            IRQ HANDLER
;
;        This handles all the IRQs
;        Note: FIQ Handler should be written similar to this
;
;*********************************************************************************************************

        IMPORT  OSIntEnter
        IMPORT  OSIntExit

        IMPORT  OSIntCtxSwFlag
        IMPORT  _IntCtxSw

  
        MACRO     
$IRQ_AsmEntery HANDLER $IRQ_CEntry
        EXPORT  $IRQ_AsmEntery              ; 输出的标号
        IMPORT  $IRQ_CEntry                 ; 引用的外部标号
                
$IRQ_AsmEntery
        stmfd sp!,{r0-r3,r12,lr}

        bl OSIntEnter
        bl $IRQ_CEntry
        bl OSIntExit

        ldr r0,=OSIntCtxSwFlag
        ldr r1,[r0]
        cmp r1,#1
        beq _IntCtxSw

        ldmfd sp!,{r0-r3,r12,lr}
        subs pc,lr,#4
        
        MEND
 
;所有的中断按如下的格式在注册(引用上面的宏定义)
;$IRQ_AsmEntery HANDLER $IRQ_CEntry




;定时器0中断
IRQASMTimer0  HANDLER IRQCTimer0

;定时器1中断
;IRQASMTimer1 HANDLER IRQCTimer1

;串口0中断
;IRQASMUart0  HANDLER IRQCUart0

;串口0中断
;IRQASMUart1  HANDLER IRQCUart1


;外部中断0中断
;IRQASMEx0     HANDLER IRQCEx0

;外部中断1中断
;IRQASMEx1    HANDLER IRQCEx1

;外部中断20中断
;IRQASMEx2    HANDLER IRQCEx2

;SPI中断
;IRQASMSpi    HANDLER IRQCSpi

;I2C中断        
;IRQASMI2c    HANDLER IRQCI2c

          END
   
                        
