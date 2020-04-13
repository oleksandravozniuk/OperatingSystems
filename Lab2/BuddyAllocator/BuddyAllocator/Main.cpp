
#include <cstdlib>
#include <cmath>
#include <stdio.h>

#define MAX_SIZE	64//Maximum space
#define LIST_LEN	7//Header array length
#define MAX_INDEX	6//Maximum index of the header array
struct BuddyNode {
	BuddyNode* preLink;
	BuddyNode* nextLink;
	int k;//Can only allocate 2 to the power of k
	int tag;//0 idle, 1 Occupy
};

typedef struct BuddyHead {
	BuddyNode* head;
	int nodeSize;
}FreeList[LIST_LEN];

static BuddyNode* freeSpace = NULL;//To free up memory
static BuddyNode* userSpace[MAX_SIZE] = { NULL };
static int userCount = 0;

void initFreeList(FreeList list) {
	for (int i = 0; i < LIST_LEN; i++) {
		list[i].head = NULL;
		list[i].nodeSize = (int)pow(2.0, i);
	}
	//Allocate maximum space
	BuddyNode* p = (BuddyNode*)malloc(sizeof(BuddyNode) * MAX_SIZE);
	if (p != NULL) {
		p->k = MAX_INDEX;
		p->tag = 0;
		p->nextLink = p->preLink = p;//Doubly circular linked list
	}
	freeSpace = list[MAX_INDEX].head = p;
}

void destoryFreeList(FreeList list) {
	for (int i = 0; i < LIST_LEN; i++) {
		list[i].head = NULL;
	}
	free(freeSpace);
}

BuddyNode* buddyAlloc(FreeList list, int size) {
	bool isFirst = true;
	int k = -1;//K value of allocated space
	int needK = -1;//The k value that needs to be allocated space.
	//Find the first space that meets the conditions.
	for (int i = 0; i < LIST_LEN; i++) {
		BuddyHead head = list[i];
		if (head.nodeSize >= size) {
			if (isFirst) {
				needK = i;
				isFirst = false;
			}
			if (head.head != NULL) {//Found allocable space
				k = i;
				break;
			}
		}
	}
	BuddyNode* freeNode = NULL;
	if (k == -1) {
		return NULL;//Can't find a space that satisfies the conditions ..
	}
	else {
		//Set this space to be occupied, and replace the header element ..
		freeNode = list[k].head;
		freeNode->k = needK;
		freeNode->tag = 1;//Set to occupied
		list[k].head = freeNode->nextLink;//Reset the meter head ..
		if (freeNode->preLink == freeNode->nextLink) {//After deleting this space, it becomes an empty table.
			list[k].head = NULL;
		}
		else {//Delete this node.
			freeNode->preLink->nextLink = freeNode->nextLink;
			freeNode->nextLink->preLink = freeNode->preLink;
		}
		//The remaining space is inserted in needK and k-1 in turn.
		for (int i = needK; i < k; i++) {//Start with high order ..
			int index = (int)pow(2.0, i);
			BuddyNode* p = freeNode + index;
			p->k = i;
			p->tag = 0;
			BuddyNode* head = list[i].head;
			if (head != NULL) {
				p->preLink = head->preLink;
				p->nextLink = head;
				p->preLink->nextLink = head->preLink = p;
			}
			else {
				p->preLink = p->nextLink = p;
			}
			list[i].head = p;//Reset the meter head ..
		}
	}
	userSpace[userCount++] = freeNode;
	return freeNode;
}

void userSpaceFree(BuddyNode* node) {
	for (int i = 0; i < userCount; i++) {
		if (userSpace[i] == node) {
			userSpace[i] = NULL;
		}
	}
}


void buddyReclaim(FreeList list, BuddyNode* node) {
	userSpaceFree(node);
	while (node->k < MAX_INDEX) {
		int sub = node - freeSpace;//Calculate the specific coordinate position
		BuddyNode* buddy = NULL;
		int i = (int)pow(2.0, node->k + 1);
		bool isNext = true;//Is your partner behind
		if (sub % i == 0) {
			buddy = node + i / 2;
		}
		else {//Partners first.
			buddy = node - i / 2;
			isNext = false;
		}
		if (buddy->tag == 0) {//Partners idle, merge
			//First release from the list buddy
			if (buddy->preLink == buddy->nextLink) {//There is only one node in the table, and after release, it becomes an empty table
				list[buddy->k].head = NULL;
			}
			else {//Delete a node
				buddy->preLink->nextLink = buddy->nextLink;
				buddy->nextLink->preLink = buddy->preLink;
				list[buddy->k].head = buddy->nextLink;
			}
			if (isNext == false) {
				node = buddy;
			}
			node->k++;
		}
		else {//Partner space is occupied ..
			break;
		}
	}
	BuddyNode* head = list[node->k].head;
	node->tag = 0;
	if (head == NULL) {//Header is empty
		node->preLink = node->nextLink = node;
	}
	else {
		node->nextLink = head;
		node->preLink = head->preLink;
		node->preLink->nextLink = head->preLink = node;
	}
	list[node->k].head = node;
}

void printList(FreeList list) {
	printf("------------------------Print buddy list-------------------\n");
	for (int i = 0; i < LIST_LEN; i++) {
		BuddyNode* head = list[i].head;
		if (head) {
			printf("The first address space is：%p,Table precursor ：%p,Successor：%p,The block size is 2 to the power of %i\n", head, head->preLink, head->nextLink, head->k);
		}
	}
	printf("------------------------User space-------------------\n");
	for (int i = 0; i < userCount; i++) {
		BuddyNode* us = userSpace[i];
		if (us != NULL) {
			printf("he first address space is：%p,The block size is 2 to the power of %i\n", us, us->k);
		}
	}
	printf("\n");
}



int main()
{
	FreeList list;
	initFreeList(list);
	printList(list);
	printf("---------------After allocating a 1 space--------------\n");
	BuddyNode* s1_1 = buddyAlloc(list, 1);
	printList(list);
	printf("---------------After allocating 2 more space--------------\n");
	BuddyNode* s1_2 = buddyAlloc(list, 1);
	printList(list);
	printf("---------------After allocating a 17 space--------------\n");
	BuddyNode* s29 = buddyAlloc(list, 17);
	printList(list);
	printf("---------------After releasing a 2 space--------------\n");
	buddyReclaim(list, s1_2);
	printList(list);
	printf("---------------After releasing a 1 space--------------\n");
	buddyReclaim(list, s1_1);
	printList(list);
	destoryFreeList(list);
	return 0;
}
