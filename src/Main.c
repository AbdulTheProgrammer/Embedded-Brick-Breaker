#include <RTL.h>
#include "LPC17xx.h"                    /* LPC17xx definitions               */
#include "GLCD.h"
#include "LED.h"
#include "KBD.h"
#include "red_block.c"
#include "blue_block.c"
#include "green_block.c"
#include "ball.c"
#include "blank_block.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define __FI        1                   /* Font index 16x24                  */
#define PLAYER_HEIGHT 10
#define PLAYER_WIDTH  48
#define BLOCK_HEIGHT 24 
#define BLOCK_WIDTH 48
#define LCD_WIDTH 192
#define LCD_HEIGHT 320
#define PLAYER_SPEED 20

//OS RESOURCES
OS_TID t_jst   ;                        /* assigned task id of task: joystick */
OS_TID t_lcd;                           /* assigned task id of task: lcd     */
OS_TID t_ball;
OS_TID t_blocks;
OS_MUT mut_GLCD;
OS_MUT mut_Px;

//GLOBAL GAME STRUCTS
typedef struct BLOCK { 
	unsigned char * bitmap; 
	unsigned char alive; 
	signed int x; 
	signed int y;
} block;

typedef struct PLAYER { 
	unsigned char * bitmap; 
	unsigned char lives; 
	signed int x; 
	signed int prev_x;
	signed int y; 
} player; 

typedef struct BALL { 
	unsigned char *bitmap; 
	signed int x; 
	signed int y; 
	signed char speedX;
	signed char speedY; 
} game_ball; 


//GLOBAL VARIABLES
volatile OS_SEM kill_switch;
volatile OS_SEM game_end;
volatile OS_SEM stop_move; 
volatile int SCORE =0; 
volatile int NUMROWS = 3; 
volatile int NUMCOLS = 4;
volatile game_ball ball;
volatile player player1; 
volatile int player_moved = 0;
volatile int SPEED_LEVEL = 8;
volatile block blocks[5][4];  
volatile int time = 0;
volatile unsigned int update_count = 0;
volatile unsigned int score_change = 0;
volatile int win = 0; 
volatile int killed_blocks[3][2]; 
unsigned char* block_types[3]; 
unsigned int ADCStat = 0;
unsigned int ADCValue = 0;







