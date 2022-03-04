#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <time.h>
#include <stdlib.h>
#include <graphics.h>
#include <string> 

#define ScreenW 1000
#define ScreenH 690
#define OptionsW 300
#define OptionsH 690
#define tSize 80
#define MaxBullets 30
#define MaxEnemies 10
#define EnemyStep 2

void GameOver();
int PlayerEnemyCollision(int dir, int x, int y);
int HitBullet();
void EnemiesCollide();
void DrawEnemies();
void GenerateEnemies();
void GenerateBullet(int dir, int x, int y);
void DrawBullet();
int HitBorder(int dir, int x, int y);
void DrawFrame();
void Move(int dir, int x, int y);
void Forward(int x, int y, int isEnemy=0);
void Backward(int x, int y, int isEnemy=0);
void Right(int x, int y, int isEnemy=0);
void Left(int x, int y, int isEnemy=0);

int bullets[MaxBullets][3];
int bIndex = 0;

int enemies[MaxEnemies][3];
int eIndex = 0;

int score = 0;
char txtScore[3] = "0";

int main() {
  //random no generation
  srand( (unsigned)time(NULL) );
  
  initwindow(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), "" );
  
  int x = ScreenW/2 - 250, y = 550;  // Player Position
  int dir = 1;  // Player Direction : 1-forward, 2-right, 3-backward, 4-left
  int step = 5; // Player Step
  char option;
  
  for(int i=0; i<MaxBullets; i++){						
  	bullets[i][0] = bullets[i][1] = bullets[i][2] = 0;	
  }
  
  for(int i=0; i<MaxEnemies; i++){
  	enemies[i][0] = enemies[i][1] = enemies[i][2] = 0;
  }
  
  DrawFrame();											//Function for playing frame
  GenerateEnemies();									//Function to generate random position for enemies
  
  while(1){
  	 setcolor(GREEN);					//Color of score card
  	 settextstyle(DEFAULT_FONT, 0, 3);
     outtextxy(ScreenW + 30, 20, (char*)"Tank Game");
     
     settextstyle(DEFAULT_FONT, 0, 4);
     outtextxy(ScreenW + 60, 150, (char*)"Score");
  	  
  	 settextstyle(DEFAULT_FONT, 0, 9);
     outtextxy(ScreenW + 100, 250, txtScore);
     
     settextstyle(DEFAULT_FONT, 0, 3);
     outtextxy(ScreenW + 20, 470, (char*)"Controls");
     
     settextstyle(SANS_SERIF_FONT, 0, 2);
     outtextxy(ScreenW + 20, 520, (char*)"A - Rotate Anti Clockwise");
     outtextxy(ScreenW + 20, 550, (char*)"S - Move Forward");
     outtextxy(ScreenW + 20, 580, (char*)"D - Rotate Clockwise");
     outtextxy(ScreenW + 20, 610, (char*)"Spacebar - Fire");
  	 
  	 if(kbhit()){
  	 	option = getch();
  	 	switch(option){
  	 		case 'a':
  	 			dir--;
  	 			if(dir<1) dir = 4;
  	 			break;
  	 		case 'd':
  	 			dir++;
  	 			if(dir>4) dir = 1;
  	 			break;
  	 		case 's':
  	 			if(dir==1){
 				   if( HitBorder(dir, x, y) != 1 )
 				     y-=step;
  	 			}
  	 			else if(dir==2){
  	 			   if( HitBorder(dir, x, y) != 1 )
  	 			     x+=step;
  	 			} 
  	 			else if(dir==3){
  	 			   if( HitBorder(dir, x, y) != 1 )
  	 			   	 y+=step;	
  	 			}
  	 			else{
 				   if( HitBorder(dir, x, y) != 1 )
  	 				  x-=step;
  	 			}
  	 			break;
  	 		case 32:
  	 			GenerateBullet(dir,x,y);
  	 			break;
  	 	}
  	 }
  	 DrawEnemies();
     HitBullet();
  	 DrawBullet();
     Move(dir, x, y);
     
     if( PlayerEnemyCollision(dir, x, y) == 1){
     	GameOver();
     	break;
     }
     
	 delay(20);	
	 cleardevice();
	 DrawFrame();
  }
    
  getch();
  closegraph();
  return 0;

    delay(2000);
    cleardevice();
    
    settextstyle(SANS_SERIF_FONT, 0, 3);
    outtextxy(200, 200, (char*)"Game Over");
}

