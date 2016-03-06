
#include <project.h>

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */
   
    EZI2C_Start();
    EZI2C_EzI2CSetBuffer1(sizeof(CapSense_dsRam), sizeof(CapSense_dsRam),(uint8 *)&CapSense_dsRam);
    
    CapSense_Start();
    CapSense_InitializeAllBaselines();
    CapSense_ScanAllWidgets();

    for(;;)
    {
        if(!CapSense_IsBusy())
        {
            CapSense_ProcessAllWidgets();
            
            CapSense_RunTuner();
        
            LED10_Write(!CapSense_IsWidgetActive(CapSense_BUTTON0_WDGT_ID));
            LED11_Write(!CapSense_IsWidgetActive(CapSense_BUTTON1_WDGT_ID));
            LED9_Write(!CapSense_IsWidgetActive(CapSense_BUTTON2_WDGT_ID));
                      
            CapSense_UpdateAllBaselines();
            CapSense_ScanAllWidgets();
        }          
    }
}
