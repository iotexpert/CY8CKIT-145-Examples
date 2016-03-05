
#include <project.h>

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    SmartIO_Start();
    
    PWM_Start();
  
    CapSense_Start();
    CapSense_InitializeAllBaselines();
    CapSense_ScanAllWidgets();

    for(;;)
    {
        if(!CapSense_IsBusy())
        {
            CapSense_ProcessAllWidgets();
        
            LED10_Write(!CapSense_IsWidgetActive(CapSense_BUTTON0_WDGT_ID));
            LED11_Write(!CapSense_IsWidgetActive(CapSense_BUTTON1_WDGT_ID));
            LED9_Write(!CapSense_IsWidgetActive(CapSense_BUTTON2_WDGT_ID));
                      
            CapSense_UpdateAllBaselines();
            CapSense_ScanAllWidgets();
        }
        
        P20_Write(!LED9_Read());
    }
}
