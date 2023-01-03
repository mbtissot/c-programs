//@author: mbtissot
//@version: jan/2023

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>


// Defining some parameters. You can change these as you wish, as long as they are integers bigger than 1.
#define WIDTH 512
#define HEIGHT 768

#define NUM_SEEDS 8 // Defining how many seeds you want to be scattered on the image.

#define SEED_RADIUS 5 // Ideally, in a square image, this should be ~1% of the square side.
					  // so, in a square image of 512x512, a radius of ~5 is ideal.

struct Seed{
	/* Struct defining each seed. Each seed has a (x, y) coordinate and a color (made up from RGB values)*/
	int x;
	int y;
	int colors[3];
};

int generateColor(){
	/* Function that generates a color in RGB space. Could be removed and each color generated on the
	"generateSeeds" function, but i have other plans for this function :D */
	int cor = rand() % 256;
	return cor;
}

void generateSeeds(struct Seed seeds[]){
	/* This function receives an array of Seed, and generates positions (x, y) for all of them, as well
	   as colors for each of them.*/
	int i;
	for(i=0; i<NUM_SEEDS; i++){
		seeds[i].x = (rand() % WIDTH);
		seeds[i].y = (rand() % HEIGHT);

		seeds[i].colors[0] = generateColor();
		seeds[i].colors[1] = generateColor();
		seeds[i].colors[2] = generateColor();
	}
}

void printSeeds(struct Seed seeds[]){
	/* This function is used mostly for debugging. It prints all the seeds.*/
	int i;
	for(i=0; i<NUM_SEEDS; i++){
		printf(" Seed %d x: %d, y: %d, colors: %d %d %d", i, seeds[i].x, seeds[i].y, seeds[i].colors[0], seeds[i].colors[1], seeds[i].colors[2]);
		printf("\n");
	}
}

float distance(int x1, int x2, int y1, int y2, double norm){
	/* This functions return the distances between the points (x1, y1) and (x2, y2).
	   The "norm" parameter changes the 'type of distance' (in case you want to use some other definition
	   besides the 2-norm). 1.8 seems to work best if you want to draw circles.*/
	double distance;
	double subx = abs(x1-x2);
	double suby = abs(y1-y2);
	double subdistance = pow(subx, (double) norm) + pow(suby, (double) norm);
	distance = pow(subdistance, 1/((double)norm));
	return distance;
}

void renderSeeds(int arrayIn[][WIDTH][3], struct Seed seeds[], int radius){
	/* This function renders each seed with a certain radius, so that we can see exactly where they are.
	   I'm thinking of a good reason for each seed having its own radius, instead of a global radius,
	   but haven't got any good ideas yet. So in the future i might change it.*/
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
	/* This functions write the header of the .ppm file.*/
	FILE* ptr;
	ptr = fopen("output.ppm", "w+");

	if(ptr==NULL){
		printf("Error here");
		exit(1);
	}
	fputs("P3\n", ptr);
	fprintf(ptr, "%d", WIDTH);

	fputs(" ", ptr);

	fprintf(ptr, "%d", HEIGHT);
	fputs("\n", ptr);
	
	fputs("255\n", ptr);
	fclose(ptr);
}

int minIndex(float * array, int arrayLen){
	/* This functions return the index of the minimum value in an array of floats. */
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
	/* This function returns the minimum value of inside an array of floats.
	   Currently unused, but i have some plans for it. */
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
	/* This function defines the color of each pixel, based on the seed closest to said pixel. */
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

void writeArray(int array[][WIDTH][3]){
	/* This function writes the array of pixels to the .ppm file.*/
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

	struct Seed seeds[NUM_SEEDS]; // Creating array of seeds

	int static array[HEIGHT][WIDTH][3]; // Creating array of pixels

	srand(time(0)); // Initializing random seed

	generateSeeds(seeds); // Generating seeds

	calculateNeighbours(array, seeds); // Calculating neighbours

	renderSeeds(array, seeds, SEED_RADIUS); // Rendering seeds

	writeHeader(); // Writing header to .ppm file

	writeArray(array); // Writing array of pixels to .ppm file

	return 0;
}