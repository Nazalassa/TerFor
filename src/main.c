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

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct content{
	char *author;
	int date;
	char *text;
};

struct post{
	struct content *content;
	struct post *previous;
	struct post *next;
};

typedef struct content content;
typedef struct post post;

char *renderPost(content *post, char type){
	char *r, *i, *j;
	if(type){
		r = malloc(sizeof(char));
		*r = 'A';
	}
	else{
		int sz = 3*sizeof(char) + sizeof(post->author) + sizeof(post->text);
		r = malloc(sz);
		*r = '[';
		for(i = r+1, j = post->author; i <= r+1+sizeof(post->author); i++, j++){
			*i = *j;
		}
		*i = ']';
		*(i+1)  = ' ';
		i += 2;
		for(j = post->text; i < r + sz; i++, j++){
			*i = *j;
		}
	}
	return(r);
}

int main(int argc, char *argv[]){
	FILE *launch = fopen("Data/launch.ascii", "r");
	int c;
	if(launch){
		while((c = getc(launch)) != EOF)
			printf("%c", c);
		fclose(launch);
	}
	content mypost;
	mypost.author = "Nazalassa", mypost.text = "HELLO!";
	char *toPrint = renderPost(&mypost, 0);
	printf("%s\n", toPrint);
	/**printf("TerFor  Copyright (C) 2022  Nazalassa");
	printf("This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.");
	printf("This is free software, and you are welcome to redistribute it");
	printf("under certain conditions; type `show c' for details.");**/
	
	return(0);
}