/*
----------------------------------------------------------------------------------------
TASK 1: JOYSTICK TASK 
----------------------------------------------------------------------------------------
- POLLS FOR CHANGES IN THE JOYSTICK 
- MOVES PLAYER'S PADDLE ACCORDINGLY
NEUTRAL POSITION = 121
LEFT POSITION = 113
RIGHT POSITION = 89
----------------------------------------------------------------------------------------
*/
__task void joystick (void) {
	unsigned char pos;
  while(1) {
			if(time < 80000) { 
				time++; 
			}
			pos = KBD_Get();
			//left movement
			if(pos == 113 && player1.x > 0 && !player_moved) {
				player1.prev_x = player1.x;
				player1.x -= PLAYER_SPEED;
				player_moved = 1;
			}
			//right movement
			if(pos == 89 && player1.x + PLAYER_WIDTH + PLAYER_SPEED <= LCD_WIDTH && !player_moved) {
				player1.prev_x = player1.x;
				player1.x += PLAYER_SPEED;
				player_moved = 1;
			}
  }
}
/*
----------------------------------------------------------------------------------------
HELPER FUNCTION: CHECK COLLISIONS 
----------------------------------------------------------------------------------------
- CHECKS FOR COLLISIONS WITH ANY OF THE BLOCKS AND THE BALL
----------------------------------------------------------------------------------------
*/
void check_collisions (unsigned char * reverse_x, unsigned char * reverse_y) { 
		int i = 0; 
		int j = 0;
		int ki = 0;
		char update = 0;
		int num_of_alive_blocks = 0;
		for(i = 0 ;i < 3;i++ ) {
			for(j =0;j< 2; j++) {
				killed_blocks[i][j] = -1;
			}
		}			
		for(i =0;i<NUMROWS;i++) { 
			for(j=0;j<NUMCOLS;j++) {
				if(blocks[i][j].alive) { 
					num_of_alive_blocks++; 
				}
				//detect collision with block 
				if(blocks[i][j].alive && (ball.x >= blocks[i][j].x - BALL_WIDTH && ball.x + BALL_WIDTH <= blocks[i][j].x + BLOCK_WIDTH + BALL_WIDTH) && (ball.y + BALL_HEIGHT  >= blocks[i][j].y && ball.y + BALL_HEIGHT <= blocks[i][j].y + BLOCK_HEIGHT || ball.y <= blocks[i][j].y + BLOCK_HEIGHT && ball.y >= blocks[i][j].y)){ 
					//detect if ball collidied with the side edge of the block  
					if(ball.y + BALL_HEIGHT > blocks[i][j].y && ball.y < blocks[i][j].y + BLOCK_HEIGHT) { 								
								killed_blocks[ki][0] = i; 
								killed_blocks[ki][1] = j;
								ki++;
								blocks[i][j].alive = 0;
								blocks[i][j].bitmap = BLANK_BLOCK_PIXEL_DATA;
							 *reverse_x = 1; 
								SCORE += SPEED_LEVEL;
								score_change =1;
								update = 1;
					}
					//detect if ball collided with bottom or top of block 
					if((ball.y +BALL_HEIGHT > blocks[i][j].y || ball.y < blocks[i][j].y + BLOCK_HEIGHT) && ball.x + BALL_WIDTH > blocks[i][j].x && ball.x < blocks[i][j].x + BLOCK_WIDTH) {
							killed_blocks[ki][0] = i; 
							killed_blocks[ki][1] = j;
							ki++;
							blocks[i][j].alive = 0;
							blocks[i][j].bitmap = BLANK_BLOCK_PIXEL_DATA;
							*reverse_y = 1;
							SCORE += SPEED_LEVEL; 
							score_change = 1;
							update = 1;
					}
				}
			}
		}
		if(!num_of_alive_blocks) { 
			win = 1; 
		}
		//signal rerender blocks task
		if(update) {
			os_sem_send(&kill_switch);
			os_sem_wait(&stop_move, 0xffff);
		}			 
}
/*
----------------------------------------------------------------------------------------
HElPER FUNCTION: CONVERT INTEGER TO C STRING 
----------------------------------------------------------------------------------------
- SELF EXPLANATORY
----------------------------------------------------------------------------------------
*/
void convert_to_cstring(int a, char score_str[]) {
	//int integer = a;
	int rem; 
	char character;
	int i = 0;
	int j =0;
	char temp[4];	
	while (a != 0) { 
		rem = a % 10; 
		a = a /10; 
		character = rem + '0';  
		temp[i] = character;
		i++;		
	} 
	for (j=0;j<i;j++) { 
		score_str[i-j-1] = temp[j]; 
	}
	score_str[i] = '\0';
}

/*
----------------------------------------------------------------------------------------
HElPER FUNCTION: DRAW SCORE 
----------------------------------------------------------------------------------------
- SELF EXPLANATORY
----------------------------------------------------------------------------------------
*/
void draw_score(int x,int y) {
	char score_str[4];
	int index = 0;
	convert_to_cstring(SCORE, score_str); 
	while(score_str[index] != '\0') { 
		GLCD_DisplayChar (y, index+x, __FI, score_str[index]);
		 index++; 
	}
	if(index ==0) { 
		GLCD_DisplayChar (y, x, __FI, '0');
	}
}

/*
----------------------------------------------------------------------------------------
TASK 2: BALL MOVE TASK 
----------------------------------------------------------------------------------------
- DRAWS NEW POSITION OF THE PLAYER AND ERASES OLD POSITION  
- DRAWS NEW POSITION OF THE BALL AND ERASES OLD POSITION
----------------------------------------------------------------------------------------
*/

