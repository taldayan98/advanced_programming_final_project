//Tal Dayan - 207378639
//Amit Neeman - 207388794

#include "header.h"


//This function gets file name that contaion picture in formart PGM with magic number P2
//The function creates picture type GREY_IAMGE (with width and hieght values from file) and add the pixels value to pixels array
//Return a pointer of grey image
GRAY_IMAGE* readPGM(char* fname) {
	FILE* fp;
	unsigned short width, height, depth;
	char* magicNumber = (char*)malloc(3 * sizeof(char));
	checkMemoryAllocate(magicNumber);
	GRAY_IMAGE* img = (GRAY_IMAGE*)malloc(sizeof(GRAY_IMAGE));
	checkMemoryAllocate(img);
	fp = fopen(fname, "r");
	checkMemoryAllocate(fp);
	headerValues(fp, magicNumber, &width, &height, &depth);
	pointToFirstIndexInArray(fp);
	img->rows = height;
	img->cols = width;
	img->pixels = createGrayImage(fp, height, width);
	fclose(fp);
	free(magicNumber);
	return img;
}
//This function gets pointer to file
//The function moves the cursor to beginning of pixels values (after the header values)
void pointToFirstIndexInArray(FILE* fp) {
	char c = fgetc(fp);
	while (c == '#' || c == ' ') {
		if (c == '#') {
			fscanf(fp, "%*[^\n]\n");
		}
		c = fgetc(fp);
	}
	fseek(fp, -1, SEEK_CUR);

}

//This fucntion gets pointer to file, pointer to magic numebr, pointer to width, pointer to height and pointer to depth
//The function gets the header values from file
//The function skip on comment
////return the header values by reference 
void headerValues(FILE* fp, char* magicNumber, unsigned short* width, unsigned short* height, unsigned short* depth) {
	int index = 0;
	char c;
	while (!feof(fp) && index < 4) {
		c = fgetc(fp);
		if (c == '#') {
			fscanf(fp, "%*[^\n]\n");
		}
		else if (c != ' ' && c != '\n') {
			fseek(fp, -1, SEEK_CUR);
			switch (index)
			{
			case 0:
				fscanf(fp, "%s\n", magicNumber);
				break;
			case 1:
				fscanf(fp, "%hd", width);
				break;
			case 2:
				fscanf(fp, "%hd", height);
				break;
			case 3:
				fscanf(fp, "%hd", depth);
				break;

			default:
				break;
			}
			index++;
		}
	}
}

//This function gets pointer to file, rows and cols
//The function creates gray image with the data from file
//Creates pixels array and updates the values
//Return a pointer to img
char** createGrayImage(FILE* fp, unsigned short rows, unsigned short cols) {
	int i, j;
	int num;
	char** img = (char**)malloc((rows) * sizeof(char*));
	checkMemoryAllocate(img);
	for (i = 0; i < rows; i++) {
		img[i] = (char*)malloc(cols * sizeof(char));
		checkMemoryAllocate(img[i]);
		for (j = 0; j < cols; j++) {
			fscanf(fp, "%d ", &num);
			img[i][j] = num;
		}
	}
	return img;

}
//This function gets pointer to binary file, rows and cols
//The function creates gray image with the data from file
//Creates pixels array and updates the values
//Return a pointer to img
char** createGrayImage_Bin(FILE* fp, unsigned short rows, unsigned short cols) {
	int i, j;
	unsigned char num;
	fseek(fp, 1, SEEK_CUR);
	char** img = (char**)malloc((rows) * sizeof(char*));
	checkMemoryAllocate(img);
	for (i = 0; i < rows; i++) {
		img[i] = (char*)malloc(cols * sizeof(char));
		checkMemoryAllocate(img[i]);
		for (j = 0; j < cols; j++) {
			fread(&num, sizeof(unsigned char), 1, fp);
			img[i][j] = num;
		}
	}
	return img;

}

//This function gets file name that contaion picture in formart PGM with magic number P5 (binary file)
//The function creates picture type GREY_IAMGE (with width and hieght values from file) and add the pixels value to pixels array
//Return a pointer of grey image
GRAY_IMAGE* readPGM_Bin(char* fname) {
	FILE* fp;
	unsigned short width, height, depth;
	char* magicNumber = (char*)malloc(3 * sizeof(char));
	checkMemoryAllocate(magicNumber);
	GRAY_IMAGE* img = (GRAY_IMAGE*)malloc(sizeof(GRAY_IMAGE));
	checkMemoryAllocate(img);
	fp = fopen(fname, "rb");
	checkMemoryAllocate(fp);
	headerValues(fp, magicNumber, &width, &height, &depth);
	pointToFirstIndexInArray(fp);
	img->rows = height;
	img->cols = width;
	img->pixels = createGrayImage_Bin(fp, height, width);
	fclose(fp);
	free(magicNumber);
	return img;
}

//This function gets pointer to GRAY_IMAGE type
//The function free first pixels array and than img
void freeGrayImage(GRAY_IMAGE* grayImg) {
	freeCharTable(grayImg->pixels, grayImg->rows);
	free(grayImg);
}