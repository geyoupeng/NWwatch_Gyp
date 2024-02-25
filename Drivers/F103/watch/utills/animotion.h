#ifndef __ANIMOTION_H
#define __ANIMOTION_H

#include "common.h"

extern anim_s animotion;

void AnimotionInto(void (*animOnComplete)(void));
void AnimotionLeave(void (*animOnComplete)(void));
bool AnimotionActive(void);
bool AnimotionMovingOn(void);
void AnimotionUpdata(void);
#endif //__ANIMOTION_H