__task void ball_move (void) { 
	unsigned char reverse_x = 0 ; 
	unsigned char reverse_y = 0; 
	while(1) {
			if(score_change) { 
				draw_score(9,9); 
				score_change = 0;
			}
			
			// update count controls the speed of the ball's movement
		 if(update_count == 1<<((8-SPEED_LEVEL)*3)) {
			update_count = 0;
			check_collisions(&reverse_x, &reverse_y);
			
			// game end conditions
			if(ball.y <= 0 || win) { 
				os_sem_send(&game_end);
			}
			if (ball.x+BALL_WIDTH >= LCD_WIDTH || ball.x <= 0 || reverse_x)
			{
					ball.speedX *= -1;
			}
			if((ball.x + BALL_WIDTH >= player1.x && ball.x <= player1.x +PLAYER_WIDTH && ball.y <= player1.y + PLAYER_HEIGHT)|| reverse_y || ball.y+BALL_HEIGHT >= LCD_HEIGHT) { 
					ball.speedY *= -1;
			}
			//erase after images of ball
			if(ball.speedY >0 ) { 
				GLCD_Bitmap(ball.y, ball.x, 1,BALL_WIDTH , BLANK_BLOCK_PIXEL_DATA );
			}
			else { 
				GLCD_Bitmap(ball.y+BALL_HEIGHT-1, ball.x, 1, BALL_WIDTH,  BLANK_BLOCK_PIXEL_DATA );
			}
			if(ball.speedX >0) { 
				GLCD_Bitmap(ball.y, ball.x, BALL_HEIGHT, 1,BLANK_BLOCK_PIXEL_DATA );
			}
			else { 
				GLCD_Bitmap(ball.y, ball.x+BALL_WIDTH-1, BALL_HEIGHT,1, BLANK_BLOCK_PIXEL_DATA );
			}
			ball.x += ball.speedX;
			ball.y += ball.speedY;
			reverse_x = 0; 
			reverse_y = 0;
			GLCD_Bitmap(ball.y, ball.x, BALL_WIDTH , BALL_HEIGHT, ball.bitmap); 
		}
		//corner case to move the ball with the paddle if it's speed is very low
		if (player_moved) { 
			if(player1.x > player1.prev_x) {
				//delete after image of player
				GLCD_Bitmap(player1.y, player1.prev_x,PLAYER_HEIGHT, PLAYER_SPEED, BLANK_BLOCK_PIXEL_DATA);
				if(ball.y < player1.y + PLAYER_HEIGHT && ball.x <= player1.x +PLAYER_WIDTH && ball.x + BALL_WIDTH >= player1.x + PLAYER_WIDTH) { 
					if(ball.x+BALL_WIDTH + PLAYER_SPEED > LCD_WIDTH) { 
						player1.x = LCD_WIDTH-BALL_WIDTH-PLAYER_WIDTH; 
					}
					GLCD_Bitmap(ball.y, ball.x, BALL_WIDTH , BALL_HEIGHT, BLANK_BLOCK_PIXEL_DATA); 
					ball.x = player1.x +PLAYER_WIDTH; 
					GLCD_Bitmap(ball.y, ball.x, BALL_WIDTH , BALL_HEIGHT, ball.bitmap);
					ball.speedX *= 1;
				}
			}
			if(player1.x < player1.prev_x) { 
				//delete after image of player
				GLCD_Bitmap(player1.y, player1.prev_x+PLAYER_WIDTH-PLAYER_SPEED,PLAYER_HEIGHT, PLAYER_SPEED, BLANK_BLOCK_PIXEL_DATA);
				if(ball.y < player1.y + PLAYER_HEIGHT && ball.x <= player1.x && ball.x + BALL_WIDTH >= player1.x) { 
					if(ball.x-PLAYER_SPEED < 0) { 
						player1.x = BALL_WIDTH; 
					}
					GLCD_Bitmap(ball.y, ball.x, BALL_WIDTH , BALL_HEIGHT, BLANK_BLOCK_PIXEL_DATA); 
					ball.x = player1.x - BALL_WIDTH;
					GLCD_Bitmap(ball.y, ball.x, BALL_WIDTH , BALL_HEIGHT, ball.bitmap); 
					ball.speedX *= -1;
				}
			}
			//render player paddle
			GLCD_Bitmap(player1.y, player1.x,PLAYER_HEIGHT, PLAYER_WIDTH, player1.bitmap);
			player_moved = 0;
		}
		update_count++;
	}
}

