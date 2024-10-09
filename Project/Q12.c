//Tal Dayan - 207378639
//Amit Neeman - 207388794

#include "header.h"

// The function starts the process. The user enters an option number and the function activates the wanted flow
void start() {
	COLOR_IMAGE* colorImg = NULL;
	GRAY_IMAGE* grayImg = NULL;
	IMG_POS_LIST* segments = NULL;
	unsigned char treshold, MaxGrayLevel;
	int option, size, maxDepth;
	char* fileName = NULL;
	FILE* fpPgm;

	printMenu();
	printf("\n\nPlease enter a number of option: ");
	scanf("%d", &option);
	while (option >= 1 && option <= 11) {
		switch (option)
		{
		case 1:
		{//the user enters a file name and we create a color image
			//uses Q1
			printf("Please enter a file name in PPM (P3) format with the extention: \n");
			fileName = readFileName();
			colorImg = readPPM(fileName);
			break;
		}
		case 2:
		{//the user enters a file name and we create a gray image
			//uses Q2
			printf("Please enter a file name in PGM (P2) format with the extention: \n");
			fileName = readFileName();
			grayImg = readPGM(fileName);
			break;
		}
		case 3:
		{//finds the segments from the file from option 2
			//uses Q4
			if (grayImg == NULL) { //if there's no gray image, the read a file name and create a gray image
				printf("File name is missing. Please enter a file name in PGM (P2) format with the extention: \n");
				fileName = readFileName();
				grayImg = readPGM(fileName);
			}
			printf("Please enter the treshold: ");
			scanf("%d", &treshold);
			size = findAllSegments(grayImg, treshold, &segments);
			segments = realloc(segments, size * sizeof(IMG_POS_LIST));
			checkMemoryAllocate(segments);
			break;
		}
		case 4:
		{//creates a gray image, and changed the color so all the segments will contain the same gray level
			//uses Q5
			if (segments == NULL) {
				printf("No segments were found. Please call option 3. \n");
			}
			else {
				grayImg = colorSegments(segments, size);
			}
			break;
		}
		case 5:
		{//writes the gray image that was created in option 4 to a text PGM file
			if (grayImg == NULL) { // if not gray image, then the user will need to call option 4 to create the gray image with the colored segments
				printf("Gray image was not found. Please call option 4. \n");
			}
			else {
				printf("Please enter a file name in PGM (P2) format with the extention: \n");
				fileName = readFileName();
				fpPgm = fopen(fileName, "w");
				checkMemoryAllocate(fpPgm);
				maxDepth = findMaxDepth(grayImg);
				writeHeaderToFile(fpPgm, "P2", grayImg->cols, grayImg->rows, maxDepth);
				writeBodyToFile(fpPgm, grayImg);
				fclose(fpPgm); //this is the only case we use the file, so we close it here
			}
			break;
		}
		case 6:
		{//the user enters a file name and max gray level and we compress it
			//uses Q6
			printf("Please enter a file name in PGM (P2) format with the extention: \n");
			fileName = readFileName();
			grayImg = readPGM(fileName);
			printf("Please enter a file name in order to create a compressed file with the extention: \n");
			fileName = readFileName();
			printf("Please enter the max gray level (0-127): \n");
			scanf("%d", &MaxGrayLevel);
			saveCompressed(fileName, grayImg, MaxGrayLevel);
			break;
		}
		case 7:
		{//the user enters a compressed file name and decompresses it
			//uses Q7
			printf("Please enter the compressed file name with the extention: \n");
			fileName = readFileName();
			convertCompressedImageToPGM(fileName);
			break;
		}
		case 8:
		{//the user enters a PPM file name and we convert it to a PGM file
			//uses Q8
			printf("Please enter a file name in PPM (P3) format with the extention: \n");
			fileName = readFileName();
			convertPPMToPGM(fileName);
			break;
		}
		case 9:
		{//the user enters a PGM file name and we convert it to a black and white PGM file
			//uses Q9
			printf("Please enter a file name in PGM (P2) format with the extention: \n");
			fileName = readFileName();
			convertPGMToBW(fileName);
			break;
		}
		case 10:
		{//the user enters a PPM file name and we convert it to a binary PGM file
			//uses Q10
			printf("Please enter a file name in PPM (P6) format with the extention: \n");
			fileName = readFileName();
			convertPPMToPGM_Bin(fileName);
			break;
		}
		case 11:
		{//the user enters a PGM file name and we convert it to a binary black and white PGM file
			//uses Q8
			printf("Please enter a file name in PGM (P5) format with the extention: \n");
			fileName = readFileName();
			convertPGMToBW_Bin(fileName);
			break;
		}
		default: //exit the while loop, in case the user enters a number that's out of range
			break;
		}

		printf("\nPlease choose another option from the menu.\n");
		printMenu();
		scanf("%d", &option);
	}
	if (colorImg != NULL) {
		freeColorImage(colorImg);
	}
	if (grayImg != NULL) {
		freeGrayImage(grayImg);
	}
	if (fileName != NULL) {
		free(fileName);
	}
	if (segments != NULL) {
		freeSegmentsArray(segments, size);
	}
}

// The function prints the start menu
void printMenu() {
	printf("option 1: Read an image file in PPM (p3) format\n");
	printf("option 2: Read an image file in PGM (p2) format\n");
	printf("option 3: Find all segments\n");
	printf("option 4: Color the segments\n");
	printf("option 5: Save the colored image to PGM\n");
	printf("option 6: Compress and save the original image in a compressed format\n");
	printf("option 7: Convert a compressed image to PGM format\n");
	printf("option 8: Convert PPM to PGM (p3 -> p2)\n");
	printf("option 9: Convert PGM to BW (p2 -> p2)\n");
	printf("option 10: Convert PPM to PGM (p6 -> p5)\n");
	printf("option 11: Convert PGM to BW (p5 -> p5)\n");
	printf("choose any other number to exit\n");
}

// The function reads a file name from a user. 
//The function returns the file name
char* readFileName() {
	int phySize = 50, logSize = 0;
	char* fileName = (char*)malloc(phySize * sizeof(char));
	checkMemoryAllocate(fileName);
	char c;
	c = (char)getchar();
	if (c == '\n') {
		c = (char)getchar();
	}
	while (c != '\n') {
		if (phySize == logSize) {
			phySize *= 2;
			fileName = realloc(fileName, phySize * sizeof(char));
			checkMemoryAllocate(fileName);
		}
		fileName[logSize] = c;
		c = (char)getchar();
		logSize++;
	}
	if (phySize >= logSize) {
		fileName = realloc(fileName, (logSize + 1) * sizeof(char));;
		checkMemoryAllocate(fileName);
	}
	fileName[logSize] = '\0';
	return fileName;

}

// The function gets a gray image 
// The function finds the max depth from the image
//The function returns the max value
int findMaxDepth(GRAY_IMAGE* grayImg) {
	int i, j, max = grayImg->pixels[0][0];
	bool biggest = false;
	if (max == 255)
		biggest = true;
	for (i = 0; i < grayImg->rows && !biggest; i++) {
		for (j = 0; j < grayImg->cols && !biggest; j++) {
			if (grayImg->pixels[i][j] > max)
				max = grayImg->pixels[i][j];
			if (max == 255)
				biggest = true;
		}
	}
	return max;
}
