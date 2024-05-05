#include <stdio.h>
#include <gb/gb.h>
#include <rand.h>
#include <time.h>

#include "graphic.h"


#define MAX_SHOOT 3
#define MAX_ASTEROIDS 11
#define ASTEROID_COLLISION_BOX 10
#define SHOOTING_DELAY 5
#define DELAY_BETWEEN_ASTEROIDS 8


typedef struct Shoot{
	UINT8 visible;
	int posX, posY;
	UINT8 t;
	UINT8 gr;
	
} Shoot;

typedef struct Actor{
	UINT8 life;
	int posX, posY;
	UINT8 t;
	
} Actor;


UBYTE z;
UBYTE shoot = 0;

UBYTE i;
UBYTE j;
UBYTE p;

UINT8 shootingDelay;
UINT8 delayBetweenAsteroids;


Shoot shoots[MAX_SHOOT];
Actor player;
Actor asteroids[MAX_ASTEROIDS];

UBYTE b_push = 0;
UBYTE input;


void initGameDatas() {
	for(i = 0; i < MAX_ASTEROIDS; i++) {
		asteroids[i].life = 0;
	}
	player.life = 1;
	player.t = 0;
	j = 0; 
	p = 0;
	shootingDelay = SHOOTING_DELAY;
	player.posY = 140;
	player.posX = 80;
	
}

void initShoots() {
	for(i = 0; i < 3; i+=1)
	{
		shoots[i].visible = 0;
		shoots[i].posY = player.posY;
		shoots[i].posX = player.posX;
	}

	shoots[0].t = 2;
	shoots[1].t = 4;
	shoots[2].t = 6;

	shoots[0].gr = 8;
	shoots[1].gr = 9;
	shoots[2].gr = 11;
}

void clearBkgAndWin() {
  DISPLAY_OFF;

  for(i = 0; i < 30; i++){
    for(j = 0; j < 30; j++){
      set_bkg_tiles(i, j, 1, 1, 0x0E);
    }
  }

  for(i = 0; i < 30; i++){
    for(j = 0; j < 30; j++){
    set_win_tiles(i, j, 1, 1, 0x0E);
    }
  }

  move_bkg(0,0);
  DISPLAY_ON;
}

void showBenjaminMetautLogo() {
  	set_bkg_data(0, 14, BenMetTiles);
 	set_win_tiles(0, 0, 9, 4, BenMetLogo);
 	move_win(SCREENWIDTH/2 - (3*8), SCREENHEIGHT/2 - (2*8)); //HUD
}

void showBootScreen() {
	showBenjaminMetautLogo();

  	SHOW_BKG;
  	SHOW_WIN;
  	DISPLAY_ON;

  	delay(2000);
  	wait_vbl_done();
}

void showStartScreen()
{	 
	HIDE_WIN;
	HIDE_SPRITES;
	clearBkgAndWin();

	initGameDatas();
	set_bkg_data(0, 32, titleBkg);
	set_bkg_tiles(0,0, 20,20,titleTiles);

	
	do{
		if(joypad() == J_A)
		{
			showRulesScreen();
		}	
	}while(joypad() != J_START);

}

void showRulesScreen()
{
	set_bkg_data(0, 36, rulesBkg);

	set_bkg_tiles(0,0, 20,20,rulesTiles);
	
	waitpad(J_START);

	set_bkg_data(0, 32, titleBkg);

	set_bkg_tiles(0,0, 20,20,titleTiles);
	
}

void showGameScreen() {
	set_bkg_data(0, 2, spaceBkg);
	set_bkg_tiles(0, 0, 20, 35, spaceTiles);
	
	SPRITES_8x16;

	drawPlayer();
	initShoots();

	
	NR52_REG = 0x80;
	NR51_REG = 0x11;
	NR50_REG = 0x77;

	SHOW_SPRITES;
	while(player.life > 0) {
		input = joypad();
		handleGameScreenInputs();
		checkCollision();
		drawPlayer();
		drawShoots();
		checkShoot();
		drawAsteroid();
		move_bkg(0, z);
		z--;
		delay(25);
	}
}

void drawAsteroid()
{
	UINT16 seed = LY_REG;
	seed |= (UINT16)DIV_REG << 8;
	initrand(seed);

	for(i = 0; i < MAX_ASTEROIDS; i++)
	{
		if(asteroids[i].life > 0)
		{
			if(asteroids[i].posY <= 150)
			{
				
				asteroids[i].posY += 2; 
				
				move_sprite(asteroids[i].t, asteroids[i].posX, asteroids[i].posY);
			}
			else
			{
				asteroids[i].life = 0;	
			}
		}
		else
		{
			asteroids[i].t = 18+(i*2);
			set_sprite_data(asteroids[i].t, 2, asteroidSprite);
			set_sprite_tile(asteroids[i].t,asteroids[i].t);
			set_sprite_tile(asteroids[i].t+1,asteroids[i].t+1);
			move_sprite(asteroids[i].t, asteroids[i].posX, asteroids[i].posY);
			move_sprite(asteroids[i].t, asteroids[i].posX, 0);

			if(delayBetweenAsteroids <= 0) {
				delayBetweenAsteroids = DELAY_BETWEEN_ASTEROIDS;
				asteroids[i].life = 1;

				asteroids[i].posY = 0;
				asteroids[i].posX = (rand() % 150);

			}
		}
	}

	delayBetweenAsteroids--;
}

