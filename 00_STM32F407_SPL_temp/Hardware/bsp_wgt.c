#include "bsp_wgt.h"

/********************************************************************************
* @author: Qi Shang
* @brief: 独立看门狗初始化
* @param:
* @retval:
********************************************************************************/
void fwdt_config(){
    RCC_LSICmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(IWDG_Prescaler_64);//32kHZ进行64分频，看门狗运行频率为500HZ
    IWDG_SetReload(1000);//看门狗重载值为1000，2秒内喂狗
    IWDG_ReloadCounter();//喂狗函数
    IWDG_Enable();
}

/********************************************************************************
* @author: Qi Shang
* @brief: 窗口看门狗初始化
* @param:
* @retval:
********************************************************************************/
void wwdt_config() {
    /* Enable WWDG clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
    /* WWDG clock counter = (PCLK1 (60MHz)/4096)/8 = 1831 Hz (~546 us)  */
    WWDG_SetPrescaler(WWDG_Prescaler_8);
    /* Set Window value to 80; WWDG counter should be refreshed only when the counter
      is below 80 (and greater than 64) otherwise a reset will be generated */
    WWDG_SetWindowValue(80);
    /* Enable WWDG and set counter value to 127, WWDG timeout = ~546 us * 64 = 34.95 ms
       In this case the refresh window is:
             ~780 * (127-80) = 25.67ms < refresh window < ~780 * 64 = 34.95ms
    */
    WWDG_Enable(127);
    WWDG_SetCounter(127);//喂狗函数
}