int PlayerEnemyCollision(int dir, int x, int y){
	// Check Collision of player tank with enemy
	for( int j=0; j<MaxEnemies; j++ ){
		
		if( x >= enemies[j][0] && x <= enemies[j][0]+tSize ){
			if( y >= enemies[j][1] && y <= enemies[j][1]+tSize ){
				return 1;
			}
		}
		
		if( x+tSize >= enemies[j][0] && x+tSize <= enemies[j][0]+tSize ){
			if( y >= enemies[j][1] && y <= enemies[j][1]+tSize ){
				return 1;
			}
		}
		
		if( x+tSize >= enemies[j][0] && x+tSize <= enemies[j][0]+tSize ){
			if( y+tSize >= enemies[j][1] && y+tSize <= enemies[j][1]+tSize ){
				return 1;
			}
		}
		
		if( x >= enemies[j][0] && x <= enemies[j][0]+tSize ){
			if( y+tSize >= enemies[j][1] && y+tSize <= enemies[j][1]+tSize ){
				return 1;
			}
		}
		 
	}
	return 0;
}

void GameOver(){	
    delay(2000);
    cleardevice();
    
    settextstyle(SANS_SERIF_FONT, 0, 7);
    outtextxy(200, 200, (char*)"Game Over");
    
    settextstyle(SANS_SERIF_FONT, 0, 3);
    outtextxy(200, 400, (char*)"Press any key to continue...");
    
    getch();
}

int HitBullet(){
	for(int i=0; i<=bIndex; i++){ 
		if( bullets[i][0] != 0 && bullets[i][1] != 0 ){
			for( int j=0; j<MaxEnemies; j++ ){
				if( bullets[i][0] >= enemies[j][0] && bullets[i][0] <= enemies[j][0] + tSize ){
					if( bullets[i][1] >= enemies[j][1] && bullets[i][1] <= enemies[j][1] + tSize ){

						enemies[j][0] = 5 + rand()%800;
						enemies[j][1] = 5 + rand()%500;
						
						score++;
						if( score > 9 ){
							txtScore[0] = (score/10)+48; 
							txtScore[1] = (score%10)+48; 	
						}else{
							txtScore[0] = '0'; 
							txtScore[1] = score+48; 
						}
						
						return 1;
					}
				}
			}	
		} 
	}
	return 0;
}

void EnemiesCollide(){					//Checks if two enemies are colliding
	for( int i=0; i<MaxEnemies; i++ ){
		for( int j=0; j<MaxEnemies; j++ ){
			if ( i!=j ){
				if( enemies[i][2] == 1 ){
					if( enemies[i][1] < enemies[j][1]+tSize ){
						if( enemies[i][0] > enemies[j][0] && enemies[i][0] < enemies[j][0]+tSize ){
							enemies[i][2] = 1 + rand()%4;
						}
					}
				}else if( enemies[i][2] == 2 ){
					if( enemies[i][0] > enemies[j][0]+tSize ){
						if( enemies[i][1] > enemies[j][1] && enemies[i][1] < enemies[j][1]+tSize ){
							enemies[i][2] = 1 + rand()%4;
						}
					}
				}else if( enemies[i][2] == 3 ){
					if( enemies[i][1] > enemies[j][1]+tSize ){
						if( enemies[i][0] > enemies[j][0] && enemies[i][0] < enemies[j][0]+tSize ){
							enemies[i][2] = 1 + rand()%4;
						}
					}
				}else {
					if( enemies[i][0] < enemies[j][0]+tSize ){
						if( enemies[i][1] > enemies[j][1] && enemies[i][1] < enemies[j][1]+tSize ){
							enemies[i][2] = 1 + rand()%4;
						}
					}
				}
			}
		}
	}
}

void GenerateEnemies(){ 					//Function to generate enemy position at random
	for(int i=0; i<MaxEnemies; i++){
		enemies[i][0] = 5 + rand()%800;		//X co- ordinate of enemy position
		enemies[i][1] = 5 + rand()%500;		//Y co- ordinate of enemy position
		enemies[i][2] = 1 + rand()%4;		//direction of enemy tank barrel
		if( i > 0 ){
			for( int j=0; j<=i; j++ ){		//In case two enemy collide during first random position generation
				if( enemies[i][0] > enemies[j][0] && enemies[i][0]+tSize < enemies[j][0]+tSize ){
					if( enemies[i][1] > enemies[j][1] && enemies[i][1]+tSize < enemies[j][1]+tSize ){
						enemies[i][0] = 5 + rand()%800;
						enemies[i][1] = 5 + rand()%500;
					}	
				}
			}
		}
	}
}