/*
----------------------------------------------------------------------------------------
TASK 3: UPDATE BLOCKS TASK
----------------------------------------------------------------------------------------
- DETERMINE WHICH BLOCKS WERE KILLED  
- ERASE KILLED BLOCKS
----------------------------------------------------------------------------------------
*/

__task void update_blocks (void) {
	while(1) { 
		int i; 
		os_sem_wait(&kill_switch, 0xFFFF); 
			for(i =0;i<3;i++) { 
				if(killed_blocks[i][0] != -1 && killed_blocks[i][1] != -1 ) {
						GLCD_Bitmap(blocks[killed_blocks[i][0]][killed_blocks[i][1]].y, blocks[killed_blocks[i][0]][killed_blocks[i][1]].x, BLOCK_HEIGHT, BLOCK_WIDTH, blocks[killed_blocks[i][0]][killed_blocks[i][1]].bitmap);
				}
				else {
					break;
				}
			}
			os_sem_send(&stop_move);
		}
}

/*
----------------------------------------------------------------------------------------
HARDWARE INTERRUPT: INT0
----------------------------------------------------------------------------------------
- DETECTS BUTTON PRESSES ON FALLING EDGE WITH DEBOUNCING USING TIME COUNTER
- CHANGES THE SPEED OF THE BALL'S MOVEMENT WITH SPEED LEVEL ALONG WITH THE SCORE INCREMENTOR
----------------------------------------------------------------------------------------
*/
void EINT3_IRQHandler(void) {  
	int i;
	if(time > 80000-1) {
		LED_Off (SPEED_LEVEL-1);
		if(SPEED_LEVEL == 1 ) { 
			SPEED_LEVEL = 8;
		}
		else { 
			SPEED_LEVEL--;
		}
		if(SPEED_LEVEL == 8) { 
			for(i = 0; i<8; i++) { 
				LED_On (i);
			}
		}
		time =0;
	}
	update_count = 0;
	LPC_GPIOINT->IO2IntClr |= (1 << 10);
}
/*
----------------------------------------------------------------------------------------
HELPER FUNCTION: INIT_GAME 
----------------------------------------------------------------------------------------
- INITIALIZES THE STRUCTS AND RENDERS THE STARTING POSITIONS OF EACH GAME OBJECT
----------------------------------------------------------------------------------------
*/

void init_game () {
	int i = 0; 
	int j = 0;
	char* msg = "SCORE: ";
	GLCD_Clear(White); 
	for (i = 0 ; i<13; i++) { 
		GLCD_Bitmap(i*24, 192,24, 5, RED_BLOCK_PIXEL_DATA); 
	}
	for(i =0;i<6;i++) {
		GLCD_DisplayChar (9, i+2, __FI, msg[i]);
	}
	draw_score(9,9);
	ball.x = 120; 
	ball.y = 100;
	ball.speedX = 1;
	ball.speedY = 1;
	player1.x = 0; 
	player1.y = 0; 
	player1.bitmap = BLUE_BLOCK_PIXEL_DATA; 
	player1.lives = 1; 
	GLCD_Bitmap(player1.y, player1.x,PLAYER_HEIGHT, PLAYER_WIDTH, player1.bitmap); 
	//intializing the postion of each of the blocks and drawing them onto the screen
	for(i =1;i<=NUMROWS;i++) { 
		for(j=0;j<NUMCOLS;j++) { 
			blocks[i-1][j].x = BLOCK_WIDTH*j;
			blocks[i-1][j].y = LCD_HEIGHT-BLOCK_HEIGHT*i; 
			blocks[i-1][j].alive = 1; 
			blocks[i-1][j].bitmap = block_types[(j+i)%3];
			GLCD_Bitmap(blocks[i-1][j].y, blocks[i-1][j].x, BLOCK_HEIGHT, BLOCK_WIDTH, blocks[i-1][j].bitmap);
		}
	}

}
/*
----------------------------------------------------------------------------------------
TASK 4: INITIALIZING TASK
----------------------------------------------------------------------------------------
- HIGHEST PRIORITY TASK (10) 
- CHANGES LEVEL OF THE GAME BASED ON WIN LOSE CONDITIONS
- CREATES ALL OTHER TASKS
----------------------------------------------------------------------------------------
*/

