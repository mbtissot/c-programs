// C program to write contents
// to the file
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>


// Driver code
#define WIDTH 1536
#define HEIGHT 1536

#define NUM_SEEDS 16


struct Seed{
	int x;
	int y;
	int colors[3];
};

void generateSeeds(struct Seed seeds[]){
	int i;
	for(i=0; i<NUM_SEEDS; i++){
		seeds[i].x = (rand() % WIDTH);
		seeds[i].y = (rand() % HEIGHT);
	}
}

void printSeeds(struct Seed seeds[]){
	int i;
	for(i=0; i<NUM_SEEDS; i++){
		printf(" Seed %d x: %d, y: %d, colors: %d %d %d", i, seeds[i].x, seeds[i].y, seeds[i].colors[0], seeds[i].colors[1], seeds[i].colors[2]);
		printf("\n");
	}
}

float distance(int x1, int x2, int y1, int y2, double norm){
	double distance;
	double subx = abs(x1-x2);
	double suby = abs(y1-y2);
	double subdistance = pow(subx, (double) norm) + pow(suby, (double) norm);
	distance = pow(subdistance, 1/((double)norm));
	return distance;
}

void renderSeeds(int arrayIn[][WIDTH][3], struct Seed seeds[], int radius){
	int i, j, s;
	for(s=0; s<NUM_SEEDS; s++){
		for(i=0; i<HEIGHT; i++){
			for(j=0; j<WIDTH; j++){
				if(distance(i, seeds[s].y, j, seeds[s].x, 1.8)<=radius){
					arrayIn[i][j][0] = 0;
					arrayIn[i][j][1] = 0;
					arrayIn[i][j][2] = 0;
				}
			}
		}
	}
}

void writeHeader(){
	FILE* ptr;
	ptr = fopen("output.ppm", "w+");

	if(ptr==NULL){
		printf("Error here");
		exit(1);
	}
	fputs("P3\n", ptr);
	fprintf(ptr, "%d", HEIGHT);

	fputs(" ", ptr);

	fprintf(ptr, "%d", WIDTH);
	fputs("\n", ptr);
	
	fputs("255\n", ptr);
	fclose(ptr);
}

int minIndex(float * array, int arrayLen){
	int i;
	float min = 100000, minindex = -1;
	for(i=0; i<arrayLen; i++){
		if(array[i]<min){
			min = array[i];
			minindex = i;
		}
	}
	return minindex;
}

float min(float * array, int arrayLen){
	int i;
	float min = 100000;
	for(i=0; i<arrayLen; i++){
		if(array[i]<min){
			min = array[i];
		}
	}
	return min;
}

void calculateNeighbours(int arrayIn[][WIDTH][3], struct Seed seeds[]){
	printf("Entered calculate");
	int s, i, j;
	for(i=0; i<HEIGHT; i++){
		for(j=0; j<WIDTH; j++){
			float distances[NUM_SEEDS] = {0};
			for(s=0; s<NUM_SEEDS; s++){
				float distanceVal = distance(i, seeds[s].y, j, seeds[s].x, 2);
				distances[s] = distanceVal;
				int index = minIndex(distances, NUM_SEEDS);
				arrayIn[i][j][0] = seeds[index].colors[0];
				arrayIn[i][j][1] = seeds[index].colors[1];
				arrayIn[i][j][2] = seeds[index].colors[2];
			}
		}
	}
}


int generateColor(){
	int cor = rand() % 256;
	return cor;
}

void writeArray(int array[][WIDTH][3]){
	int i, j;
	FILE* ptr;
	ptr = fopen("output.ppm", "a+");

	if(ptr==NULL){
		printf("Error here");
		exit(1);
	}
	
	for(i=0; i<HEIGHT; i++){
		for(j=0; j<WIDTH; j++){
			fprintf(ptr, "%d", array[i][j][0]);
			fputs(" ", ptr);

			fprintf(ptr, "%d", array[i][j][1]);
			fputs(" ", ptr);

			fprintf(ptr, "%d", array[i][j][2]);
			fputs("\n", ptr);
		}
	}
}

int main(){

	struct Seed seeds[NUM_SEEDS];

	int static array[HEIGHT][WIDTH][3];

	srand(time(0));

	generateSeeds(seeds);
	// printSeeds(seeds);

	int i, j, s;
	for(s=0; s<NUM_SEEDS; s++){
		seeds[s].colors[0] = generateColor();
		seeds[s].colors[1] = generateColor();
		seeds[s].colors[2] = generateColor();
	}

	// printSeeds(seeds);

	calculateNeighbours(array, seeds);

	renderSeeds(array, seeds, 10);

	writeHeader();

	writeArray(array);

	return 0;
}