// Tetris in C, on the cheap.
// Single-player, wiimote supported in under 300 lines.
// Jay Lang, 2019

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <ncurses.h>
#include <unistd.h>
#include "wiiuse.h"

#define ROWS 20
#define COLS 10
#define MAX_WIIMOTES 1
#define P1 wiimotes[0]

char tbl[ROWS][COLS] = {0};
int score = 0;
char liveGame = 1;
suseconds_t timer = 500000;

typedef struct {
  char **array;
  int width;      // non-rectangles anyway. who needs height?
  int row;        // refer to on-screen position
  int col;
} shape;
shape current;

/// Introducing: all of the tetris shapes
/// Actually got out a piece of paper to confirm these are right 
const shape shapesArrays[7] = {
  {(char *[]){(char []){0,1,1},(char []){1,1,0}, (char []){0,0,0}}, 3},                         
  {(char *[]){(char []){1,1,0},(char []){0,1,1}, (char []){0,0,0}}, 3},     
  {(char *[]){(char []){0,1,0},(char []){1,1,1}, (char []){0,0,0}}, 3},                       
  {(char *[]){(char []){0,0,1},(char []){1,1,1}, (char []){0,0,0}}, 3},                       
  {(char *[]){(char []){1,0,0},(char []){1,1,1}, (char []){0,0,0}}, 3},                      
  {(char *[]){(char []){1,1},(char []){1,1}}, 2},                                             
  {(char *[]){(char []){0,0,0,0}, (char []){1,1,1,1}, (char []){0,0,0,0}, (char []){0,0,0,0}}, 4}
};

/// Helper functions time!

/// copyOver: self explanatory, copies a shape
shape copyOver(shape oldDrawing) {
  int i, j;
  shape newDrawing = oldDrawing;
  char **toCopy = newDrawing.array;
  newDrawing.array = (char **)malloc(newDrawing.width*sizeof(char*));
  
  for (i = 0; i < newDrawing.width; i++) {
    newDrawing.array[i] = (char *)malloc(newDrawing.width*sizeof(char));
    for (j = 0; j < newDrawing.width; j++) {
      newDrawing.array[i][j] = toCopy[i][j];
    }
  }
  return newDrawing;
}

/// Yeet shapes out of existence by freeing rows
void deleteShape(shape deadDrawing) {
  int i;
  for (i = 0; i < deadDrawing.width; i++) {
    free(deadDrawing.array[i]);
  }
  free(deadDrawing.array);
}

/// Check the position of a given shape
int checkPos(shape drawing) {
  char **array = drawing.array;
  int i, j;
  for (i = 0; i < drawing.width; i++) {
    for (j = 0; j < drawing.width; j++) {
      
      /// OB check. In negative/too far over in either direction?
      if (drawing.col+j < 0 || drawing.col+j >= COLS || drawing.row+i >= ROWS) {
        if (array[i][j]) return 0;      // shortcut for false. remember, shapes aren't width-wide squares
  
      /// Is the space already occupied?
      } else if (tbl[drawing.row+i][drawing.col+j]) { 
        if (array[i][j]) return 0;
      }
    }
  }
  return 1; // true, good to go
}  
      
/// Make a random shape of the ones we've specified above
void mkShape() {
  shape newDrawing = copyOver(shapesArrays[rand()%7]); 
  newDrawing.row = 0;
  newDrawing.col = rand()%(COLS-newDrawing.width+1);    // 1 index vs. 0 index necessitates +1 
  deleteShape(current);                                 // replacement time
  current = newDrawing;

  /// If the shape can't be created, the game is OVER :'(
  if (!checkPos(current)) {
    liveGame = 0;
  }
}

/// Rotate a shape clockwise 90 degrees
void rotateShape(shape drawing) {
  shape tmp = copyOver(drawing);
  int i;
  int j;
  int k;
  int width = drawing.width;
  
  for (i = 0; i < width; i++) {
    int k = width-1;
    for (j = 0; j < width; j++){  
      drawing.array[i][j] = tmp.array[k][i];
      k--;
    }
  }
  deleteShape(tmp);
}

/// Writes a shape to the master table
void writeCurrentShape() {
  int i;
  int j;

  for (i = 0; i < current.width; i++) {
    for (j = 0; j < current.width; j++) {
      if (current.array[i][j]) {
        tbl[current.row+i][current.col+j] = current.array[i][j];
      }
    }
  }
}

/// Handles lower line impacts. And scores!
void lowerLine() {
  int i;
  int j;
  int sum;
  int count = 0;

  /// Figure out what's happening down there. Full row?
  for (i = 0; i < ROWS; i++) {
    sum = 0;
    for (j = 0; j < COLS; j++) {
      sum += tbl[i][j];
    }
    
    /// Full row completed handling - remember, can be more than one
    if (sum == COLS) {
      count++;                
      int k; int l;
      for (k = i; k >= 1; k--) {
        for (l = 0; l < COLS; l++) {
          tbl[k][l] = tbl[k-1][l];      // yeet yeet advance the things TODO TOCHECK
        }
//        for (l = 0; l < COLS; l++) {
//          tbl[k][l] = 0;
//        }
      }
    }
  }
  timer -= 1000; score += 100*count;  
}

