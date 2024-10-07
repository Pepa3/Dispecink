#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <assert.h>

#define W 160
#define H 50
char* output;
int cx=0, cy = 0;
int selected = 5;
char sel(){
	assert(selected<6);
	return "-/|\\* "[selected];
}

  //mvchgat(25,80,-1, A_REVERSE, 0, NULL);

  //printw("Hello World !!!");
  //refresh();

void rtoolbar(){
  attron(A_BOLD);

  char toolbar[] = " [-](1) [/](2) [|](3) [\\](4) [*](5) [ ](6)";
  //2,9,16,23,30
  int x = 2+(selected*7);
  toolbar[x-1]='<';
  toolbar[x+1]='>';
  getyx(stdscr,cy,cx);
  mvprintw(0,W/2-strlen(toolbar)/2,"%s",toolbar);
  move(cy,cx);
  attroff(A_BOLD);
}

int main(int argc, char** argv){
  (void) argc;(void)argv;
  initscr();
  raw();
  noecho();
  keypad(stdscr,TRUE);
  mousemask(BUTTON1_PRESSED|BUTTON2_PRESSED,NULL);
  if(!has_colors()) return -1;
  start_color();
  output = malloc(W*H);
  memset(output,' ',W*H);
  
  init_pair(1,COLOR_WHITE,COLOR_BLACK);
  bkgd(COLOR_PAIR(1));

  attron(A_BOLD);
  move(H,0);
  hline('-',W);
  move(0,W);
  vline('|',H);
  attroff(A_BOLD);
  move(H/2,W/2);
  rtoolbar();
  int a;
  while((a=getch())!='q'){
    switch(a){
		case KEY_UP:
		  getyx(stdscr,cy,cx);
		  cy--;
		  move(cy,cx);
		  break;
		case KEY_DOWN:
		  getyx(stdscr,cy,cx);
		  cy++;
		  move(cy,cx);
		  break;
		case KEY_LEFT:
		  getyx(stdscr,cy,cx);
		  cx--;
		  move(cy,cx);
		  break;
		case KEY_RIGHT:
		  getyx(stdscr,cy,cx);
		  cx++;
		  move(cy,cx);
		  break;
		case '1':
		  selected = 0;
		  rtoolbar();
		  break;
		case '2':
		  selected = 1;
		  rtoolbar();
		  break;
		case '3':
		  selected = 2;
		  rtoolbar();
		  break;
		case '4':
		  selected = 3;
		  rtoolbar();
		  break;
		case '5':
		  selected = 4;
		  rtoolbar();
		  break;
		case '6':
		  selected = 5;
		  rtoolbar();
		  break;
		case KEY_MOUSE:
		  MEVENT event;
		  if(getmouse(&event)==OK){
		    if(event.bstate & BUTTON1_PRESSED){
		  	  move(event.y,event.x);
		  	  getyx(stdscr,cy,cx);
		  	}else if(event.bstate & BUTTON2_PRESSED){
		  	  output[cx+cy*W]=sel();
		  	  printw("%c",sel());
		      getyx(stdscr,cy,cx);
		  	}
		  }
		  break;
		case 10://ENTER
		  output[cx+cy*W]=sel();
		  printw("%c",sel());
		  getyx(stdscr,cy,cx);
		  break;
		default:
//		  printw("Unknown key %d",a);
		  break;
    }
	refresh();
  }
  endwin();
  printf("%s",output);
  free(output);
  return 0;
}
