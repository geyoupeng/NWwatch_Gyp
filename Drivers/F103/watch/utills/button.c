#include "button.h"
#include "gpio.h"
#include "drivers.h"

static button_s button[BTNCOUNT] = {0}; //定义BTNCOUNT个按钮

/*
 * 描述：初始化按钮、设置每个按钮按下的声音
 * 参数：无
 * 返回值：无
 */
void ButtonInit(void){
	button[BTN1].tune = tuneBtn1;
	button[BTN2].tune = tuneBtn2;
	button[BTN3].tune = tuneBtn3;
}

/*
 * 描述：设置每个按钮按下触发的事件
 * 参数：button_f bt0 按钮0按下后触发的事件
 * 返回值：无
 */
void SetButtonFunc(button_f bt0,button_f bt1,button_f bt2){
	button[BTN1].onPress = bt0;
	button[BTN2].onPress = bt1;
	button[BTN3].onPress = bt2;
}

button_f SetButtonSignedFunc(uint8_t num,button_f bt){

	button_f oldfunc = button[num].onPress;
	button[num].onPress = bt;
	return oldfunc;
}

/*
 * 描述：按钮按下以后进行消抖等处理、一次按下只执行一次事件
 * 参数：buttons：要判断的按钮
 * 参数：isPressed：true 被按下
 * 返回值：无
 */
static void precessButton(button_s *buttons,bool isPressed){
	buttons->counter <<= 1;
	if(isPressed)
	{
		buttons->counter++;
		if( buttons->counter > 0x4 )
		{
			if(!buttons->processed && buttons->onPress != NULL)
			{
                buttons->processed = true;
				LedFlash(LEDGREEN,50);
				buttons->onPress();
				BuzzerPlay(buttons->tune, VOL_UI, PRIO_UI);
			}
		}
	}else if(buttons->counter)
	{
		buttons->counter = 0;
		buttons->processed = false;
	}

}

/*
 * 描述：处理每个按钮是否被按下
 * 参数：无
 * 返回值：无
 */
static void precessButtons(){
	bool isPressed[BTNCOUNT];
	isPressed[BTN1] = !BTN_1;
	isPressed[BTN2] = !BTN_2;;
	isPressed[BTN3] = !BTN_3;

	for(uint8_t i = 0;i< BTNCOUNT;i++){
		precessButton(&button[i],isPressed[i]);
	}
}

/*
 * 描述：每隔10ms 判断所有按钮是否被按下
 * 参数：无
 * 返回值：无
 */
void ButtonUpdate(void){
	static uint32_t millsec = 0;
	uint32_t now = mills();

	if( (now - millsec) >= 10 ){
		millsec = now;
		precessButtons();
	}
}

