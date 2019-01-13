#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/inotify.h>

#include <sys/stat.h>

#include <dirent.h>
#include <wait.h>

#include <errno.h>



//#include "NameTree.hpp"
//#include "Inode.hpp"

#include "Mystructs.hpp"


//The fixed size of the event buffer:
#define EVENT_SIZE  ( sizeof (struct inotify_event) )

//The size of the read buffer: estimate 1024 events with 16 bytes per name over and above the fixed size above
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )




const char * target_type(struct inotify_event *event) {
	if( event->len == 0 )
		return "";
	else
		return event->mask & IN_ISDIR ? "directory" : "file";
}

const char * target_name(struct inotify_event *event) {
	return event->len > 0 ? event->name : NULL;
}

const char * event_name(struct inotify_event *event) {

	if (event->mask & IN_ATTRIB)
		return "attrib";
	else if (event->mask & IN_CLOSE_WRITE)
		return "close write";
	else if (event->mask & IN_CREATE)
		return "create";
	else if (event->mask & IN_DELETE)
		return "delete";
	else if (event->mask & IN_DELETE_SELF)
		return "watch target deleted";
	else if (event->mask & IN_MODIFY)
		return "modify";
	else if (event->mask & IN_MOVED_FROM)
		return "moved out";
	else if (event->mask & IN_MOVED_TO)
		return "moved into";
	else
		return "unknown event";


}







void signal_handler( int sig){		// do nothing

//	signal(SIGINT, signal_handler);

	printf("OUCH");

}

































