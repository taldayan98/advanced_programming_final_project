//Tal Dayan - 207378639
//Amit Neeman - 207388794

#include "header.h"

//This function gets a PGM file name in P2 format 
//The function creates a gray image, converts it to a black and white image, 
// and created a PGM file in P2 format with the converted black and white pixels.
// the file name will be the same as the received file name with an addition of "bw"
void convertPGMToBW(char* fname) {
	unsigned short depth, stamWidth, stamHeight;
	char* magicNumber;
	FILE* fp = fopen(fname, "r");
	checkMemoryAllocate(fp);
	GRAY_IMAGE* grayImg = readPGM(fname);
	magicNumber = (char*)malloc(3 * sizeof(char));
	checkMemoryAllocate(magicNumber);
	headerValues(fp, magicNumber, &stamWidth, &stamHeight, &depth);
	createBWFile(fname, grayImg, depth, magicNumber);
	fclose(fp);
	free(magicNumber);
	freeGrayImage(grayImg);
}

//This function gets a PGM file in P2 format, a gray image, a depth and a magic number
//The function creates 3 black and white PGM file in P2 format with different masks  
void createBWFile(char* fname, GRAY_IMAGE* grayImg, unsigned short depth, char* magicNumber) {
	int i;
	char* newName;
	char** mask;
	bool found;
	FILE* fp;
	for (i = 2; i <= 4; i++) {
		newName = createBWFileName(fname, (char)i + '0', ".bw.pgm");
		fp = fopen(newName, "w");
		checkMemoryAllocate(fp);
		writeHeaderToFile(fp, magicNumber, grayImg->cols, grayImg->rows, 1);
		mask = createMask(i);
		found = writeBWBodyToFile(fp, grayImg, mask, i, depth);
		if (!found) {
			fseek(fp, 0, SEEK_SET);
			writeHeaderToFile(fp, magicNumber, grayImg->cols, grayImg->rows, 0);
		}
		freeCharTable(mask, i);
		free(newName);
		fclose(fp);
	}
}

//This function gets a 2 dimentional array and the rows size
//The function frees the table  
void freeCharTable(char** charTable, int rows)
{
	int i;
	for (i = 0; i < rows; i++) {
		free(charTable[i]);
	}

	free(charTable);
}

//This function gets a file and a gray image, a mask, the mask size and depth
// The function gets the pixels from the gray image and converts them to black and white pixels, according to the mask
// and writes them to the file.
//The function returns a bool that indicates if the max value is white (1) or black (0).
bool writeBWBodyToFile(FILE* fp, GRAY_IMAGE* grayImg, char** mask, int maskSize, unsigned short depth) {
	int grayRange = depth / (maskSize * maskSize - 1);
	int i, j, maskRow, maskCol, grayShade;
	bool found = false;

	for (i = 0; i < grayImg->rows; i++) {
		for (j = 0; j < grayImg->cols; j++) {
			maskRow = i % maskSize;
			maskCol = j % maskSize;
			grayShade = grayImg->pixels[i][j] / grayRange;
			if (grayShade > mask[maskRow][maskCol]) {
				fprintf(fp, "%d ", WHITE);
				found = true;
			}
			else {
				fprintf(fp, "%d ", BLACK);
			}
		}
		if (i != (grayImg->rows - 1)) {
			fprintf(fp, "\n");
		}
	}
	return found;

}

//This function gets a size for the mask
// The function creates the mask according to the size, and the values that mentioned in the project file
char** createMask(int k) {
	char** mask = (char**)malloc(k * sizeof(char*));
	checkMemoryAllocate(mask);

	switch (k)
	{
	case 2:
	{
		mask[0] = (char*)malloc(k * sizeof(char));
		checkMemoryAllocate(mask[0]);
		mask[0][0] = 0;
		mask[0][1] = 2;
		mask[1] = (char*)malloc(k * sizeof(char));
		checkMemoryAllocate(mask[1]);
		mask[1][0] = 3;
		mask[1][1] = 1;
		return mask;
	}
	case 3:
	{
		mask[0] = (char*)malloc(k * sizeof(char));
		checkMemoryAllocate(mask[0]);
		mask[0][0] = 2;
		mask[0][1] = 6;
		mask[0][2] = 4;
		mask[1] = (char*)malloc(k * sizeof(char));
		checkMemoryAllocate(mask[1]);
		mask[1][0] = 5;
		mask[1][1] = 0;
		mask[1][2] = 1;
		mask[2] = (char*)malloc(k * sizeof(char));
		checkMemoryAllocate(mask[2]);
		mask[2][0] = 8;
		mask[2][1] = 3;
		mask[2][2] = 7;
		return mask;

	}
	case 4:
	{
		mask[0] = (char*)malloc(k * sizeof(char));
		checkMemoryAllocate(mask[0]);
		mask[0][0] = 0;
		mask[0][1] = 8;
		mask[0][2] = 2;
		mask[0][3] = 10;
		mask[1] = (char*)malloc(k * sizeof(char));
		checkMemoryAllocate(mask[1]);
		mask[1][0] = 12;
		mask[1][1] = 4;
		mask[1][2] = 14;
		mask[1][3] = 6;
		mask[2] = (char*)malloc(k * sizeof(char));
		checkMemoryAllocate(mask[2]);
		mask[2][0] = 3;
		mask[2][1] = 11;
		mask[2][2] = 1;
		mask[2][3] = 9;
		mask[3] = (char*)malloc(k * sizeof(char));
		checkMemoryAllocate(mask[3]);
		mask[3][0] = 15;
		mask[3][1] = 7;
		mask[3][2] = 13;
		mask[3][3] = 5;
		return mask;
	}
	default:
		return NULL;
		break;
	}
}

//This function gets a file name, a size of the mask and a new extention
//The function creates a new file name, with the name and adds the mask size and extention
//The function returns the new file name
char* createBWFileName(char* name, char k, char* extension) {
	int len, i = 0, j = 0;
	len = strlen(name);
	bool found = false;
	char* newName = (char*)malloc((len + 5) * sizeof(char));
	checkMemoryAllocate(newName);
	while (i < len && !found) {
		if (name[i] == '.')
			found = true;
		newName[i] = name[i];
		i++;
	}
	newName[i - 1] = k;
	for (i; i < len + 4; i++, j++) {
		newName[i] = extension[j];
	}
	newName[len + 4] = '\0';
	return newName;
}