void DrawEnemies(){ 
    setcolor(RED);					///Inner circle of enemy tank
	for(int i=0; i<MaxEnemies; i++){
		
		if( enemies[i][2] == 1 ){		//If direction is forward
			enemies[i][1]-=EnemyStep;	//y- axis decrement
			if( enemies[i][1] - tSize * 0.5 < 1 ){	//For extreme top
				enemies[i][2] = 1 + rand()%4;		//Generate another direction
			}
		}else if( enemies[i][2] == 2){				//If direction is right
			enemies[i][0]+=EnemyStep;				//x- axis increment
			if( enemies[i][0] + tSize * 1.5 > ScreenW ){	//When tank reaches almost right most part
				enemies[i][2] = 1 + rand()%4;				//Generate another direction
			}
		}else if( enemies[i][2] == 3){						//Similar for backward and left part
			enemies[i][1]+=EnemyStep;
			if( enemies[i][1] + tSize * 1.5 > ScreenH ){
				enemies[i][2] = 1 + rand()%4;
			}
		}else if( enemies[i][2] == 4){
			enemies[i][0]-=EnemyStep;
			if( enemies[i][0] - tSize * 0.5 < 1 ){
				enemies[i][2] = 1 + rand()%4;
			}
		}
	    
	    if( enemies[i][0] == 0 && enemies[i][1] == 0 )	//If x, y= 0, 0 , continue	
	    	continue;
	    
	    EnemiesCollide();
	    
		if( enemies[i][2] == 1 ){					//If direction is 1
			Forward(enemies[i][0],enemies[i][1],1);	//move forward
		}else if( enemies[i][2] == 2 ){				//If direction is 2
			Right(enemies[i][0],enemies[i][1],1);	//move right
		}else if( enemies[i][2] == 3 ){				//If direction is 3
			Backward(enemies[i][0],enemies[i][1],1);	//move backward
		}else if( enemies[i][2] == 4 ){					//If direction is 4
			Left(enemies[i][0],enemies[i][1],1);		//move left
		}
		
	}
}

void GenerateBullet(int dir, int x, int y){
	int x1, y1;
	if( dir == 1 ){
		x1 = x + (tSize/2)+(tSize/15);
		y1 = y - tSize * 0.5;
	}
	else if( dir == 2 ){
		x1 = x + tSize * 1.5;
		y1 = y + tSize/2 + (tSize/15);
	}
	else if( dir == 3 ){
		x1 = x + (tSize/2) + (tSize/15);
		y1 = y + tSize * 1.5;
	}
	else if( dir == 4 ){
		x1 = x - tSize * 0.5;
		y1 = y + tSize/2 + (tSize/15);
	}
	
	bullets[bIndex][0] = x1;
	bullets[bIndex][1] = y1;
	bullets[bIndex][2] = dir;
	bIndex++;
	if( bIndex >= MaxBullets ){
		bIndex = 0;
	}
         
	circle(x1,y1,5);
}

void DrawBullet(){
    setcolor(GREEN);
	for(int i=0; i<=bIndex; i++){
		if( bullets[i][2] == 1 ){
			bullets[i][1]-=5;
			if( bullets[i][1] < 1 ){
				bullets[i][0] = 0;
				bullets[i][1] = 0;
			}
		}else if( bullets[i][2] == 2){
			bullets[i][0]+=5;
			if( bullets[i][0] > ScreenW ){
				bullets[i][0] = 0;
				bullets[i][1] = 0;
			}
		}else if( bullets[i][2] == 3){
			bullets[i][1]+=5;
			if( bullets[i][1] > ScreenH ){
				bullets[i][0] = 0;
				bullets[i][1] = 0;
			}
		}else if( bullets[i][2] == 4){
			bullets[i][0]-=5;
			if( bullets[i][0] < 1 ){
				bullets[i][0] = 0;
				bullets[i][1] = 0;
			}
		}
		
		if( bullets[i][0] != 0 && bullets[i][1] != 0 )
			circle(bullets[i][0], bullets[i][1], 5);
	}
}

