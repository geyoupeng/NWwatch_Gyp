#include "common.h"
#include "drivers.h"
#include <stdlib.h>
#include <string.h>
#define PLATFORM_WIDTH	12
#define PLATFORM_HEIGHT	4
#define UPT_MOVE_NONE	0
#define UPT_MOVE_RIGHT	1
#define UPT_MOVE_LEFT	2
#define BLOCK_COLS		32
#define BLOCK_ROWS		5
#define BLOCK_COUNT		(BLOCK_COLS * BLOCK_ROWS)
#define FRAME_WIDTH 	128
#define FRAME_HEIGHT 	64

typedef struct{
	float x;
	float y;
	float velX;
	float velY;
}s_ball;

static const uint8_t block[] ={
	0x07,0x07,0x07,
};

static const uint8_t platform[] ={
	0x60,0x70,0x50,0x10,0x30,0xF0,0xF0,0x30,0x10,0x50,0x70,0x60,
};

static const uint8_t ballImg[] ={
	0x03,0x03,
};
	
static uint8_t uptMove;
static s_ball ball;
static bool blocks[BLOCK_COUNT] = {0};
static uint8_t lives;
static uint16_t score;
static uint8_t platformX;
static bool Game1Left(){
	uptMove = UPT_MOVE_LEFT;
	return true;
}

static bool Game1Right(){
	uptMove = UPT_MOVE_RIGHT;
	return true;
}

static bool Game1Exit(){
	if(lives == 255){
        Game1Start();    
    }
	else
	{ 
		AnimotionLeave(back);
	}
	return true;
}

static void PlatformMoving(){
	uint8_t platformXtmp = platformX;

	// Move platform
	if(uptMove == UPT_MOVE_RIGHT)
		platformXtmp += 3;
	else if(uptMove == UPT_MOVE_LEFT)
		platformXtmp -= 3;
	uptMove = UPT_MOVE_NONE;

	// Make sure platform stays on screen
	if(platformXtmp > 250)
		platformXtmp = 0;
	else if(platformXtmp > FRAME_WIDTH - PLATFORM_WIDTH)
		platformXtmp = FRAME_WIDTH - PLATFORM_WIDTH;

	// Draw platform
	OledShowImage(platformXtmp, FRAME_HEIGHT - 8, 12, 8, platform, 0);
	platformX = platformXtmp;

}

static void Game1Draw(){
	bool gameEnded = ((score >= BLOCK_COUNT) || (lives == 255));

	PlatformMoving();
	// Move ball
	if(!gameEnded)
	{
		ball.x += ball.velX;
		ball.y += ball.velY;
	}

	bool blockCollide = false;
	const float ballX = ball.x;
	const uint8_t ballY = ball.y;

	// Block collision  
	uint8_t idx = 0;
	for(uint8_t x = 0;x<BLOCK_COLS;x++)
	{
		for(uint8_t y = 0;y<BLOCK_ROWS;y++)
		{
			if(!blocks[idx] && ballX >= x * 4 && ballX < (x * 4) + 4 && ballY >= (y * 4) + 8 && ballY < (y * 4) + 8 + 4)
			{
				BuzzerBuzz(100,TONE_2KHZ,VOL_UI,PRIO_UI,NULL);
				//led_flash(LED_GREEN, 50, 255);
				blocks[idx] = true;
				blockCollide = true;
				score++;
			}
			idx++;
		}
	}

	// Side wall collision
	if(ballX > FRAME_WIDTH - 2)
	{
		if(ballX > 240)
			ball.x = 0;		
		else
			ball.x = FRAME_WIDTH - 2;
		ball.velX = -ball.velX;		
	}
	if(ballX < 0)
	{
		ball.x = 0;		
		ball.velX = -ball.velX;	
	}

	// Platform collision
	bool platformCollision = false;
	if(!gameEnded && ballY >= FRAME_HEIGHT - PLATFORM_HEIGHT && ballY < 240 && ballX >= platformX && ballX <= platformX + PLATFORM_WIDTH)
	{
		platformCollision = true;
		BuzzerBuzz(200,TONE_5KHZ,VOL_UI,PRIO_UI,NULL);
		ball.y = FRAME_HEIGHT - PLATFORM_HEIGHT;
		if(ball.velY > 0)
			ball.velY = -ball.velY;
		ball.velX = ((float)rand() / (RAND_MAX / 2)) - 1; // -1.0 to 1.0
	}

	// Top/bottom wall collision
	if(!gameEnded && !platformCollision && (ballY > FRAME_HEIGHT - 2 || blockCollide))
	{
		if(ballY > 240)
		{
			BuzzerBuzz(200,TONE_2_5KHZ,VOL_UI,PRIO_UI,NULL);
			ball.y = 0;
		}
		else if(!blockCollide)
		{
			BuzzerBuzz(200,TONE_2KHZ,VOL_UI,PRIO_UI,NULL);
			ball.y = FRAME_HEIGHT - 1;
			lives--;
		}
		ball.velY *= -1;
	}

	// Draw ball
	OledShowImage(ball.x, ball.y,2, 8, ballImg, 0);

	// Draw blocks
	idx = 0;
	for(uint8_t x = 0;x<BLOCK_COLS;x++)
	{
		for(uint8_t y = 0;y<BLOCK_ROWS;y++)
		{
			if(!blocks[idx])
				OledShowImage(x * 4, (y * 4) + 8,3, 8, block, 0);
			idx++;
		}
	}

	// Draw score
	char buff[6];
	sprintf(buff, PSTR("%u"), score);
	OledShowStr(0, 0,buff,OLED_8X8);

	// Draw lives
	if(lives != 255)
	{
		for(uint8_t i = 0;i<lives;i++)
			OledShowImage((FRAME_WIDTH - (3*8)) + (8*i), 0,  7, 8, livesImg, 0);
	}	

	// Got all blocks
	if(score >= BLOCK_COUNT)
		OledShowStr(50, 32,PSTR(STR_WIN),OLED_16X8);

	// No lives left (255 because overflow)
	if(lives == 255)
		OledShowStr(34, 32,PSTR(STR_GAMEOVER), OLED_16X8);

}

void Game1Start(void){
	SetDrawFunc(Game1Draw);
	SetButtonFunc(Game1Left, Game1Exit, Game1Right);
	
	uptMove = UPT_MOVE_NONE;
    
    memset(blocks,false,sizeof(blocks));
	ball.x = FRAME_WIDTH / 2;
	ball.y = FRAME_HEIGHT - 10;
	ball.velX = -0.5;
	ball.velY = -0.6;
	
	lives = 3;
	score = 0;
	platformX = (FRAME_WIDTH / 2) - (PLATFORM_WIDTH / 2);
}

