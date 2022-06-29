/** 
TerFor   --   Forums for terminal
Copyright (C) 2022  Nazalassa

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
**/

// COMPILATION: gcc -o tf-a tf-a.c -lncursesw
// --------------------------------------- HEADERS ------------------------------------------

#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// --------------------------------------- STRUCTS ------------------------------------------

struct thread{
  char *content;
  struct thread *next;
};
typedef struct thread THREAD;

struct metathread{
  THREAD *thread;
  int len;
};
typedef struct metathread MTHREAD;

// --------------------------------- GLOBALS AND CONSTANTS ----------------------------------



// ---------------------------------- FUNCTION PROTOTYPES -----------------------------------

void displayPosts(MTHREAD*, WINDOW*);

void addPost(MTHREAD*, char*, char*);

void displayFile(WINDOW*, char*);

// ---------------------------------- FUNCTION DEFINITIONS ----------------------------------

void displayPosts(MTHREAD *postList, WINDOW *postWin){
  THREAD *reading;
  reading = postList->thread;
  short int winX, winY, lines, linesDisplayed;
  getmaxyx(postWin, winY, winX);
  short int currentY = winY;
  wclear(postWin);
  while(reading != NULL && linesDisplayed < winY){
    lines = strlen(reading->content) / winX + 1;
    linesDisplayed += lines, currentY -= lines;
    mvwprintw(postWin, currentY, 0, reading->content);
    reading = reading->next;
  }
  wrefresh(postWin);
}

void addPost(MTHREAD *mainThread, char *post, char *author){
  THREAD *newPost;
  newPost = malloc(sizeof(THREAD));
  newPost->content = (char*)malloc(strlen(author) + strlen(post) + 4);
  strcpy(newPost->content, "[");
  strcat(newPost->content, author);
  strcat(newPost->content, "] ");
  strcat(newPost->content, post);
  newPost->next = mainThread->thread;
  mainThread->thread = newPost;
  mainThread->len++;
}

void displayFile(WINDOW *win, char *file){
  FILE *f = fopen(file, "rb");
  if(f){
    fseek(f, 0, SEEK_END);
    long pos = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *f_array = malloc(pos);
    fread(f_array, pos, 1, f);
    fclose(f);
    waddstr(win, f_array);
    free(f_array);
    wrefresh(win);
    wgetch(win);
  }
}


char *usrs[10] = {"Jebediah", "Valentina", "Jebediah", "KSC", "Jebediah", "KSC", "Valentina", "Jebediah", "KSC", "Valentina"};
char *psts[10] = {"So we're getting near Duna.", "Yup! I can't wait until the landing! When is the maneuver?", "Uh, I don't really know. KSC, when is the maneuver?", "Well, in a few hours.", "Only? I thought we'd have more time.", "Well, we can't really delay a Periapsis Burn, you know.", "Anyway, the guys down on the surface surely wait us. That'll be cruel to make them wait!", "Sure! How long will it take until we're on the ground of Duna?", "Maybe seven hours, not more.", "Yay! Thanks, KSC. I'm waiting for the burn."};


// --------------------------------------- MAIN ---------------------------------------------

int main(int argc, char **argv){
  setlocale(LC_ALL, "");
  initscr();
  raw();
  noecho();
  keypad(stdscr, TRUE);
  // END OF CURSES INITIALIZATION
  // ...
  // WINDOW INITIALIZATION
  short int termX, termY;
  getmaxyx(stdscr, termY, termX);
  char numWrite = termY < 25 ? 4 : termY / 6;
  char postWriteY = termY - numWrite;
  char numPosts = termY - numWrite - 1;
  WINDOW *input, *postArea, *separator, *startwin;
  input = newwin(postWriteY, termX, numPosts + 1, 0), postArea = newwin(numPosts, termX, 0, 0), separator = newwin(1, termX, numPosts, 0);
  wmove(separator, 0, 0);
  for(int i = 0; i < termX; i++) waddch(separator, '=');
  startwin = newwin(24, 80, termY / 2 - 12, termX / 2 - 40);
  // END OF WINDOW INITIALIZATION
  // ...
  // STARTUP
  displayFile(startwin, "Data/launch.ascii");
  // END OF STARTUP
  // ...
  wrefresh(separator);
  MTHREAD *mainThread;
  mainThread = malloc(sizeof(MTHREAD));
  mainThread->len = 0, mainThread->thread = NULL;
  displayPosts(mainThread, postArea);
  for(int i = 0; i < 10; i++){
    wgetch(input);
    addPost(mainThread, psts[i], usrs[i]);
    displayPosts(mainThread, postArea);
  }
  // ...
  // SHUTTING DOWN
  delwin(postArea);
  delwin(separator);
  delwin(input);
  endwin();
  return(0);
}
