
#include <project.h>

CY_ISR(pintInt)
{
    
    P22_ClearInterrupt();
    P25_Write(!P25_Read());
}

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    SmartIO_Start();
    
    P25_Write(1);
    
    isr_1_StartEx(pintInt);
    
    PWM_Start();
 
    for(;;)
    {
        
    }
}