int main(int argc, char* argv[]){

	bool synchronise = true;		// if we will synchronise the two folders

/* If backup doesn't exist, and we create a copy of source using cp , we don't need to synchronise */


	Tree* SourceT = new Tree();		// creating the empty trees "trees"
	Tree* BackupT = new Tree();

	LINode* SourceL = new LINode();
	LINode* BackupL = new LINode();

	/* Check if BACKUP exists */

	DIR* dir;


	if( ( dir = opendir(argv[2]) ) == NULL ){		// if backup dir doesn't exist

		synchronise = false;		// backup will be a copy of source

		/* Copy source dir (and its contents) to backup */


		struct stat temp;

		if( stat(argv[1],&temp)  == -1){

			perror("Error Stat() in mirr (1) : ");
			return -1;

		}

		mkdir(argv[2], temp.st_mode);		//creating the empty dir

		copy1(argv[1],argv[2],'d',true);		// copy argv[1](source) into argv[2](backup)
		// true argument is because we wnat to copy the content of source, not the whole dir

	}else{

		closedir(dir);

	}



	/* Creating the trees */

	printf(" CREATING SOURCE TREE \n\n");
	SourceT->Creation(argv[1],&SourceL,false);		// Filling the trees and i-nodes


	printf(" CREATING BACKUP TREE \n\n");
	BackupT->Creation(argv[2],&BackupL,false);	// Creating Backup Tree


	/* Printing */
	printf("\n\nSTARTED PRINTING SOURCE TREE \n");
	SourceT->Print();

	printf("\n\nSTARTED PRINTING BACKUP TREE \n");
	BackupT->Print();

	printf("\n\nSTARTED PRINTING SOURCE LIST \n\n");
	SourceL->Print();

	printf("\n\nSTARTED PRINTING BACKUP LIST \n");
	BackupL->Print();





	/* "Linking"  INode* copy with its appropriate copy */

	//In other words, we are fixing the INode* copy pointers */ 

	SourceL->CopyPointers(&BackupL,argv[1],argv[2]);

	printf("\n\nNo Problems\n\n");


	/* Check if we need to Synchronise (step 1.2) */

	// if backup was created by the programm => no need to synchronise 

	if( synchronise == true){

		printf( "\n\nSTARTING SYNCHRONISATION \n\n");

		SourceT->Synchronisation(&BackupT, &BackupL);

	}

	printf("\n SYNCHRONISATION DONE \n\n");


	/* Printing */
	printf("\n\nSTARTED PRINTING SOURCE TREE \n");
	SourceT->Print();

	printf("\n\nSTARTED PRINTING BACKUP TREE \n");
	BackupT->Print();

	printf("\n\nSTARTED PRINTING SOURCE LIST \n\n");
	SourceL->Print();

	printf("\n\nSTARTED PRINTING BACKUP LIST \n\n");
	BackupL->Print();




			/* INOTIFY */

	/* Initiate inotify */

	int fd = inotify_init();

	if( fd == -1){

		perror("Couldn't create inotify: ");
		return -1;

	}


	/* Inotify_add_watch all directories */

	List* list = new List();		// list of wd (watch descriptors 

	INode* temp = SourceL->head;

	int wd;

// all the directories, or only the ones under source


	while( temp != NULL){		// watch all the directories, or only the ones under source

		if( temp->mode == 'd'){

			wd = inotify_add_watch( fd, temp->Tlist->head->tnode->name, IN_CREATE | IN_ATTRIB | IN_MODIFY | IN_CLOSE_WRITE | IN_DELETE | IN_DELETE_SELF | IN_MOVED_FROM | IN_MOVED_TO );

			if( wd == -1){

				perror(" Error in inotify_add_watch() (1) : ");
				return -1;

			}else{

				Node* temp1 = new Node(wd,temp->Tlist->head->tnode);

				list->AddNode(temp1);		// add wd to the list

				temp1 = NULL;

			}

		}

		temp = temp->next;

	}

	temp = NULL;


	/* Below is taken from sampleinotify.c ( the read procedure is the same )

		But I have added "what to do" for each event */



	int read_offset = 0; //remaining number of bytes from previous read
	int read_ptr;

	char buffer[EVENT_BUF_LEN];	//the buffer to use for reading the events

	int length;

	int saved_cookie = 0;
	bool previous_moved_from = false;
	int saved_wd = 0;
	char* saved_name = NULL;


//	bool exit = false;



	signal(SIGINT,signal_handler);



	while (1) {

		printf("\n\n\nBlocking on Read() (1) \n\n");

		/* read next series of events */


/*
		length = read(fd, buffer + read_offset, sizeof(buffer) - read_offset);

		if (length < 0){

			if(errno == EINTR)	break;		// if interrupted by a signal break

			perror("Error in read() : ");
			return -1;

		}

*/



		if( ( length = read(fd, buffer + read_offset, sizeof(buffer) - read_offset) ) == -1){

			if(errno == EINTR)	break;		// if interrupted by a signal break

			perror("Error in read() : ");
			return -1;

		}



		length += read_offset; // if there was an offset, add it to the number of bytes to process
		read_ptr = 0;
		
		// process each event
		// make sure at least the fixed part of the event in included in the buffer
		while (read_ptr + EVENT_SIZE <= length ) { 
			//point event to beginning of fixed part of next inotify_event structure
			struct inotify_event *event = (struct inotify_event *) &buffer[ read_ptr ];
			
			// if however the dynamic part exceeds the buffer, 
			// that means that we cannot fully read all event data and we need to 
			// deffer processing until next read completes
			if( read_ptr + EVENT_SIZE + event->len > length ) 
				break;
			//event is fully received, process
			printf("WD:%i %s %s %s COOKIE=%u\n", 
				event->wd, event_name(event), 
				target_type(event), target_name(event), event->cookie);
			//advance read_ptr to the beginning of the next event
			read_ptr += EVENT_SIZE + event->len;



			/* IGNORE ".files" */
	//		const char *check1 = target_name(event);

			/* IGNORE ".files" */
			char* check1 = event->name;

			if( check1[0] == '.') continue;		// we ignore ".files"































		//////////////////////////////////////////
		/////////////////////////////////////////

		/* IMPORTANT */

		//I hypothesize that IN_MOVED_FROM, IN_MOVED_TO might come sequentially (if there is movement in our hierarchy

		// if not => unlink file

		if( ( previous_moved_from == true ) && ( strcmp( event_name(event) , "moved into") != 0)){

			//however if the next event after previous_moved_from isn't IN_MOVED_TO

			// UNLINK FILE

			Node* temp2;

			list->getNode(saved_wd,&temp2);


			if( temp2 == NULL){

				printf("Something is wrong (1), didn't find Node (with watch descriptor %d) \n\n",saved_wd);

				return -1;

			}


			// Create the relative path name to this file

			// Source_dir/event->name

			int length1 = snprintf(NULL,0,"%s/%s",temp2->tnode->name,saved_name) + 1;

			char name[length1];

			snprintf(name,length1, "%s/%s", temp2->tnode->name, saved_name);


			TNode* copydir = temp2->tnode->inode->copy->Tlist->head->tnode;


			// fixing the name: backup_dir/event->name

			length1 = snprintf(NULL,0,"%s/%s", copydir->name, saved_name) + 1;

			char name1[length1];

			snprintf(name1,length1,"%s/%s", copydir->name, saved_name);


			// Remove backup file first 

			unlink(name1);



			TLNode* temp1 = copydir->child->head;

			while( temp1 != NULL){	//search for file-name in backup

				if( strcmp( temp1->tnode->name,name1) == 0) break; 

				temp1 = temp1->next;
			}


			if( temp1 == NULL){

				printf(" Something is wrong (2), temp1 shouldn't be NULL \n\n");
				return -1;

			}


			// Remove backup tnode

			copydir->child->deleteNode(temp1->tnode);



			temp1 = temp2->tnode->child->head;

			while( temp1 != NULL){	// search for file-name in source

				if( strcmp( temp1->tnode->name,name) == 0) break; 

				temp1 = temp1->next;
			}


			if( temp1 == NULL){

				printf(" Something is wrong (3), temp1 shouldn't be NULL \n\n");
				return -1;

			}

			//Remove source tnode

			temp2->tnode->child->deleteNode(temp1->tnode);



			temp1 = NULL;
			temp2 = NULL;
			copydir = NULL;
		}





























			/* Now we update our trees (if necessary) */

			if( strcmp( "create", event_name(event) ) == 0){		// IN_CREATE




				previous_moved_from = false;




				struct stat stat1;
				INode* temp1 = NULL;

				Node* temp2 = NULL;

				list->getNode(event->wd,&temp2);// find the node (from the list) with our wd

				if( temp2 == NULL){

					printf("Something is wrong (4), didn't find Node (with watch descriptor %d) \n\n",event->wd);

					return -1;

				}


				// Create the relative path name to this new file/dir

				// Source_dir/event->name

				int length1 = snprintf(NULL,0,"%s/%s",temp2->tnode->name,event->name) + 1;

				char name[length1];

				snprintf(name,length1, "%s/%s", temp2->tnode->name, event->name);


				if( stat(name, &stat1) == -1){

					perror(" Error in Stat() Inotify (1): ");
					return -1;

				}


				// Search if the inode of the newly created file exists => its a hard link

//				SourceL->getINode(stat1.st_ino,&temp1);






				// check if file or dir

				if( strcmp( target_type(event), "file" ) == 0){		// if its a file


					// Search if the inode of the newly created file exists => its a hard link

					SourceL->getINode(stat1.st_ino,&temp1);

					if( temp1 == NULL){		// if Inode doesn't exist

					// Create Both the INode and TNode in source, as well as in backup

						// SOURCE

					temp1 = new INode(name,SourceL);

					SourceL->AddINode(temp1);

					TNode* temp3 = new TNode(name,temp1,SourceL,temp2->tnode->child);

					temp2->tnode->child->AddNode( temp3);

					temp3 = NULL;


						// BACKUP

					TNode* copydir = temp2->tnode->inode->copy->Tlist->head->tnode;


					// first copy the file to the backup folder

					copy1(name,copydir->name,'f', false);

					// fixing the name: backup_dir/event->name

					length1 = snprintf(NULL,0,"%s/%s", copydir->name, event->name) + 1;

					char name1[length1];

					snprintf(name1,length1,"%s/%s", copydir->name, event->name);


					INode* temp4 = new INode(name1,BackupL);

					BackupL->AddINode(temp4);

					temp3 = new TNode(name1,temp4,BackupL,copydir->child);

					copydir->child->AddNode(temp3);

					// Also link the two inodes

					temp1->copy = temp4;



					temp3 = NULL;
					temp1 = NULL;
					temp4 = NULL;

					copydir = NULL;


				}else{	// if inode exists

					// Create the Tnode in Source, Backup
					// Also create a hard link in backup


					// SOURCE

					TNode* temp3 = new TNode(name,temp1,SourceL,temp2->tnode->child);

					temp2->tnode->child->AddNode( temp3);

					temp3 = NULL;

					//Backup

					TNode* copydir = temp2->tnode->inode->copy->Tlist->head->tnode;


					// fixing the name: backup_dir/event->name

					length1 = snprintf(NULL,0,"%s/%s", copydir->name, event->name) + 1;

					char name1[length1];

					snprintf(name1,length1,"%s/%s", copydir->name, event->name);


					temp3 = new TNode(name1,temp1,BackupL,copydir->child);

					copydir->child->AddNode(temp3);


					// Also create a hard link of the "copy" file

					if( link( temp1->copy->Tlist->head->tnode->name, name1) == -1){

						perror(" Error link() in inotify: ");
						return -1;


					}

					temp3 = NULL;
					temp1 = NULL;
					copydir = NULL;

				}


			}else{	// directory

			// temp1 = will be NULL , because inode of dir has only 1 name pointing to it 
			// so because we have just created it ===> getINode returns NULL



				// Create a TNode (DIR) at source ,
				// Create an INode and a TNode at backup backup

					// SOURCE 


				temp1 = new INode(name,SourceL);

				SourceL->AddINode(temp1);


				TNode* temp3 = new TNode(name,temp1,SourceL,temp2->tnode->child);

				temp2->tnode->child->AddNode(temp3);



					/* Add the directory in the objects under monitoring */

				wd = inotify_add_watch( fd, temp3->name, IN_CREATE | IN_ATTRIB | IN_MODIFY | IN_CLOSE_WRITE | IN_DELETE | IN_DELETE_SELF | IN_MOVED_FROM | IN_MOVED_TO );

				if( wd == -1){

					perror(" Error in inotify_add_watch() (2) : ");
					return -1;

				}else{

	// FIX HERE , temp = NULL ==> WAY ABOVE

//					Node* temp4 = new Node(wd,temp->Tlist->head->tnode);

					Node* temp4 = new Node(wd,temp3);

					list->AddNode(temp4);		// add wd to the list

					temp4 = NULL;

				}


				temp3 = NULL;



				// BACKUP

				TNode* copydir = temp2->tnode->inode->copy->Tlist->head->tnode;

				// first copy the DIR to the backup folder

				copy1(name,copydir->name,'d', false);


				// fixing the name: backup_dir/event->name

				length1 = snprintf(NULL,0,"%s/%s", copydir->name, event->name) + 1;

				char name1[length1];

				snprintf(name1,length1,"%s/%s", copydir->name, event->name);


				INode* temp4 = new INode(name1,BackupL);

				BackupL->AddINode(temp4);

				temp3 = new TNode(name1,temp4,BackupL,copydir->child);

				copydir->child->AddNode(temp3);

				/* Also link the two INodes */

				temp1->copy = temp4;


				temp3 = NULL;
				temp1 = NULL;
				temp4 = NULL;

				copydir = NULL;


			}




		}









		if( strcmp( event_name(event) ,  "attrib" ) == 0 ){


			previous_moved_from = false;







			if( strcmp( "file", target_type(event) ) == 0){

				Node* temp2 = NULL;

				list->getNode(event->wd,&temp2);// find the node (from the list) with our wd

				if( temp2 == NULL){

					printf("Something is wrong (5), didn't find Node (with watch descriptor %d) \n\n",event->wd);

					return -1;

				}


				struct stat stat1;

				// Create the relative path name to this new file/dir

				// Source_dir/event->name

				int length1 = snprintf(NULL,0,"%s/%s",temp2->tnode->name,event->name) + 1;

				char name[length1];

				snprintf(name,length1, "%s/%s", temp2->tnode->name, event->name);


				if( stat(name, &stat1) == -1){

					perror(" Error in Stat() Inotify (2): ");
					return -1;

				}

				INode* temp1;

				SourceL->getINode( stat1.st_ino, &temp1);


				if( stat1.st_mtime != temp1->last_mod){

					temp1->last_mod = stat1.st_mtime;		// Update Source

					temp1->copy->last_mod = stat1.st_mtime;		// Update Backup

				}

				temp1 = NULL;
				temp2 = NULL;

			}
		}








		if( strcmp( event_name(event) ,  "modify" ) == 0 ){		//	IN_MODIFY


			if( strcmp( target_type(event) ,  "file" ) == 0 ){// if it is a file

				Node* temp2 = NULL;

				list->getNode(event->wd,&temp2);// find the node (from the list) with our wd

				if( temp2 == NULL){

					printf("Something is wrong (6), didn't find Node (with watch descriptor %d) \n\n",event->wd);

					return -1;

				}

				// Create the relative path name to this new file/dir

				// Source_dir/event->name

				int length1 = snprintf(NULL,0,"%s/%s",temp2->tnode->name,event->name) + 1;

				char name[length1];

				snprintf(name,length1, "%s/%s", temp2->tnode->name, event->name);


				/* Check if exists in modified TList of our Node */

				TLNode* temp3 = temp2->modified->head;

				while( temp3 != NULL){

					if( strcmp( name, temp3->tnode->name ) == 0) break;

					temp3 = temp3->next;

				}


				if( temp3 == NULL){			// name isn't in modified

					// find the TNode which has TNode->name = name

					TLNode* temp4 = temp2->tnode->child->head;

					while( temp4 != NULL){

						if( strcmp(temp4->tnode->name, name) == 0) break;

						temp4 = temp4->next;

					}

					if( temp4 == NULL){

						printf("Something went wrong in inotify, temp4 shouldn't be NULL (2) \n");
						return -1;
					}

					// temp4 != NULL

					temp2->modified->AddNode(temp4->tnode);		// add to the list of modified

					temp4 = NULL;

				}
				//else if already in modified => do nothing

				temp3 = NULL;
				temp2 = NULL;

			}

		}














		if( strcmp( event_name(event) ,  "close write" ) == 0 ){		//	IN_CLOSE_WRITE


			previous_moved_from = false;



			if( strcmp( target_type(event) ,  "file" ) == 0 ){// if it is a file

				Node* temp2 = NULL;

				list->getNode(event->wd,&temp2);// find the node (from the list) with our wd

				if( temp2 == NULL){

					printf("Something is wrong (7), didn't find Node (with watch descriptor %d) \n\n",event->wd);

					return -1;

				}

				// Create the relative path name to this file

				// Source_dir/event->name

				int length1 = snprintf(NULL,0,"%s/%s",temp2->tnode->name,event->name) + 1;

				char name[length1];

				snprintf(name,length1, "%s/%s", temp2->tnode->name, event->name);


				/* Check if exists in modified TList of our Node */

				TLNode* temp3 = temp2->modified->head;

				while( temp3 != NULL){

					if( strcmp( name, temp3->tnode->name ) == 0) break;

					temp3 = temp3->next;

				}


				if( temp3 == NULL){

					temp2 = NULL;
					temp3 = NULL;

					continue;		// is not modified => do nothing
				}
				//else, copy the file to backup


				TNode* copydir = temp2->tnode->inode->copy->Tlist->head->tnode;

				// first copy the file to the backup folder

				copy1(name,copydir->name,'f', false);


				// fixing the name: backup_dir/event->name

				length1 = snprintf(NULL,0,"%s/%s", copydir->name, event->name) + 1;

				char name1[length1];

				snprintf(name1,length1,"%s/%s", copydir->name, event->name);

				//Update the inodes 

				struct stat stat1;

				if( stat(name,&stat1) == -1){

					perror(" Error in Stat() inotify (3)");
					return -1;
				}


				INode* temp1 = NULL;

				SourceL->getINode(stat1.st_ino,&temp1);

				if( temp1 == NULL ){

					printf("Error something is wrong (8)");
					return -1;

				}

					// SIZE

				temp1->size = stat1.st_size;
				temp1->copy->size = stat1.st_size;

					// Modification time

				temp1->last_mod = stat1.st_mtime;
				temp1->copy->last_mod = stat1.st_mtime;

				temp1 = NULL;
				copydir = NULL;
				temp3 = NULL;
				temp2 = NULL;

			}

		}















		if( strcmp( event_name(event) ,  "delete" ) == 0 ){		//	IN_DELETE


			previous_moved_from = false;




			if( strcmp( target_type(event) ,  "file" ) == 0 ){// if it is a file

				Node* temp2 = NULL;

				list->getNode(event->wd,&temp2);// find the node (from the list) with our wd

				if( temp2 == NULL){

					printf("Something is wrong (9), didn't find Node (with watch descriptor %d) \n\n",event->wd);

					return -1;

				}

				// Create the relative path name to this file

				// Source_dir/event->name

				int length1 = snprintf(NULL,0,"%s/%s",temp2->tnode->name,event->name) + 1;

				char name[length1];

				snprintf(name,length1, "%s/%s", temp2->tnode->name, event->name);


				TNode* copydir = temp2->tnode->inode->copy->Tlist->head->tnode;


				// fixing the name: backup_dir/event->name

				length1 = snprintf(NULL,0,"%s/%s", copydir->name, event->name) + 1;

				char name1[length1];

				snprintf(name1,length1,"%s/%s", copydir->name, event->name);






				/* UNLINK the FILE (from backup)

				if( unlink(name1) == -1){

					perror(" Error in unlink() in inotify (1) ");
					return -1;

				}

	*/

				// delete TNode in Backup first

				struct stat stat1;

//				if( stat( name, &stat1) == -1){

				if( stat( name1, &stat1) == -1){

					perror("Error in Stat() in inotify (4) ");
					return -1;

				}


				INode* temp1;
//				SourceL->getINode(stat1.st_ino, &temp1);

				BackupL->getINode(stat1.st_ino, &temp1);

				if( temp1 == NULL){

					printf("Something is wrong (10) in inotify,temp1 shouldn't be NULL (2)");
					return -1;

				}
						//Searching the Tnode(in backup) with TNode->name = name1
//				TLNode* temp3 = temp1->copy->Tlist->head;

				TLNode* temp3 = temp1->Tlist->head;
				while( temp3 != NULL){

					if( strcmp( temp3->tnode->name, name1) == 0) break;

					temp3 = temp3->next;

				}


				if( temp3 == NULL){

					printf("Something is wrong (11) in inotify,temp3 shouldn't be NULL (3)");
					return -1;

				}

						// Removing the TNode(backup)

//				TNode* copydir = temp2->tnode->inode->copy->Tlist->head->tnode;

				copydir->child->deleteNode( temp3->tnode);









				// Removing the TNode(source)

//				temp3 = temp1->Tlist->head;

				temp3 = temp2->tnode->child->head;
				while( temp3 != NULL){

					if( strcmp( temp3->tnode->name, name) == 0) break;

					temp3 = temp3->next;

				}


				if( temp3 == NULL){

					printf("Something is wrong (12) in inotify,temp3 shouldn't be NULL (4)");
					return -1;

				}

				temp2->tnode->child->deleteNode( temp3->tnode);


				// UNLINK the FILE (from backup)

				if( unlink(name1) == -1){

					perror(" Error in unlink() in inotify (1) ");
					return -1;

				}




				temp3 = NULL;
				copydir = NULL;
				temp1 = NULL;
				temp2 = NULL;


			}
		}































		if( strcmp( event_name(event) ,  "watch target deleted" ) == 0 ){	//	IN_DELETE_SELF


			previous_moved_from = false;





//			*saved_cookie = 0;


			Node* temp2 = NULL;

			list->getNode(event->wd,&temp2);// find the node (from the list) with our wd

			if( temp2 == NULL){

				printf("Something is wrong (13), didn't find Node (with watch descriptor %d) \n\n",event->wd);

				return -1;

			}

			// Create the relative path name to this file

			// Source_dir/event->name

			int length1 = snprintf(NULL,0,"%s/%s",temp2->tnode->name,event->name) + 1;

			char name[length1];

			snprintf(name,length1, "%s/%s", temp2->tnode->name, event->name);


			printf("%s\n\n",temp2->tnode->name);

			if( temp2->tnode->inode->copy->Tlist->head == NULL) printf("\n\nOUCH\n\n");

			TNode* copydir = temp2->tnode->inode->copy->Tlist->head->tnode;


			// fixing the name: backup_dir/event->name

			length1 = snprintf(NULL,0,"%s/%s", copydir->name, event->name) + 1;

			char name1[length1];

			snprintf(name1,length1,"%s/%s", copydir->name, event->name);



	// CHECK
			// BACKUP

			if( strcmp( copydir->name, basename(argv[2]) ) == 0){// if copydir == BackupT->root

				// Removing Backup Root Folder ==> REMOVE EVERYTHING


				printf("\n\n IN_DELETE_SELF DELETING BACKUP TREE and SOURCE TREE\n\n\n");

				Removedir(name1);		// deleting backup folder

				SourceT->RemoveWD( fd, &list,false);

				if( close(fd) == -1){

					perror("Couldn't close Inotify: (1) ");
					return -1;

				}

				delete BackupT;
				delete SourceT;

				delete SourceL;
				delete BackupL;

				return 0;

			}

			//Remove the copydir(backup)
			copydir->parent->deleteNode(copydir);		// parent is a TList


			// SOURCE

			// Recursively remove_watch_descriptors (and delete Nodes from List)

			Tree* temp1 = (Tree*) temp2->tnode->inode->Tlist->head;

			temp1->RemoveWD(fd,&list,false);

			//Now, remove the dir (source)
			temp2->tnode->parent->deleteNode(temp2->tnode);		// parent is a TList

			temp2->tnode = NULL;
			list->deleteNode(temp2);


			temp1 = NULL;
			temp2 = NULL;
			copydir = NULL;



		}









			/* ONLY FOR FILES */

		if( strcmp( event_name(event) ,  "moved out" ) == 0 ){	//	IN_MOVED_FROM

			if( strcmp( target_type(event), "file") == 0){

				printf("Saving cookie \n\n");

				saved_cookie = event->cookie;

				previous_moved_from = true;

				saved_wd = event->wd;

				int length = strlen(event->name) +1;

				//if saved_name_file already points to some char array
				if( saved_name != NULL) delete[] saved_name;

				saved_name = new char[length];

				strcpy(saved_name, event->name);

			}
		}

























			/* ONLY FOR FILES */
		if( strcmp( event_name(event) ,  "moved into" ) == 0 ){	//	IN_MOVED_TO

			if( strcmp( target_type(event), "file") == 0){

				if( (previous_moved_from == true) && ( event->cookie == saved_cookie)){


					// First , get our Nodes for both events ( IN_MOVED_FROM and IN

					Node* temp1 = NULL;		// IN_MOVED_TO

					list->getNode(event->wd,&temp1);// find the node (from the list) with our wd

					if( temp1 == NULL){

						printf("Something is wrong (14), didn't find Node (with watch descriptor %d) \n\n",event->wd);

						return -1;

					}


					Node* temp2 = NULL;		// IN_MOVED_FROM

					list->getNode(saved_wd,&temp2);// find the node (from the list) with our wd

					if( temp2 == NULL){

						printf("Something is wrong(15), didn't find Node (with watch descriptor %d) \n\n",event->wd);

						return -1;

					}




					TNode* copydir = temp2->tnode->inode->copy->Tlist->head->tnode;

					TNode* copydir1 = temp1->tnode->inode->copy->Tlist->head->tnode;


					// fixing the name: source_dir/saved_name	(CURRENT)

					int length1 = snprintf(NULL,0,"%s/%s", temp2->tnode->name, saved_name) + 1;

					char name[length1];

					snprintf(name,length1,"%s/%s", temp2->tnode->name, saved_name);


					// fixing the name: source_dir1/event->name	(NEW)

					length1 = snprintf(NULL,0,"%s/%s", temp1->tnode->name, event->name) + 1;

					char name1[length1];

					snprintf(name1,length1,"%s/%s", temp1->tnode->name, event->name);






					// fixing the name:backup_dir/saved_name	(CURRENT)

					length1 = snprintf(NULL,0,"%s/%s", copydir->name, saved_name) + 1;

					char name2[length1];

					snprintf(name2,length1,"%s/%s", copydir->name, saved_name);


					// fixing the name: backup_dir1/event->name	(NEW)

					length1 = snprintf(NULL,0,"%s/%s", copydir1->name, event->name) + 1;

					char name3[length1];

					snprintf(name3,length1,"%s/%s", copydir1->name, event->name);



					printf(" \n\n EVENT NAME IS %s \n\n", event->name);





					struct stat stat1;


					if( stat(name1, &stat1) == -1){

						perror(" Error in Stat() Inotify (6): ");
						return -1;

					}






					INode* temp3;

					SourceL->getINode(stat1.st_ino,&temp3);

					if( temp3 == NULL){

						printf("Something is wrong (16), temp3 shouldn't be NULL\n\n");
						return -1;

					}




					// BACKUP

					// temp3 inode has currently Tnode->name = name pointing to it

					// temp3 inode ===> source INode

					TLNode* temp4 = temp3->copy->Tlist->head;

					while( temp4 != NULL){	//search the original backup TNode

						if( strcmp( temp4->tnode->name, name2) == 0) break;

						temp4 = temp4->next;
					}

					if( temp4 == NULL){

						printf("Something is wrong (17), temp3 shouldn't be NULL\n\n");
						return -1;

					}

					// Found the Tnode

					// Copy the original backup file to its new destination

//					copy1(name2, copydir1->name, 'f', false);

//					copy1(name2, name3, 'f', false);

//					unlink(name2);		// delete old backup

					rename(name2,name3);		// change the path from name2 ==> name3


					// Create the new backup TNode 
// (and INode) because I noticed that during cp the resulting file had different inode than the original 

				





					TNode* temp5 = new TNode(name3, temp4->tnode->inode, BackupL, copydir1->child);

					copydir1->child->AddNode(temp5);

					// Delete the original TNode

					copydir->child->deleteNode(temp4->tnode);
					temp4 = NULL;



					// SOURCE

					temp4 = temp3->Tlist->head;

					while( temp4 != NULL){	//search the original backup TNode

						if( strcmp( temp4->tnode->name, name) == 0) break;

						temp4 = temp4->next;
					}

					if( temp4 == NULL){

						printf("Something is wrong (17), temp3 shouldn't be NULL\n\n");
						return -1;

					}

					// Found the Tnode

					// Create the new source TNode

					TNode* temp6 = new TNode(name1, temp4->tnode->inode, BackupL, temp1->tnode->child);

					temp1->tnode->child->AddNode(temp6);

					// Delete the original TNode

					temp2->tnode->child->deleteNode(temp4->tnode);
					temp4 = NULL;


					temp1 = NULL;
					temp2 = NULL;
					temp3 = NULL;
					temp4 = NULL;
					temp5 = NULL;
					temp6 = NULL;

					copydir = NULL;
					copydir1 = NULL;


				}else{		// otherwise act as IN_CREATE


					struct stat stat1;
					INode* temp1 = NULL;

					Node* temp2 = NULL;

					list->getNode(event->wd,&temp2);// find the node (from the list) with our wd

					if( temp2 == NULL){

						printf("Something is wrong (19), didn't find Node (with watch descriptor %d) \n\n",event->wd);

						return -1;

					}

					// Create the relative path name to this new file/dir

					// Source_dir/event->name

					int length1 = snprintf(NULL,0,"%s/%s",temp2->tnode->name,event->name) + 1;

					char name[length1];

					snprintf(name,length1, "%s/%s", temp2->tnode->name, event->name);


					if( stat(name, &stat1) == -1){

						perror(" Error in Stat() Inotify (1): ");
						return -1;

					}


					// Search if the inode of the newly created file exists => its a hard link

//					SourceL->getINode(stat1.st_ino,&temp1);


					// check if file or dir (already checked , care only for file

//					if( strcmp( target_type(event), "file" ) == 0){		// if its a file


						// Search if the inode of the newly created file exists => its a hard link

					SourceL->getINode(stat1.st_ino,&temp1);







					if( temp1 == NULL){		// if Inode doesn't exist

					// Create Both the INode and TNode in source, as well as in backup

					// SOURCE

						temp1 = new INode(name,SourceL);

						SourceL->AddINode(temp1);

						TNode* temp3 = new TNode(name,temp1,SourceL,temp2->tnode->child);

						temp2->tnode->child->AddNode( temp3);

						temp3 = NULL;


							// BACKUP

						TNode* copydir = temp2->tnode->inode->copy->Tlist->head->tnode;


						// first copy the file to the backup folder

						copy1(name,copydir->name,'f', false);

						// fixing the name: backup_dir/event->name

						length1 = snprintf(NULL,0,"%s/%s", copydir->name, event->name) + 1;

						char name1[length1];

						snprintf(name1,length1,"%s/%s", copydir->name, event->name);


						INode* temp4 = new INode(name1,BackupL);

						BackupL->AddINode(temp4);

						temp3 = new TNode(name1,temp4,BackupL,copydir->child);

						copydir->child->AddNode(temp3);

						// Also link the two inodes

						temp1->copy = temp4;



						temp3 = NULL;
						temp1 = NULL;
						temp4 = NULL;

						copydir = NULL;





					}else{	// if inode exists

						// Create the Tnode in Source, Backup
						// Also create a hard link in backup


						// SOURCE

						TNode* temp3 = new TNode(name,temp1,SourceL,temp2->tnode->child);

						temp2->tnode->child->AddNode( temp3);

						temp3 = NULL;

						//Backup

						TNode* copydir = temp2->tnode->inode->copy->Tlist->head->tnode;


						// fixing the name: backup_dir/event->name

						length1 = snprintf(NULL,0,"%s/%s", copydir->name, event->name) + 1;

						char name1[length1];

						snprintf(name1,length1,"%s/%s", copydir->name, event->name);


						temp3 = new TNode(name1,temp1,BackupL,copydir->child);

						copydir->child->AddNode(temp3);


						// Also create a hard link of the "copy" file

						if( link( temp1->copy->Tlist->head->tnode->name, name1) == -1){

							perror(" Error link() in inotify: ");
							return -1;


						}

						temp3 = NULL;
						temp1 = NULL;
						copydir = NULL;

					}


				}

			}


			previous_moved_from = false;

		}


































			//advance read_ptr to the beginning of the next event
//			read_ptr += EVENT_SIZE + event->len;

		}

//		if( BackupT == NULL) break;		// THe Backup Tree is empty
		//probably from many IN_DELETE_SELF or IN_DELETE



		//check to see if a partial event remains at the end
		if( read_ptr < length ) {
			//copy the remaining bytes from the end of the buffer to the beginning of it
			memcpy(buffer, buffer + read_ptr, length - read_ptr);
			//and signal the next read to begin immediatelly after them			
			read_offset = length - read_ptr;
		} else
			read_offset = 0;

	}
























	/* Closing INotify */


	if( close(fd) == -1){

		perror("Couldn't close Inotify: (2) ");
		return -1;

	}


	printf("Deleting Source Tree \n\n");
	delete SourceT;

	printf("Deleting Backup Tree \n\n");
	delete BackupT;

	printf("Deleting Source List \n\n");
	delete SourceL;


	printf("Deleting Backup List \n\n");
	delete BackupL;


	return 0;

}




















