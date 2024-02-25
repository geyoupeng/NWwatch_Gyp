#include "common.h"
#include "drivers.h"
#include <stdlib.h>
#include <string.h>

#define UPT_MOVE_NONE	0
#define UPT_MOVE_UP		1
#define UPT_MOVE_DOWN	2

#define CAR_COUNT	3
#define CAR_WIDTH	12
#define CAR_LENGTH	15
#define ROAD_SPEED	6
#define FRAME_WIDTH 	128
#define FRAME_HEIGHT 	64
typedef struct {
//	bool ok;
	uint8_t x;
	uint8_t y;
	uint8_t speed;
}s_otherCars;

typedef struct {
	bool hit;
	uint8_t lane;
	uint8_t y;
}s_myCar;

static const uint8_t carImg[] ={
	0x40,0xF8,0xEC,0x2C,0x2C,0x38,0xF0,0x10,0xD0,0x30,0xE8,0x4C,0x4C,0x9C,0xF0,
	0x02,0x1F,0x37,0x34,0x34,0x1C,0x0F,0x08,0x0B,0x0C,0x17,0x32,0x32,0x39,0x0F,
};

static const uint8_t roadMarking[] ={
	0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
};

static uint8_t highscore;
static uint8_t score;
static uint8_t uptMove;
static uint8_t lives;
static s_otherCars cars[CAR_COUNT];

static bool Game2Up(){
	uptMove = UPT_MOVE_UP;
	return true;
}

static bool Game2Down(){
	uptMove = UPT_MOVE_DOWN;
	return true;
}

static bool Game2Exit(){
	if(lives == 255){
        Game2Start();    
    }
	else
	{ 
		AnimotionLeave(back);
	}
	return true;
}

static void Game2Draw(){
	static s_myCar myCar;
	static uint32_t hitTime;
	static bool newHighscore;

	// Change lane
	if(uptMove == UPT_MOVE_UP && myCar.lane < 3)
		myCar.lane++;
	else if(uptMove == UPT_MOVE_DOWN&& myCar.lane > 0)
		myCar.lane--;
	uptMove = UPT_MOVE_NONE;

	// Move to new lane
	uint8_t y = myCar.lane * 16;
	if(myCar.y > y)
		myCar.y -= 2;
	else if(myCar.y < y)
		myCar.y += 2;

	
	if(lives != 255)
		{
			// Move cars
			for(uint8_t i = 0;i <CAR_COUNT ;i++)
			{
				// Move car
				cars[i].x -= cars[i].speed;
	
				// Gone off screen
				if(cars[i].x > 200 && cars[i].x < 255 - 16)
				{
					cars[i].x = FRAME_WIDTH;
					cars[i].y = (rand() % 4) * 16;
					score++;
				}
			}
	
			// Stop cars from going through each other
			// Its a little bit glitchy
			for(uint8_t i = 0;i <CAR_COUNT ;i++)
			{
				for(uint8_t c = 0;c <CAR_COUNT ;c++)
				{
					if(i != c && cars[i].y == cars[c].y && cars[i].x > cars[c].x && cars[i].x < cars[c].x + CAR_LENGTH)
					{
						cars[i].x = cars[c].x + CAR_LENGTH + 1;
						//cars[i].speed = cars[c].speed;
					}					
				}
			}

			// Collision
			if(!myCar.hit)
			{
				for(uint8_t i = 0;i <CAR_COUNT ;i++)
				{
					if(cars[i].x < CAR_LENGTH)
					{
						uint8_t carY = cars[i].y + 2;
						uint8_t myCarY = myCar.y + 2;
						if((carY >= myCarY && carY <= myCarY + CAR_WIDTH) || (carY + CAR_WIDTH >= myCarY && carY <= myCarY))
						{
							myCar.hit = true;
							hitTime = mills();
							lives--;
							if(lives == 255)
							{
								// Check for new highscore
								if(score > highscore)
								{
									newHighscore = true;
									highscore = score;
								}
								else
									newHighscore = false;
									BuzzerBuzz(250, TONE_2KHZ, VOL_UI, PRIO_UI,NULL);
							}	
							else
							{
	
								BuzzerBuzz(100, TONE_2KHZ, VOL_UI, PRIO_UI,NULL);
							}
						}
					}
				}
			}
		}

	uint32_t now = mills();

	if(myCar.hit && now - hitTime >= 1000)
		myCar.hit = false;

	// Quake
	static int8_t quakeY;
	if(myCar.hit && now - hitTime < 350)
	{
	if(quakeY == 2)
		quakeY = -2;
	else
		quakeY = 2;
	}
	else
	quakeY = 0;

	// Draw my car
	if(!myCar.hit || (myCar.hit && (now & 64)))
		OledShowImage(0, myCar.y + quakeY, 15, 16, carImg, 0);

	char buff[6];
	if(lives != 255)
	{
		// Draw other cars
		for(uint8_t i = 0;i <CAR_COUNT ;i++)
			OledShowImage(cars[i].x, cars[i].y + quakeY, 15, 16, carImg, 0);

		// Draw road markings
		static uint8_t dotX[3] = {0, 45, 90};
		for(uint8_t i = 0;i <3 ;i++)
		{
			dotX[i] -= ROAD_SPEED;

			if(dotX[i] >= FRAME_WIDTH && dotX[i] < 255 - 8)
				dotX[i] = FRAME_WIDTH;

			for(uint8_t x = 0;x <3 ;x++)
				OledShowImage(dotX[i], (x * 16) + quakeY + 16, 8, 8, roadMarking, 0);
		}

		// Draw score
		sprintf(buff, PSTR("%u"), score);
		OledShowStr(FRAME_WIDTH - (7*5), 1,buff, OLED_8X8);

		// Draw lives
		for(uint8_t i = 0;i <lives ;i++)
			OledShowImage(32 + (8*i), 1, 7, 8, livesImg, 0);
	}
	else
	{
		// Draw end game stuff

		OledShowStr(20, 0,PSTR(STR_GAMEOVER),OLED_16X8);
		OledShowStr(20, 16,PSTR(STR_SCORE), OLED_16X8);
		OledShowStr(20, 32,PSTR(STR_HIGHSCORE), OLED_16X8);

		if(newHighscore)
			OledShowStr(0, 48,PSTR(STR_NEWHIGHSCORE), OLED_16X8);

		sprintf(buff, PSTR("%u"), score);
		OledShowStr(96, 16,buff, OLED_16X8);

		sprintf(buff, PSTR("%u"), highscore);
		OledShowStr(104, 32,buff, OLED_16X8);
	}
}

void Game2Start(void){
	SetDrawFunc(Game2Draw);
	SetButtonFunc(Game2Up, Game2Exit, Game2Down);

	lives = 3;
	for(uint8_t i = 0;i <CAR_COUNT ;i++)
	{
		cars[i].y = i * 16;
		cars[i].speed = i + 1;
		cars[i].x = FRAME_WIDTH;
	}	
}