__task void init (void) {
	int reset = 0;
	int i;
	U32 delay = 1<<25; 
  os_mut_init(mut_GLCD);
	os_mut_init(mut_Px);
	os_sem_init(&kill_switch, 0);
	os_sem_init(&stop_move, 0);
	os_sem_init(&game_end, 0);
	for(i = 0; i<8; i++) { 
		LED_On (i);
	}
	block_types[0] = RED_BLOCK_PIXEL_DATA; 
	block_types[1] = GREEN_BLOCK_PIXEL_DATA; 
	block_types[2] = BLUE_BLOCK_PIXEL_DATA; 		
	ball.bitmap = BALL_PIXEL_DATA; 
	init_game();
	os_tsk_prio_self(10);
	t_ball  = os_tsk_create (ball_move, 0); 
	t_jst		= os_tsk_create (joystick, 0);
	t_blocks = os_tsk_create (update_blocks, 0);
	while(1) {  
		if(!reset) {
			os_sem_wait(&game_end,0xffff);
			GLCD_Clear(White); 
			GLCD_SetTextColor(Black);
			if(win) { 
				GLCD_DisplayString(3, 7, __FI, "WINNER! ");
				GLCD_DisplayString(4, 8, __FI, "SCORE");
				draw_score(9,5);
				while(delay) { 
					delay--;
				}
				delay = 1<<25;
				NUMROWS++;
				init_game();
				win = 0;
				os_sem_wait(&game_end,0xffff);
				GLCD_Clear(White); 
				if(win) { 
					GLCD_DisplayString(3, 7, __FI, "WINNER! ");
					GLCD_DisplayString(4, 8, __FI, "SCORE");
					draw_score(9,5);
					while(delay) { 
						delay--;
					}
					delay = 1<<25; 
					NUMROWS++;
					init_game();
					win = 0;
					os_sem_wait(&game_end,0xffff);
					GLCD_Clear(White); 
					if(win) { 
						GLCD_DisplayString(3, 7, __FI, "WINNER! ");
						GLCD_DisplayString(4, 8, __FI, "SCORE");
						draw_score(9,5);
						while(delay) { 
							delay--;
						}
						delay = 1<<25; 
						}
					else {
						GLCD_DisplayString(3, 6, __FI, "GAME OVER ");
						GLCD_DisplayString(4, 8, __FI, "SCORE");
						draw_score(9,5);
					}
				}
				else {
					GLCD_DisplayString(3, 6, __FI, "GAME OVER ");
					GLCD_DisplayString(4, 8, __FI, "SCORE");
					draw_score(9,5);
				}
			}
			else {
				GLCD_DisplayString(3, 6, __FI, "GAME OVER ");
				GLCD_DisplayString(4, 8, __FI, "SCORE");
				draw_score(9,5);
			}
			reset =1; 
		}
	}
}


/*----------------------------------------------------------------------------
  Main: Initialize and start RTX Kernel
 *---------------------------------------------------------------------------*/
int main (void) {
  LED_Init ();                              /* Initialize the LEDs           */
  GLCD_Init();                              /* Initialize the GLCD           */
	KBD_Init ();                              /* initialize Push Button        */

  GLCD_Clear(White);                        /* Clear the GLCD                */
  os_sys_init(init);                        /* Initialize RTX and start init */
}
