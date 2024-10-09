#include "header.h"

//This function gets GRAY_IMAGE img array, start point, threshold to check, and a bool table
//Set the root of segment that start at start point and changes his value at bool table
// Creates a tree in function findSingleSegmentHelper
// Return the segment that contain pointer to root of the tree
SEGMENT findSingleSegmentWithBool(GRAY_IMAGE* img, IMG_POS start, unsigned char threshold, bool** boolTable)
{
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
		originalStartValue = img->pixels[row][col];
		segment.root = createTnode(row, col);
		boolTable[row][col] = true;
		findSingleSegmentHelper(img, segment.root, originalStartValue, threshold, boolTable);
	}
	return segment;
}

//This function gets GRAY_IMAGE img array, threshold to check, an array of pointers to IMG_POS_LIST
//The function finds all segments. The start point is (0,0).
// Creates a list of a single segment, and adds it to the array.
// Returns the size of the IMG_POS_LIST array
int findAllSegments(GRAY_IMAGE* img, unsigned char threshold, IMG_POS_LIST** segments) {
	IMG_POS_LIST* returnedSegments = (IMG_POS_LIST*)malloc((img->rows * img->cols) * sizeof(IMG_POS_LIST));
	SEGMENT segment;
	IMG_POS start;
	IMG_POS_LIST list;
	int i, j, segIndex = 0;
	bool** boolTable = createBoolTable(img->rows, img->cols);
	for (i = 0; i < img->rows; i++) {
		for (j = 0; j < img->cols; j++) {
			start[0] = i;
			start[1] = j;
			if (boolTable[i][j] == false) {
				segment = findSingleSegmentWithBool(img, start, threshold, boolTable);
				list = convertSegmentToList(segment);
				returnedSegments[segIndex] = list;
				segIndex++;
			}
		}
	}
	returnedSegments = realloc(returnedSegments, segIndex * sizeof(IMG_POS_LIST));
	checkMemoryAllocate(returnedSegments);
	freeBoolTable(boolTable, img->rows);
	*segments = returnedSegments;
	return segIndex;

}

//This function gets a segment.
//The function creates a list of a single segment.
// Returns the IMG_POS_LIST list
IMG_POS_LIST convertSegmentToList(SEGMENT segment) {
	IMG_POS_LIST list;
	IMG_POS_LIST stamList;
	makeEmptyList(&list);
	stamList = convertSegmentToListHelper(&list, segment.root);
	return list;

}

//This function gets a pointer to list, and a pointer to TNODE.
//The function creates a list of a single segment in a recursive way. The functions adds the nodes positions to the received list
// Returns the IMG_POS_LIST list
IMG_POS_LIST convertSegmentToListHelper(IMG_POS_LIST* originalList, TNODE* root) {
	IMG_POS_LIST list;
	makeEmptyList(&list);
	TNODE_LNODE* curr;
	TNODE* currTnode;
	insertDataToEndDList(&list, root->position);
	if (root->nextPossiblePositions.tail == NULL)
	{
		mergeLists(originalList, &list);
		return list;
	}
	else
	{
		mergeLists(originalList, &list);
		curr = root->nextPossiblePositions.head;
		while (curr != NULL) {
			currTnode = curr->node;
			list = convertSegmentToListHelper(originalList, currTnode);
			curr = curr->next;
		}
		return list;
	}


}

//This function gets a position and next IMG_POS_NODE and a prev IMG_POS_NODE
//The function creates IMG_POS_NODE and adds the received value
//returns a pointer to the created IMG_POS_NODE
IMG_POS_NODE* createNewDListNode(IMG_POS pos, IMG_POS_NODE* next, IMG_POS_NODE* prev)
{
	IMG_POS_NODE* res;
	res = (IMG_POS_NODE*)malloc(sizeof(IMG_POS_NODE));
	checkMemoryAllocate(res);
	res->position[0] = pos[0];
	res->position[1] = pos[1];
	res->next = next;
	res->prev = prev;
	return res;
}

//This function gets a pointer to a list and a position
//The function adds the position to the end of the list
void insertDataToEndDList(IMG_POS_LIST* lst, IMG_POS position)
{
	IMG_POS_NODE* newTail;
	newTail = createNewDListNode(position, NULL, NULL);
	insertNodeToEndDList(lst, newTail);
}

//This function gets a pointer to a list and a pointer to a new IMG_POS_NODE tail
//The function adds the node to the end of the list
void insertNodeToEndDList(IMG_POS_LIST* lst, IMG_POS_NODE* tail)
{
	if (isEmptyDList(lst) == true)
	{
		tail->next = tail->prev = NULL;
		lst->head = lst->tail = tail;
	}
	else
	{
		tail->prev = lst->tail;
		tail->next = NULL;
		lst->tail->next = tail;
		lst->tail = tail;
	}
}

//This function gets list and checks if the list is empty
//Return true if empty, else return false
bool isEmptyDList(IMG_POS_LIST* lst)
{
	if (lst->head == NULL)
	{
		return true;
	}
	else {
		return false;
	}
}

//This function gets 2 pointers to 2 list
//The function adds the values of the second list to the first list
void mergeLists(IMG_POS_LIST* list1, IMG_POS_LIST* list2) {
	if (isEmptyDList(list1)) {
		list1->head = list2->head;
		list1->tail = list2->tail;
	}
	else if (isEmptyDList(list2)) {
		return;
	}
	else {
		list2->head->prev = list1->tail;
		list1->tail->next = list2->head;
		list1->tail = list2->tail;
	}
}

//This function gets a pointer to a list
//The function adds a NULL value to the head and tail of the list
void makeEmptyList(IMG_POS_LIST* list) {
	list->head = NULL;
	list->tail = NULL;
}

//This function gets an array of pointers to segments and the size
//The function frees all values of the list of each array cell, and the array
void freeSegmentsArray(IMG_POS_LIST* segments, int size) {
	int i;
	IMG_POS_LIST currList;
	IMG_POS_NODE* currNode;
	IMG_POS_NODE* nextNode;
	for (i = 0; i < size; i++) {
		currList = segments[i];
		currNode = currList.head;
		while (currNode != NULL) {
			nextNode = currNode->next;
			free(currNode);
			currNode = nextNode;
		}
	}
	free(segments);
}
