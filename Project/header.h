//Tal Dayan - 207378639
//Amit Neeman - 207388794

#ifndef __HEADER_H
#define __HEADER_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

//Definition
typedef struct tnode TNODE;
typedef struct tnode_lnode TNODE_LNODE;
typedef struct tnode_list TNODE_LIST;

typedef struct grayImage {
	unsigned short rows, cols;
	unsigned char** pixels;
}GRAY_IMAGE;

typedef unsigned short IMG_POS[2];

typedef struct tnode_lnode {
	TNODE* node;
	struct tnode_lnode* next;
}TNODE_LNODE;

typedef struct tnode_list {
	TNODE_LNODE* head;
	TNODE_LNODE* tail;
}TNODE_LIST;

typedef struct tnode {
	IMG_POS position;
	TNODE_LIST nextPossiblePositions;  //הילדים רשימת
}TNODE;

typedef struct segment {
	TNODE* root;
}SEGMENT;

typedef struct img_pos_node {
	IMG_POS position;
	struct img_pos_node* next, * prev;
}IMG_POS_NODE;

typedef struct img_pos_list {
	IMG_POS_NODE* head;
	IMG_POS_NODE* tail;
}IMG_POS_LIST;

typedef struct rgb {
	unsigned char r, g, b;
}RGB;

typedef struct colorImage {
	unsigned short rows, cols;
	RGB** pixels;
}COLOR_IMAGE;

#define WHITE 1
#define BLACK 0
#define BTYE_SIZE 8

//functions
SEGMENT findSingleSegment(GRAY_IMAGE* img, IMG_POS start, unsigned char threshold);
void findSingleSegmentHelper(GRAY_IMAGE* img, TNODE* root, char originalStartValue, unsigned char threshold, bool** boolTable);
bool** createBoolTable(int rows, int cols);
TNODE* createTnode(int row, int col);
void findSingleSegmentHelper(GRAY_IMAGE* img, TNODE* root, char originalStartValue, unsigned char threshold, bool** boolTable);
void findNeighborsForStart(GRAY_IMAGE* img, TNODE* root, char originalStartValue, unsigned char threshold, bool** boolTable);
void AddPixelToRootList(GRAY_IMAGE* img, TNODE* root, int row, int col, char originalStartValue, unsigned char threshold, bool** boolTable);
bool checkBorderOfPoint(int row, int col, GRAY_IMAGE* img);
bool checkIfAlreadyPassed(int row, int col, bool** boolTable);
void createAndInsertToEndList(TNODE* root, int row, int col);
bool isEmptyList(TNODE_LIST* lst);
void freeBoolTable(bool** boolTable, int rows);
void checkMemoryAllocate(void* ptr);
void insertTnodeLnodeToEndList(TNODE_LIST* lst, TNODE_LNODE* itemToInsert);
TNODE_LNODE* createTnode_Lnode(TNODE* tnode, TNODE_LNODE* next);
SEGMENT findSingleSegmentWithBool(GRAY_IMAGE* img, IMG_POS start, unsigned char threshold, bool** boolTable);
int findAllSegments(GRAY_IMAGE* img, unsigned char threshold, IMG_POS_LIST** segments);
IMG_POS_LIST convertSegmentToList(SEGMENT segment);
IMG_POS_LIST convertSegmentToListHelper(IMG_POS_LIST* originalList, TNODE* root);
IMG_POS_NODE* createNewDListNode(IMG_POS pos, IMG_POS_NODE* next, IMG_POS_NODE* prev);
void insertDataToEndDList(IMG_POS_LIST* lst, IMG_POS position);
void insertNodeToEndDList(IMG_POS_LIST* lst, IMG_POS_NODE* tail);
bool isEmptyDList(IMG_POS_LIST* lst);
void mergeLists(IMG_POS_LIST* list1, IMG_POS_LIST* list2);
void makeEmptyList(IMG_POS_LIST* list);
void freeSegmentsArray(IMG_POS_LIST* segments, int size);
GRAY_IMAGE* readPGM(char* fname);
void headerValues(FILE* fp, char* magicNumber, unsigned short* width, unsigned short* height, unsigned short* depth);
char** createGrayImage(FILE* fp, unsigned short rows, unsigned short cols);
void pointToFirstIndexInArray(FILE* fp);
COLOR_IMAGE* readPPM(char* fname);
RGB** createColorImage(FILE* fp, unsigned short rows, unsigned short cols);
GRAY_IMAGE* colorSegments(IMG_POS_LIST* segments, unsigned int size);
void fillPixels(IMG_POS_LIST* segments, unsigned int size, char** pixels);
void countRowsAndCols(IMG_POS_LIST* segments, unsigned int size, unsigned short* rows, unsigned short* cols);
void convertPPMToPGM(char* fname);
GRAY_IMAGE* createGrayImageFromColorImage(COLOR_IMAGE* colorImg);
char* createFileName(char* name, char* extension);
void writeHeaderToFile(FILE* fp, char* magicNumber, int width, int height, int depth);
void writeBodyToFile(FILE* fp, GRAY_IMAGE* img);
void convertPPMToPGM_Bin(char* fname);
void createBWFile(char* fname, GRAY_IMAGE* grayImg, unsigned short depth, char* magicNumber);
char** createMask(int k);
char* createBWFileName(char* name, char k, char* extension);
void convertPGMToBW(char* fname);
bool writeBWBodyToFile(FILE* fp, GRAY_IMAGE* grayImg, char** mask, int maskSize, unsigned short depth);
void freeCharTable(char** charTable, int rows);
void convertPGMToBW_Bin(char* fname);
void createBWFile_Bin(char* fname, GRAY_IMAGE* grayImg, unsigned short depth, char* magicNumber);
bool writeBWBodyToFile_Bin(FILE* fp, GRAY_IMAGE* grayImg, char** mask, int maskSize, unsigned short depth);
RGB** createColorImage_Bin(FILE* fp, unsigned short rows, unsigned short cols);
char** createGrayImage_Bin(FILE* fp, unsigned short rows, unsigned short cols);
void writeBodyToFile_Bin(FILE* fp, GRAY_IMAGE* img);
COLOR_IMAGE* readPPM_Bin(char* fname);
GRAY_IMAGE* readPGM_Bin(char* fname);
void freeColorImage(COLOR_IMAGE* colorImg);
void freeGrayImage(GRAY_IMAGE* grayImg);
void printMenu();
void start();
char* readFileName();
void saveCompressed(char* fname, GRAY_IMAGE* image, unsigned char maxGrayLevel);
void compressPixels(FILE* fp, GRAY_IMAGE* image, unsigned char maxGrayLevel, int maxBits);
int bitsSize(unsigned char maxGrayLevel);
void convertCompressedImageToPGM(char* fname);
void decompressPixels(FILE* fpRead, FILE* fpWrite, int rows, int cols, int maxBits);
int findMaxDepth(GRAY_IMAGE* grayImg);

#endif
