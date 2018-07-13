/**~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
＊　ファイル名： KernelTimer.c
＊　内容　　　： カーネルタイマ用CMT0の設定
＊　作成日　　： 2018/6/22
＊　作成者　　： kern-gt

　FreeRTOSデモプロジェクトを参考に適当なものを作成しました。
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~**/
/**----------------------------------------------------------------------------
<<利用ファイルのヘッダ>>
-----------------------------------------------------------------------------**/
/* Renesas includes. */
/*#include "r_cg_macrodriver.h"*/
/*#include "r_cg_userdefine.h"*/
#include "iodefine.h"

/* Kernel includes. */
#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"

/***カーネルタイマ初期設定*******************************************************************/
/*
	カーネルタイマ（OSタイマ）にはCMT0を使用している。
	スケジューラ起動時にこのコールバック関数が呼ばれタイマの周期設定後、タイマを起動する。
	また、FreeRTOS/portable/port.cにOStickハンドラとしてCMT0の割込みハンドラを定義しているので、
	コード生成ツールを使用する場合はCMT0を使用しないようにする。
	以下の関数はデモ用のサンプルを参考に一部修正している。
		　デフォルトのtick周波数は1000Hzでこのまま使うのであれば問題ないが、100Hzで使いたい場合は114Hz以下の
		設定ができないのでその際はCMT0のクロックソースをPCLKB/8からPCLKB/32に変更するようにしている。ただし、
		30Hz未満にはできないので注意すること。
	
	 The RX port uses this callback function to configure its tick interrupt.
	This allows the application to choose the tick interrupt source. 
*/
void vApplicationSetupTimerInterrupt( void )
{
	const uint32_t ulEnableRegisterWrite = 0xA50BUL, ulDisableRegisterWrite = 0xA500UL;

	/* Disable register write protection. */
	SYSTEM.PRCR.WORD = ulEnableRegisterWrite;

	/* Enable compare match timer 0. */
	MSTP( CMT0 ) = 0;

	/* Interrupt on compare match. */
	CMT0.CMCR.BIT.CMIE = 1;

	/* Set the compare match value. */
	if(configTICK_RATE_HZ > 114){
		/* Divide the PCLK by 8. */
		CMT0.CMCR.BIT.CKS = 0;
		CMT0.CMCOR = ( unsigned short ) ( ( ( configPERIPHERAL_CLOCK_HZ / configTICK_RATE_HZ ) -1 ) / 8 );
	}else  if(configTICK_RATE_HZ > 29){
		/* Divide the PCLK by 32. */
		CMT0.CMCR.BIT.CKS = 1;
		CMT0.CMCOR = ( unsigned short ) ( ( ( configPERIPHERAL_CLOCK_HZ / configTICK_RATE_HZ ) -1 ) / 32 );
	}else{
		/*configTICK_RATE_HZ is very small.*/
		while(1);
	}

	/* Enable the interrupt... */
	_IEN( _CMT0_CMI0 ) = 1;

	/* ...and set its priority to the application defined kernel priority. */
	_IPR( _CMT0_CMI0 ) = configKERNEL_INTERRUPT_PRIORITY;

	/* Start the timer. */
	CMT.CMSTR0.BIT.STR0 = 1;

    /* Reneable register protection. */
    SYSTEM.PRCR.WORD = ulDisableRegisterWrite;
}
/*-----------------------------------------------------------*/