int HitBorder(int dir, int x, int y){
	int scX1 = 0, scY1 = 5, scX2 = ScreenW, scY2 = 5 + ScreenH;
	int x1, y1, x2, y2;
	
	if( dir == 1 ){
		y1 = y - tSize * 0.5;
		if( y1 <= scY1 )
		   return 1; 
	}
	else if( dir == 2 ){
		x2 = x + tSize * 1.5;
		if( x2 >= scX2 )
			return 1;
	}
	else if( dir == 3 ){
		y2 = y + tSize * 1.5;
		if( y2 >= scY2 )
		   return 1; 
	}
	else if( dir == 4 ){
		x1 = x - tSize * 0.5;
		if( x1 <= scX1 )
			return 1;
	}
	return 0;
}

void DrawFrame(){
  setlinestyle(SOLID_LINE, 1, 2);
  setcolor(WHITE);
  rectangle(0,10,ScreenW,ScreenH-20);
  rectangle(ScreenW,10,ScreenW + OptionsW,OptionsH);
}

void Move(int dir, int x, int y){
    setcolor(WHITE);
	if( dir == 1 )
		Forward(x,y);
	if( dir == 2 )
		Right(x,y);
	if( dir == 3 )
		Backward(x,y);
	if( dir == 4 )
		Left(x,y);	
	
}

void Forward(int x, int y, int isEnemy){
	 int clr = 9;
	 if(isEnemy)
	    clr = 4;
		 
	 setfillstyle(8, clr);
	 //Tank Body
	 bar(x, y, x + tSize, y + tSize);
	 
	 setfillstyle(11, clr);
	 //Tank Barrel
	 bar(x + (tSize/2)-(tSize/15), y, x + (tSize/2)+(tSize/15), y - (tSize/2));
	 
	 //Tank Top Cover
     setlinestyle(SOLID_LINE, 15, 0);
	 setfillstyle(3, getmaxcolor());
	 fillellipse( x+tSize/2, y+tSize/2, tSize/4, tSize/4 );
}

void Backward(int x, int y, int isEnemy){
	 int clr = 9;
	 if(isEnemy)
	    clr = 4;
		 
	 setfillstyle(8, clr);
	 //Tank Body
	 bar(x, y, x + tSize, y + tSize);
	 
	 setfillstyle(11, clr);
	 //Tank Barrel 
	 bar(x + (tSize/2)-(tSize/15), y + tSize, x + (tSize/2)+(tSize/15), y + (tSize*1.5));
	 
	 //Tank Top Cover
  	 setlinestyle(SOLID_LINE, 15, 0);
	 setfillstyle(3, getmaxcolor());
	 fillellipse( x+tSize/2, y+tSize/2, tSize/4, tSize/4 ); 
}

void Right(int x, int y, int isEnemy){
	 int clr = 9;
	 if(isEnemy)
	    clr = 4;
		 
	 setfillstyle(8, clr);
	 //Tank Body
	 bar(x, y, x + tSize, y + tSize);
	 
	 setfillstyle(11, clr);
	 //Tank Barrel   
	 bar(x + tSize, y + tSize/2 - (tSize/15), x + tSize * 1.5, y + tSize/2 + (tSize/15));
	 
	 //Tank Top Cover
     setlinestyle(SOLID_LINE, 15, 0);
	 setfillstyle(3, getmaxcolor());
	 fillellipse( x+tSize/2, y+tSize/2, tSize/4, tSize/4 );
}

void Left(int x, int y, int isEnemy){ 
	 int clr = 9;
	 if(isEnemy)
	    clr = 4;
		 
	 setfillstyle(8, clr);
	 //Tank Body
	 bar(x, y, x + tSize, y + tSize);
	 
	 setfillstyle(11, clr);
	 //Tank Barrel   
	 bar(x , y + tSize/2 - (tSize/15), x - tSize * 0.5, y + tSize/2 + (tSize/15));
	 
	 //Tank Top Cover
     setlinestyle(SOLID_LINE, 15, 0);
	 setfillstyle(3, getmaxcolor());
	 fillellipse( x+tSize/2, y+tSize/2, tSize/4, tSize/4 );
}


