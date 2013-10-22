#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "sorted-list.h"
#include "sorted-list.c"
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <ftw.h>
#include <errno.h>
#include <unistd.h>

SortedListPtr mainList;

void operate(char* filename, SortedListPtr mainList, char* fullpath);

void print(SortedListPtr p){
	node t;
	t = (node)mainList->head;
	while(t != NULL){
		printf("%s\n",t->data);
		node_f k;
		k = (node_f)t->head;
		while(k != NULL){
			printf("file: %s count: %d\n\n",k->file,k->count);
			k = k->next;
		}
		t = t->next;
	}
}

void printFile(SortedListPtr p, char* filename){

	FILE *wp;
	wp = fopen(filename, "w");

	node t;
	t = (node)mainList->head;
	while(t != NULL){
		fprintf(wp, "<list> %s\n", t->data);
		node_f k;
		k = (node_f)t->head;
		while(k != NULL){
			fprintf(wp, "%s %d ", k->file,k->count);
			k = k->next;
		}
		fprintf(wp, "\n</list>\n");
		t = t->next;

	}


	//fclose(wp);


}

void folderThings(char* foldername,char*folderpath,int depth,SortedListPtr mainList){

	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;
	if((dp = opendir(foldername)) == NULL){
		fprintf (stderr, "cannot open directory: %s\n",foldername);
		return;
	}
	chdir(foldername);
	//iterate through files, call self if subdirectory is encountered
	while((entry = readdir(dp)) != NULL){
		lstat(entry->d_name,&statbuf);
		if(S_ISDIR(statbuf.st_mode)){

			//this is a subdirectory!

			if((strcmp(".",entry->d_name) == 0) ||
				strcmp("..",entry->d_name) == 0)
				continue;
			//operate(entry->d_name,mainList);//printf("%*s%s/\n", depth,"entry->d_name");

			char* nullbytessuck = strdup(folderpath);

			nullbytessuck = realloc(nullbytessuck,sizeof(nullbytessuck)+sizeof(entry->d_name)+2);
			strcat(nullbytessuck, entry->d_name);
			strcat(nullbytessuck,"\\");

			//printf("new folder path: %s\n", nullbytessuck);

			folderThings(entry->d_name,nullbytessuck,depth+4,mainList);

			free(nullbytessuck);
			//shorten!
		}else{

			//this is a file!

			char* argh = strdup(folderpath);

			argh = realloc(argh,sizeof(argh)+sizeof(entry->d_name)+2);
			strcat(argh, entry->d_name);

			//printf("path: %s\n", argh);

			operate(entry->d_name,mainList,argh);

			free(argh);

			}//printf("%*s%s\n", depth, "", entry->d_name);
	}
	chdir("..");
	//closedir(dp);

}

void operate(char* filename, SortedListPtr mainList, char* fullpath){


		FILE *fp;

		fp = fopen(filename, "r");

		//printf("operating on %s\n", filename);

		if(fp == NULL){
			printf("%s is null (%s) :(\n", filename,strerror(errno));
			exit(EXIT_FAILURE);
		}

		struct stat st;
		stat(filename, &st);
		int filesize = st.st_size;

		if(S_ISDIR(st.st_mode)){

			int len = (int)strlen(filename);

			//printf("char %c\n", filename[len-1]);

			char* folderpath = malloc(sizeof(filename)+2);

			if(filename[len-1] == '/'){
				//printf("oh no slash detected\n");
				strncpy(folderpath,filename,strlen(filename)-1);
			}else{
				strcpy(folderpath,filename);
			}

			strcat(folderpath,"\\");

			folderThings(filename,folderpath,0,mainList);

			return;

		}else{

			if(fullpath == NULL){
				fullpath = strdup(filename);
			}
		}

		//reads in file into char array
		char tempstr[filesize+1];
		fread(tempstr, filesize+1, 1, fp);

		/*
		* Has three counters to iterate through array containing file and pick out only [A-Z]/[a-z]/[0-9]
		* tempc is main counter, used for seeing if the end of the array has been reached. fcnt keeps place at beginning
		* alpha/digit element and bcnt is increased until an element is found that is not alpha/digit signifying that the
		* token is ready for hashing.
		*/
		int tempc = 0;
		int fcnt = 0;
		int bcnt = 0;

		while(tempc != filesize){
			if(isalpha(tempstr[bcnt]) || isdigit(tempstr[bcnt])){
				bcnt++;
			}else{
				if((bcnt-fcnt) > 0 && fcnt != bcnt){
					char subbuff[bcnt-fcnt];
					memcpy(subbuff, &tempstr[fcnt], bcnt-fcnt);
					subbuff[bcnt-fcnt] = '\0';

					//convert to lowercase
					int counter = 0;
					//char tempc;


					while(subbuff[counter]){
						subbuff[counter] = tolower(subbuff[counter]);
						counter++;
					}

					//printf("about to print pathname higher up\n");

					//printf("%s\n", fullpath);


					SLInsert(mainList,subbuff,fullpath);

					bcnt++;
					fcnt = bcnt;
				}else{
					fcnt++;
					bcnt++;
				}
			}
			tempc++;
		}

		//accounts for last word in array
		if((bcnt-fcnt) > 0 && fcnt != bcnt){
			char subbuff[bcnt-fcnt];
			memcpy(subbuff, &tempstr[fcnt], bcnt-fcnt);
			subbuff[bcnt-fcnt] = '\0';
			int counter = 0;
			//char tempc;

			while(subbuff[counter]){
				subbuff[counter] = tolower(subbuff[counter]);
				counter++;
			}


			//printf("about to print pathname\n");

			//printf("%s\n", fullpath);

			

			SLInsert(mainList,subbuff,fullpath);
			//printf("successfully read %s\n", filename);
		}
	}

int main(int argc, char** argv){

	//char ch;

	if (argc != 3){
		printf("\nproper usage: ./index <output> <input>'\n\n");
		exit(EXIT_SUCCESS);
	}

	mainList = SLCreate(compareStrings);

	FILE* fp = fopen(argv[1],"r");
	if(fp != NULL){

		char a;
 
		printf("\noutput file already exists, please enter o to overwrite\n");
		
		scanf("%c", &a);

		if(a != 'o'){

			fclose(fp);
			exit(EXIT_SUCCESS);
		}
	}

	fclose(fp);

	operate(argv[2], mainList, NULL);

	printFile(mainList, argv[1]);

	SLDestroy(mainList);

	return 0;
}
