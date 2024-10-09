#include "header.h"


//This function gets GRAY_IMAGE img array, start point, threshold to check
//The function first creates a bool table(dynamic array) and set the value for each point to false
//Set the root of segment that start at start point and changes his value at bool table
// Creates a tree in function findSingleSegmentHelper
// Return the segment that contain pointer to root of the tree
SEGMENT findSingleSegment(GRAY_IMAGE* img, IMG_POS start, unsigned char threshold)
{
	bool** boolTable;
	char originalStartValue;
	SEGMENT segment;
	int row, col;
	row = start[0]; //the value in start
	col = start[1];
	if (img->rows == 0 && img->cols == 0) { //checks if img is valid 
		printf("error in img array, please check.\n");
		segment.root = NULL;
	}
	else {
		boolTable = createBoolTable(img->rows, img->cols);
		originalStartValue = img->pixels[row][col];
		segment.root = createTnode(row, col);
		boolTable[row][col] = true;
		findSingleSegmentHelper(img, segment.root, originalStartValue, threshold, boolTable);
		freeBoolTable(boolTable, img->rows);
	}
	return segment;
}

//create bool table (2 dimensional dynamic array) with value false
bool** createBoolTable(int rows, int cols)
{
	bool** arr = (bool**)malloc(rows * sizeof(bool*));
	checkMemoryAllocate(arr);
	int i, j;
	for (i = 0; i < rows; i++) {
		arr[i] = (bool*)malloc(cols * sizeof(bool));
		checkMemoryAllocate(arr[i]);
		for (j = 0; j < cols; j++) {
			arr[i][j] = false;
		}
	}
	return arr;
}
//This function check memory allocate
//print a error if memory allocate doesn't successes
void checkMemoryAllocate(void* ptr)
{
	if (ptr == NULL) //memory issue
	{
		printf("Error! memory not allocated.");
		exit(0);
	}
}

//This function gets 2 int - row and col 
//The function create Tnode, put in position[0] - row and in position[1]- col
//Update the head and tail of the list in tnode to NULL
//return the adress of Tnode
TNODE* createTnode(int row, int col)
{
	TNODE* res;
	res = (TNODE*)malloc(sizeof(TNODE));
	checkMemoryAllocate(res);
	res->position[0] = row;
	res->position[1] = col;
	res->nextPossiblePositions.head = NULL;
	res->nextPossiblePositions.tail = NULL;
	return res;
}

