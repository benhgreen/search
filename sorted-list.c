#include	<string.h>
#include	"sorted-list.h"
#include	<stdio.h>

int compareStrings(char *p1, char *p2){
		char *s1 = p1;
		char *s2 = p2;

		return strcmp(s1, s2);
}

SortedListPtr SLCreate(CompareFuncT cf){
	SortedListPtr temp = (SortedListPtr)malloc(sizeof(struct SortedList));

	if(temp == NULL || cf == NULL){
		return NULL;
	}

	temp->head = NULL;
	temp->totalcount = 0;
	temp->cmp = cf;

	if(cf == NULL){
		return NULL;
	}

	return temp;
}

FileListPtr FLCreate(CompareFuncT cf){
	FileListPtr temp = (FileListPtr)malloc(sizeof(struct FileList));

	if(temp == NULL || cf == NULL){
		return NULL;
	}

	temp->head = NULL;
	temp->totalcount = 0;
	temp->cmp = cf;

	if(cf == NULL){
		return NULL;
	}

	return temp;
}

void SLDestroy(SortedListPtr list){
	/*traverse all nodes to free them*/
	node *temp = list->head;
	node t = (node)list->head;
	node t_nxt = (node)t->next;
	/*special case if there is only a head*/

	if(t_nxt == NULL){
		free(t->data);
		free (temp);

		return;
	}else{
		/*continuously remove head until none left*/

		while(t->next != NULL && t != NULL){
			list->head = (node*)t->next;
			node *temp = list->head;
			free(temp);
			t = (node)list->head;
		}
		return;
	}
}

int SLInsert(SortedListPtr list, char *newObj, char *file){

	node temp = (node)malloc(sizeof(struct LLnode));
	node_f temp2 = (node_f)malloc(sizeof(struct LLnode_file));

	if(temp == NULL || newObj == NULL){
		return 0;
	}

	int c = strlen(newObj)+1;
	char* tempa = (char*)malloc(sizeof(newObj)*c);
    memcpy(tempa, newObj, sizeof(newObj)*c);

    int c2 = strlen(file)+1;
	char* tempa2 = (char*)malloc(sizeof(file)*c2);
    memcpy(tempa2, file, sizeof(file)*c2);

	temp->data = tempa;
	temp->count = 0;
	temp->next = NULL;
	temp->previous = NULL;

	temp2->file = tempa2;
	temp2->count = 0;
	temp2->next = NULL;

	/*intializes head*/
	if(list->head == NULL){
		temp->count++;
		list->head = (node*)temp;
		list->totalcount++;

		/*initlizes list of files for node*/
		temp2->count++;
		node ptr = (node)list->head;
		ptr->head = (node_f)temp2;

		return 1;
	}else{
		/*first check for duplicate, if there is a duplicate in the list iterate through files
		when file is found, add to counter.*/
		CompareFuncT cmp = (CompareFuncT)list->cmp;
		node t;
		t = (node)list->head;
		/*insert new node that doesn't already exist in list

		this adds the file data to the newly created temp node. if there is no file it adds as head
		if there is already a file it puts the new file in the head position and then it sorts all files and nodes
		before returning*/
		while(t != NULL){
			int cmpval = cmp(t->data,temp->data);
			if(cmpval == 0){
				node_f k;
				k = (node_f)t->head;
				while(k != NULL){
					cmpval = cmp(k->file,temp2->file);
					if(cmpval == 0){
						temp->count++;
						k->count++;
						return 1;
					}else{
						k = k->next;
					}

				}

				/*add file data then sort*/
				temp->count++;
				temp2->count++;
				temp2->next = (node_f)t->head;
				t->head = (node_f)temp2;

				node ptr;
	 			ptr = (node)list->head;

				while(ptr->next != NULL){

					/*sorts file nodes in descending order*/
					node curptr;
					curptr = (node)ptr;
					node_f fileptr;
					fileptr = (node_f)curptr->head;

					while(fileptr->next != NULL){
						int curnum = fileptr->count;
						int nextnum = fileptr->next->count;

						if(nextnum > curnum){
							char* a = (char*) fileptr->file;
							char* b = (char*) fileptr->next->file;
							fileptr->file = b;
							fileptr->next->file = a;
							fileptr->count = nextnum;
							fileptr->next->count = curnum;
						}

						fileptr = fileptr->next;
						if(fileptr == NULL){
							break;
						}
					}
					
					int i = cmp(ptr->data,ptr->next->data);

					/*swaps node and file data for sorting*/
					if(i > 0){
						char* a = (char*) ptr->data;
						char* b = (char*) ptr->next->data;
						ptr->data = b;
						ptr->next->data = a;

						node_f* v = (node_f*) ptr->head;
						node_f* v2 = (node_f*) ptr->next->head;
						ptr->head = (node_f) v2;
						ptr->next->head = (node_f) v;

					}

					ptr = ptr->next;

					if(ptr == NULL){
						return 1;
					}
				}
			return 0;
		}else{
			t = t->next;
			}
		}

		/*the while loop exited without finding a match and returning success (1), so a new node is inserted
		after it is inserted in the head position, the list and files are sorted*/
		temp->count++;
		temp2->count++;

		node ptr;
		ptr = (node)list->head;

		temp->head = (node_f)temp2;
		temp->next = (node)list->head;
		list->head = (node*)temp;
		list->totalcount++;

		ptr = (node)list->head;

		while(ptr->next != NULL){

			/*sorts file nodes in descending order*/
			node curptr;
			curptr = (node)ptr;
			node_f fileptr;
			fileptr = (node_f)curptr->head;

			while(fileptr->next != NULL){
				int curnum = fileptr->count;
				int nextnum = fileptr->next->count;

				if(nextnum > curnum){
					char* a = (char*) fileptr->file;
					char* b = (char*) fileptr->next->file;
					fileptr->file = b;
					fileptr->next->file = a;
					fileptr->count = nextnum;
					fileptr->next->count = curnum;
				}

				fileptr = fileptr->next;
				if(fileptr == NULL){
					break;
				}
			}
			
			int i = cmp(ptr->data,ptr->next->data);
			/*swaps node and file data for sorting*/
			if(i > 0){
				char* a = (char*) ptr->data;
				char* b = (char*) ptr->next->data;
				ptr->data = b;
				ptr->next->data = a;
					
				node_f* v = (node_f*) ptr->head;
				node_f* v2 = (node_f*) ptr->next->head;
				ptr->head = (node_f) v2;
				ptr->next->head = (node_f) v;

			}

			ptr = ptr->next;

			if(ptr == NULL){
				return 1;
			}
		}
	}
	return 1;
}