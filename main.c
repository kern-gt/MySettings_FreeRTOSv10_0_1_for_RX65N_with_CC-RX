/**~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
＊　ファイル名： main.c Ver1.00
＊　内容　　　： FreeRTOSv10.0.1デモプロジェクト（RenesasRX65N用）
＊　作成日　　： 2018/3/11
＊　作成者　　： kern-gt

	文字エンコード：UTF-8

	FreeRTOS:v10.0.1　(RX600 RXv2)
	開発環境：CS+forCC V6.01.00
	コンパイラ：CC-RX V2.08.00
	CPUボード：TARGET BOARD for RX65N(RTK5RX65N0C00000BR)
	CPU：R5F565NEDDFP

	デモ内容：
	CPUボード上のLED0を1Hz、LED1を5Hzで点滅させる2つのタスクを動かします。

	クロック発生回路とポート初期化をスマートコンフィグレータで設定しています。
	FreeRTOSではカーネルタイマにCMT0,システムコールにソフトウェア割込み(SWINT)を使用している
	ので、その機能は使用しないでください。
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~**/
/**----------------------------------------------------------------------------
<<利用ファイルのヘッダ>>
-----------------------------------------------------------------------------**/
/* Renesas includes. */
/*#include "r_cg_macrodriver.h"*/
/*#include "r_cg_userdefine.h"*/
#include "iodefine.h"

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Prototype */
void main(void);
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);


void main(void)
{
	/*タスク生成*/
	xTaskCreate(vTask1,"Task1",100,NULL,1,NULL);
	xTaskCreate(vTask2,"Task2",100,NULL,1,NULL);
	
	/* OS起動*/
	vTaskStartScheduler();

	/*ここには戻ってこないはず*/
	while(1);
}


/***タスク定義*******************************************************************/
void vTask1(void *pvParameters)
{
	PORTD.PDR.BIT.B6 = 1;
	PORTD.DSCR.BIT.B6 = 1;
	while(1) {
		/*CPUボード上のLED0を1Hzで点滅*/
		PORTD.PODR.BIT.B6 = ~PORTD.PODR.BIT.B6;
		vTaskDelay(500/portTICK_PERIOD_MS);
	}
}

void vTask2(void *pvParameters)
{
	PORTD.PDR.BIT.B7 = 1;
	PORTD.DSCR.BIT.B7 = 1;
	while(1) {
		/*CPUボード上のLED1を5Hzで点滅*/
		PORTD.PODR.BIT.B7 = ~PORTD.PODR.BIT.B7;
		vTaskDelay(100/portTICK_PERIOD_MS);
	}
}