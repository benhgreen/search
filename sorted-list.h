#ifndef SORTED_LIST_H
#define SORTED_LIST_H
/*
 * sorted-list.h
 */

#include <stdlib.h>


/*
 * When your sorted list is used to store objects of some type, since the
 * type is opaque to you, you will need a comparator function to order
 * the objects in your sorted list.
 *
 * You can expect a comparator function to return -1 if the 1st object is
 * smaller, 0 if the two objects are equal, and 1 if the 2nd object is
 * smaller.
 *
 * Note that you are not expected to implement any comparator functions.
 * You will be given a comparator function when a new sorted list is
 * created.
 */


typedef int (*CompareFuncT)(char*, char*);



struct LLnode_file
{	
	char* file;
	int count;
	struct LLnode_file *next;
};
typedef struct LLnode_file* node_f;

/*
* 
*/
struct LLnode
{	
	char *data;
	int count;
	struct LLnode *previous, *next;
	struct LLnode_file *head;
};
typedef struct LLnode* node;

/*
 * Sorted list type.  You need to fill in the type as part of your implementation.
 */
struct SortedList
{
  node *head;
  int totalcount;
  CompareFuncT cmp;
};
typedef struct SortedList* SortedListPtr;

/*
 * File list type. 
 */
struct FileList
{
  node_f *head;
  int totalcount;
  CompareFuncT cmp;
};
typedef struct FileList* FileListPtr;



/*
 * SLCreate creates a new, empty sorted list.  The caller must provide
 * a comparator function that can be used to order objects that will be
 * kept in the list.
 * 
 * If the function succeeds, it returns a (non-NULL) SortedListT object.
 * Else, it returns NULL.
 *
 */

SortedListPtr SLCreate(CompareFuncT cf);

/*
 * FLCreate creates a new, empty list. 
 */

FileListPtr FLCreate(CompareFuncT cf);

/*
 * SLDestroy destroys a list, freeing all dynamically allocated memory.
 *
 */
void SLDestroy(SortedListPtr list);


/*
 * SLInsert inserts a given object into a sorted list, maintaining sorted
 * order of all objects in the list.  If the new object is equal to a subset
 * of existing objects in the list, then the subset can be kept in any
 * order.
 *
 * If the function succeeds, it returns 1.  Else, it returns 0.
 *
 */

int SLInsert(SortedListPtr list, char *newObj, char * file);


#endif