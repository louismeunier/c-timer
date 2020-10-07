#include <stdlib.h>
#include <stdio.h>
#include <time.h>


char moves[6] = {'R','L','U','D','F','B'};

void generate_basic_scramble() {
   	int index,choice;
   	char prev = 'A', curr, type;
   	char scramble[48];
    srand(time(0)); 
  
 	for (int i=0;i<48;i++) {
 		if (i%2==0)
 		{
		index = rand()%6;
		curr = moves[index];
  		if (curr==prev)
  			i--;
  		else
  			scramble[i] = curr;
  			prev = curr;
 		}  	
 		else 
 		{
 			choice = rand()%3;
  			switch (choice) {
  				case 0:
  					type = '\'';
  					break;
  				case 1:
  					type = '2';
  					break;
  				case 2:
  					type = ' ';
  					break;
  			}
  			scramble[i] = type;
 		}
 	}
  	for (int j = 0;j<48;j++){
  		if (scramble[j]!=' ')
  	  		printf("%c", scramble[j]);
  	}

}