void drawShoots(){
	for(i = 0; i < MAX_SHOOT; i++) {
		if(shoots[i].visible == 1) {
			if(shoots[i].posY > 0) {
				shoots[i].posY = shoots[i].posY - 10; 
				move_sprite(shoots[i].t+1, shoots[i].posX, shoots[i].posY);
				move_sprite(shoots[i].gr, 500, 500);
			} else {
				shoots[i].visible = 0;
			}
		} else {
			shoots[i].visible = 0;
			set_sprite_data(shoots[i].gr, 1, munition);
			set_sprite_tile(shoots[i].gr,shoots[i].gr);
			
			//move_sprite(shoots[i].t+1, shoots[i].posX, shoots[i].posY);
			move_sprite(shoots[i].gr, 142+(i*8), 8);
		}
	}	
	
}

void drawPlayer() {
	set_sprite_data(player.t, player.t+2, playerSprite);

	set_sprite_tile(player.t, player.t);
	set_sprite_tile(player.t+1, player.t+1);
	move_sprite(player.t, player.posX, player.posY); // je positione le sprite de l'adresse 0
}

int checkCollision()
{
	//Check borders collision
	if(player.posX <= 8)
	{
		player.posX = 8;
		player.posY = player.posY;
		move_sprite(player.t, player.posX, player.posY);
	}
	if(player.posX >= 160)
	{
		player.posX = 160;
		player.posY = player.posY;
		move_sprite(player.t, player.posX, player.posY);
	}
	if(player.posY  <= 18)
	{
		player.posX = player.posX;
		player.posY = 18;
		move_sprite(player.t, player.posX, player.posY);
	}
	if(player.posY >= 140)
	{
		player.posX = player.posX;
		player.posY = 140;
		move_sprite(player.t, player.posX, player.posY);
	}

	//Check collision with asteroids
	for(i = 0; i < MAX_ASTEROIDS; i++) {
		if(asteroids[i].life == 1) {
			if(asteroids[i].posX+ASTEROID_COLLISION_BOX >= player.posX && asteroids[i].posX-ASTEROID_COLLISION_BOX <= player.posX
				&& asteroids[i].posY+ASTEROID_COLLISION_BOX >= player.posY && asteroids[i].posY-ASTEROID_COLLISION_BOX <= player.posY) {
				player.life = 0;
			}
			for(j = 0; j < MAX_SHOOT; j++) {
				if(asteroids[i].posX+ASTEROID_COLLISION_BOX >= shoots[j].posX && asteroids[i].posX-ASTEROID_COLLISION_BOX <= shoots[j].posX
					&& asteroids[i].posY+ASTEROID_COLLISION_BOX >= shoots[j].posY && asteroids[i].posY-ASTEROID_COLLISION_BOX <= shoots[j].posY) {
					asteroids[i].life = 0;
					asteroids[i].posX = -30;
					shoots[j].visible = 0;
					shoots[j].posX = -50;
					move_sprite(shoots[j].t+1, shoots[j].posX, shoots[j].posY);
				}
			}
		}
	}
	
}

void checkShoot() {
	if(!shootingDelay)
	{
		if(shoot != 0)
		{
			for(i = 0; i < MAX_SHOOT; i++) {
				if(shoots[i].visible == 0) {
					shoots[i].posX = player.posX;
					shoots[i].posY = player.posY;
					shoots[i].visible = 1;

					set_sprite_data(shoots[i].t, 2, shootSprite);
					
					set_sprite_tile(shoots[i].t, shoots[i].t);
					set_sprite_tile((shoots[i].t)+1, (shoots[i].t)+1);
				
					NR10_REG = 0x1F;
					NR11_REG = 0xF1;
					NR12_REG = 0x83;
					NR13_REG = 0xFF;
					NR14_REG = 0x87;
					
					shootingDelay = SHOOTING_DELAY;
					break;
				}
			}
		}
		
		shoot = 0;
	}
	else shootingDelay--;
}


void handleGameScreenInputs() {

	if(joypad()==J_RIGHT) {
		player.posX +=5;
	}

	if(joypad()==J_LEFT) {
		player.posX-=5;
	}

	if(joypad()==J_UP) {
		player.posY-=5;
	}
	
	if(joypad()==J_DOWN) {
		player.posY+=5;
	}

	if(joypad() == J_START)
	{
		waitpad(J_START);
	}
	
	if((input & J_B) && (b_push == 0)){
		shoot = 1;
		b_push = 1;		
	} else {
		b_push = 0;
		shoot = 0;
	}
}


int main(){

	SHOW_BKG;
	SPRITES_8x8;	
	clearBkgAndWin();

  	showBootScreen();


	while(1) {	
		showStartScreen();
		showGameScreen();
	}


	return 0;

}