/// Print out our current state of affairs
/// See above for this declaration - this is where draw primitives go!
void printTbl() {
  char buf[ROWS][COLS] = {0};                                     // current shape
  int i;
  int j;

  for (i = 0; i < current.width; i++) {
    for (j = 0; j < current.width; j++) {
      if (current.array[i][j]) {
        buf[current.row+i][current.col+j] = current.array[i][j];  // in-shape coordinates
      }
    }
  }
  clear();
  for (i = 0; i < ROWS; i++) {
    for (j = 0; j < COLS; j++) {
      printw("%c ", (tbl[i][j]+buf[i][j]) ? 'O': '.');         // in the permatable? in the current?
    }
    printw("\n");
  }
  printw("\nScore: %d\n", score);
  refresh();
}

void doCurrent(int action) {
  shape tmp = copyOver(current);
  switch(action) {
    case 's':             // TODO
      tmp.row++;
      if (checkPos(tmp)) current.row++;
      else {
        writeCurrentShape();
        lowerLine();
        mkShape();
      }
      break;
    case 'a':
      tmp.col--;
      if (checkPos(tmp)) current.col--;
      break;
    case 'd':
      tmp.col++;
      if (checkPos(tmp)) current.col++;
      break;
    case 'w':
      rotateShape(tmp);
      if (checkPos(tmp)) rotateShape(current);
      break;
    }
    deleteShape(tmp);
    printTbl();
}

/// Helper for main - timing is incredibly important
static inline int isLater(struct timeval pre, struct timeval post) {
  return ((suseconds_t)(post.tv_sec*1000000 + post.tv_usec) -((suseconds_t)pre.tv_sec*1000000 + pre.tv_usec)) > timer;
}

int main() {
  wiimote **wiimotes;
  int found, connected;
  wiimotes = wiiuse_init(MAX_WIIMOTES);
  printf("--- Welcome to Tetris! Grab your wiimotes... ---\n");
  printf("--- Press 1+2 to enter discovery mode. ---\n");

  found = wiiuse_find(wiimotes, MAX_WIIMOTES, 5);
  if (!found)  {
    printf("[MAIN-ERR] Discovery failed! Check your wiimote to be sure the LEDs are flashing.\n");
    return 0;
  }

  connected = wiiuse_connect(wiimotes, MAX_WIIMOTES);
  if (!connected) {
    printf("[MAIN-ERR] Connection failed! Feel free to re-run.\n");
    return 0;
  }

  printf("--- Connected! Loading Tetris now... ---\n");
  wiiuse_set_leds(P1, WIIMOTE_LED_1);
  wiiuse_rumble(P1, 1);
  usleep(300000);
  wiiuse_rumble(P1, 0);

  initscr();
  refresh();
  srand(time(0));
  score = 0;
  clear();

  struct timeval pre, post;
  gettimeofday(&pre, NULL);
  nodelay(stdscr, 1);
  struct timespec ts = {0, 1000000};      // 1ms
  timeout(1);

  mkShape();
  printTbl();

  int c;
  while (liveGame) {
    if (wiiuse_poll(wiimotes, MAX_WIIMOTES)) {
      switch (P1->event) {
        case WIIUSE_EVENT:
          if (IS_PRESSED(P1, WIIMOTE_BUTTON_LEFT)) doCurrent('a');
          if (IS_PRESSED(P1, WIIMOTE_BUTTON_RIGHT)) doCurrent('d');
          if (IS_PRESSED(P1, WIIMOTE_BUTTON_DOWN)) doCurrent('s');
          if (IS_PRESSED(P1, WIIMOTE_BUTTON_B)) doCurrent('w');
          if (IS_PRESSED(P1, WIIMOTE_BUTTON_A)) {
            printw("\n -- YOU HATH YEETED YOURSELF --\n");
            printw("-> Final score: %d\n", score);
            refresh();
            usleep(10000000);
            wiiuse_cleanup(wiimotes, MAX_WIIMOTES);
            endwin();
            return 0;
          }
          break;
        case WIIUSE_DISCONNECT:
        case WIIUSE_UNEXPECTED_DISCONNECT:
            printw("\n -- YOUR WIIMOTE DIED, F FOR RESPECTS --\n");
            refresh();
            usleep(10000000);
            wiiuse_cleanup(wiimotes, MAX_WIIMOTES);
            endwin();
            return -1;
            break;
      }
    }
    gettimeofday(&post, NULL);
    if (isLater(pre, post)) {
      doCurrent('s');
      gettimeofday(&pre, NULL);
    }
  }

  printw("\n ------ YOU ARE DED ------\n");
  printw("-> Final score: %d\n", score);
  refresh();
  usleep(10000000);
  wiiuse_cleanup(wiimotes, MAX_WIIMOTES);
  endwin();
  return 0;
}