//This function helps the function findSingleSegment 
//Gets img, root, originalStartValue, threshold and bool tavle
//First check all neighbors for root and update the relevant information
// Than if the list in root is empty - return
// if not - call recursivi for each item in list of root
void findSingleSegmentHelper(GRAY_IMAGE* img, TNODE* root, char originalStartValue, unsigned char threshold, bool** boolTable)
{
	TNODE_LNODE* curr;
	TNODE* currTnode;
	findNeighborsForStart(img, root, originalStartValue, threshold, boolTable);

	if (root->nextPossiblePositions.tail == NULL)
	{
		return;
	}
	else {
		curr = root->nextPossiblePositions.head;
		//currTnode = curr->node;
		while (curr != NULL)
		{
			currTnode = curr->node;
			findSingleSegmentHelper(img, currTnode, originalStartValue, threshold, boolTable);
			curr = curr->next;
		}
	}
}
//This function gets img, root, originalStartValue, threshold and bool table
//The function calls each neighbor by the order in question 
//if the neighbor is passes the criteria - add his to list
void findNeighborsForStart(GRAY_IMAGE* img, TNODE* root, char originalStartValue, unsigned char threshold, bool** boolTable)
{
	int row, col;

	//all of start neighbors 
	// one up from start - (-1 in row)
	row = root->position[0] - 1;
	col = root->position[1];
	AddPixelToRootList(img, root, row, col, originalStartValue, threshold, boolTable);

	// one up and right from start (-1 in row and +1 in col)
	row = root->position[0] - 1;
	col = root->position[1] + 1;
	AddPixelToRootList(img, root, row, col, originalStartValue, threshold, boolTable);

	// on the right of start (+1 in col)
	row = root->position[0];
	col = root->position[1] + 1;
	AddPixelToRootList(img, root, row, col, originalStartValue, threshold, boolTable);

	//One down and right to start (+1 to row and +1 to col)
	row = root->position[0] + 1;
	col = root->position[1] + 1;
	AddPixelToRootList(img, root, row, col, originalStartValue, threshold, boolTable);

	//One down to start (+1 in row)
	row = root->position[0] + 1;
	col = root->position[1];
	AddPixelToRootList(img, root, row, col, originalStartValue, threshold, boolTable);

	// One down left to start (+1 to row and -1 to col) 
	row = root->position[0] + 1;
	col = root->position[1] - 1;
	AddPixelToRootList(img, root, row, col, originalStartValue, threshold, boolTable);

	// One left to start (-1 to col)
	row = root->position[0];
	col = root->position[1] - 1;
	AddPixelToRootList(img, root, row, col, originalStartValue, threshold, boolTable);

	//One up left to start (-1 to row and -1 to col) 
	row = root->position[0] - 1;
	col = root->position[1] - 1;
	AddPixelToRootList(img, root, row, col, originalStartValue, threshold, boolTable);

}
//This function gets img, root, row, col, threshold and bool table
//the function first checks that the point (row, col) in border of img arrays
//Than check that the value of point (row, col) in false in bool table(meating didn't add the point to list)
//If all true - check that the threshold between original value to point value in smaller than threshold we got
//if True - create Tnode and Tnode_Lnode for point and add to list
//Update the value of point in bool table to true
void AddPixelToRootList(GRAY_IMAGE* img, TNODE* root, int row, int col, char originalStartValue, unsigned char threshold, bool** boolTable)
{
	bool checkBorderValid;
	char currentStartValue;
	int orMiCs, CsMiOr;
	checkBorderValid = checkBorderOfPoint(row, col, img);
	if (checkBorderValid) { //check border of point
		if (!checkIfAlreadyPassed(row, col, boolTable)) { //not added to list yet
			currentStartValue = img->pixels[row][col];
			orMiCs = originalStartValue - currentStartValue; //
			CsMiOr = currentStartValue - originalStartValue; // 
			if ((orMiCs <= threshold && orMiCs >= 0) || (CsMiOr <= threshold && CsMiOr >= 0))
			{
				createAndInsertToEndList(root, row, col);
				boolTable[row][col] = true;
			}
		}
	}
}
//This function check border of point in pixesls array
bool checkBorderOfPoint(int row, int col, GRAY_IMAGE* img)
{
	bool border = false;
	if ((row < img->rows && row >= 0) && (col < img->cols && col >= 0)) {
		border = true;
	}
	return border;
}

//this function checks if we already check the point in bool table
//return false if not checked yet
//True if the point alreay passed 
bool checkIfAlreadyPassed(int row, int col, bool** boolTable)
{
	return boolTable[row][col];
}

//This function gets root, row and col
//First creates Tnode for point(row, col) and than creates
//Tnode_Lnode for res and insert the Tnode_Lnode to end of list
void createAndInsertToEndList(TNODE* root, int row, int col)
{
	TNODE* res;
	TNODE_LNODE* TnodeLnode;
	res = createTnode(row, col);
	TnodeLnode = createTnode_Lnode(res, NULL);
	insertTnodeLnodeToEndList(&(root->nextPossiblePositions), TnodeLnode);

}
//This function gets tnode, and the adress of the next Tnode_Lnode
//The function creates Tnode_Lnode with the values 
//return the adress of the created Tnode_Lnode
TNODE_LNODE* createTnode_Lnode(TNODE* tnode, TNODE_LNODE* next)
{
	TNODE_LNODE* res;
	res = (TNODE_LNODE*)malloc(sizeof(TNODE_LNODE));
	checkMemoryAllocate(res);
	res->node = tnode;
	res->next = next;

	return res;

}
//This function gets list, and item to insert from type TNODE_LNODE
//The function checks if the list is empty - if true - updates the head and tail to the item
//If not Update the last item ->next to itemToInsert and update the new tail
void insertTnodeLnodeToEndList(TNODE_LIST* lst, TNODE_LNODE* itemToInsert)
{
	if (isEmptyList(lst)) {
		lst->head = lst->tail = itemToInsert;
	}
	else {
		lst->tail->next = itemToInsert;
		lst->tail = itemToInsert;
	}
	itemToInsert->next = NULL;
}
//This function gets list and checks if the list is empty
//Return true if empty, else return false
bool isEmptyList(TNODE_LIST* lst)
{
	if (lst->head == NULL)
	{
		return true;
	}
	else {
		return false;
	}
}
//This function free the bool table
// gets bool table and the number of rows
//Free first every row and than free all
void freeBoolTable(bool** boolTable, int rows)
{
	int i;
	for (i = 0; i < rows; i++) {
		free(boolTable[i]);
	}

	free(boolTable);
}