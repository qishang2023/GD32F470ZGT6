//
// Created by pc on 2024/1/11.
//

#include "bsp_Wdgt.h"

/********************************************************************************
* @author: Qi Shang
* @brief: 独立看门狗初始化
* @param:
* @retval:
********************************************************************************/
void fwtd_config() {
    rcu_osci_on(RCU_IRC32K);

    while( SUCCESS != rcu_osci_stab_wait(RCU_IRC32K) ){  }
    fwdgt_config( 1000,  FWDGT_PSC_DIV64);//32K晶振进行64分频，500HZ，2秒内必须喂狗
    fwdgt_enable();
    //fwdgt_counter_reload(); 独立看门狗喂狗函数
}

/********************************************************************************
* @author: Qi Shang
* @brief: 窗口看门狗初始化
* @param:
* @retval:
********************************************************************************/
void wwtd_config() {
    //1.打开时钟
    rcu_periph_clock_enable(RCU_WWDGT);
    //重置看门狗
    wwdgt_deinit();
    //2.配置预分频
    //3.配置窗口上限 (窗口下限0x3F)
    //WWDGT_CFG_PSC_DIV8
    //时钟频率:60000000<=>1s
    //经过4096分频: 60000000/4096<=>1s
    //经过WWDGT_CFG_PSC_DIV8:60000000/4096/8<=>1s  1831
    //倒计时计数:0x7F<=>127
    //窗口上限:80
    //窗口下限:0x3F<=>63
    //最短喂狗时间:0x7F-80  经过时间   47个数经过时间  47/1831*1000 = 26ms
    //最长喂狗时间:0x7F-0x3F经过时间   64个数经过时间  64/1831*1000 = 34.9ms
    wwdgt_config(0x7F,80,WWDGT_CFG_PSC_DIV8);
    //4.打开看门狗
    wwdgt_enable();
    // wwdgt_counter_update(127);  窗口看门狗喂狗函数
}