//Tal Dayan - 207378639
//Amit Neeman - 207388794

#include "header.h"

//This function gets IMG_POS_LIST array and size
//The function creates image from type GREY_IMAGE that all pixels in eash segments from IMG_POS_LIST array with have the same grey level value
//The grey level value calculated depend on the amount of segments (size) 
//For example - grey level value of segment i will be (i*(255\size-1)) rounded down
//Return img in GREY_IMAGE type 
GRAY_IMAGE* colorSegments(IMG_POS_LIST* segments, unsigned int size) {
	int i;
	GRAY_IMAGE* img = (GRAY_IMAGE*)malloc(sizeof(GRAY_IMAGE));
	checkMemoryAllocate(img);
	countRowsAndCols(segments, size, &(img->rows), &(img->cols));
	img->pixels = (char**)malloc(img->rows * sizeof(char*));
	for (i = 0; i < img->rows; i++) {
		img->pixels[i] = (char*)malloc(img->cols * sizeof(char));
		checkMemoryAllocate(img->pixels[i]);
	}
	fillPixels(segments, size, img->pixels);
	return img;

}
//This function gets IMG_POS_LIST array, size and pixels array
//The function calculates the grey level value of each segment 
//update the values in pixels array
void fillPixels(IMG_POS_LIST* segments, unsigned int size, char** pixels) {
	unsigned int i, currRow, currCol, roundValue;
	double value;
	bool sizeIs1;
	IMG_POS_NODE* curr;
	if (size == 1) {
		sizeIs1 = true;
	}
	else {
		sizeIs1 = false;
	}
	for (i = 0; i < size; i++) {
		curr = segments[i].head;
		while (curr != NULL) {
			currRow = curr->position[0];
			currCol = curr->position[1];
			if (sizeIs1) {//if size = 1, then it's illegal to calculate the value, so the value should be 0
				roundValue = 0;
			}
			else {
				value = (double)i * (255 / (size - 1));
				roundValue = (int)floor(value);
			}
			pixels[currRow][currCol] = roundValue;
			curr = curr->next;
		}
	}
}

//This function gets IMG_POS_LIST, size, rows and cols by reference
//The function check the position in all items in IMG_POS_LIST array (searching the max of row and col)
//Return the rows and cols values by reference 
void countRowsAndCols(IMG_POS_LIST* segments, unsigned int size, unsigned short* rows, unsigned short* cols) {
	unsigned int i;
	unsigned short maxRow = segments[0].head->position[0];
	unsigned short maxCol = segments[0].head->position[1];

	IMG_POS_NODE* curr;
	for (i = 0; i < size; i++) {
		curr = segments[i].head;
		while (curr != NULL) {
			if (curr->position[0] > maxRow) {
				maxRow = curr->position[0];
			}
			if (curr->position[1] > maxCol) {
				maxCol = curr->position[1];
			}
			curr = curr->next;

		}
	}
	*rows = maxRow + 1;
	*cols = maxCol + 1;

}
