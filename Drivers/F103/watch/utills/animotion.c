#include "animotion.h"
#include "stm32f1xx_hal.h"
anim_s animotion = {0};

static void AnimotionStart(void (*animOnComplete)(void),bool active){
	animotion.active = true;
	animotion.animOnComplete = animOnComplete;
	animotion.goingOffScreen = active;
	animotion.offsetY = active?0:192;
}

void AnimotionInto(void (*animOnComplete)(void)){
	AnimotionStart(animOnComplete,false);
}

void AnimotionLeave(void (*animOnComplete)(void)){
	AnimotionStart(animOnComplete,true);
}

bool AnimotionActive(void){
	return animotion.active;
}

bool AnimotionMovingOn(void){
	return !animotion.goingOffScreen;
}

void AnimotionUpdata(void){
	uint8_t offsetY = 0;
	if(animotion.active){
		offsetY = animotion.offsetY;
		if(animotion.goingOffScreen){
			if(offsetY < 4)
			{
				offsetY = offsetY + 1;
			}
			else if(offsetY < 8)
			{
				offsetY = offsetY + 3;
			}
			else if(offsetY < 16)
			{
				offsetY = offsetY + 5;
			}
			else
			{
				offsetY = offsetY + 8;
			}
			if(offsetY > 64)
			{
				animotion.active = false;
				offsetY = 0;
			}
		}else{
			if(offsetY > 255 - 4)
				offsetY += 1;
			else if(offsetY > 255 - 8)
				offsetY += 3;
			else if(offsetY > 255 - 16)
				offsetY += 5;
			else
				offsetY += 8;

			if(offsetY < 10)
			{
				animotion.active = false;
				offsetY = 0;
			}
		}

		animotion.offsetY = offsetY;

		if(!AnimotionActive() && animotion.animOnComplete != NULL){
			animotion.animOnComplete();
			animotion.animOnComplete = NULL;
		}
	}
}

