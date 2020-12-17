#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <semaphore.h>

enum dep {Police,Fire,Ambulance,City};
typedef struct dispachermessage {
    char* message;
    enum dep num; //number of the dep
    struct dispachermessage * next;

} Dispachermessage;
 
Dispachermessage* CreateNode(Dispachermessage* head ,char* message);
Dispachermessage* MoveFromToDoToDoing(Dispachermessage* headSource, Dispachermessage* headDestination);
Dispachermessage* DeleteLastNode(Dispachermessage* head);
Dispachermessage * readmessagefromfile (FILE *fptr);
void print_list (Dispachermessage *head);
void* printpolice();
void* printambulance();
void* printfire ();
void* printcity ();
Dispachermessage* head1 = NULL;
Dispachermessage* head2=NULL;
bool isActive=true; //thread is active
sem_t semcpolice; // semphore.
sem_t semcambulance;
sem_t semcfire;
sem_t semccity;	


int main (void) 
{
	  int i=0;
	  int j=0;
	  char num[2];
      FILE *fmessage;
      fmessage= fopen("messages.txt","r");
      FILE *flog;
      flog= fopen("log.txt","a");
	  pthread_t thread_police;
	  pthread_t thread_fire;
	  pthread_t thread_ambulance;
	  pthread_t thread_city;
	  
	  
	  
	  pthread_create (&thread_police,NULL,&printpolice,NULL);
	  pthread_create (&thread_fire,NULL,&printfire,NULL);
      pthread_create (&thread_ambulance,NULL,&printambulance,NULL);
      pthread_create (&thread_city,NULL,&printcity,NULL);
      
      sem_init(&semcpolice, 0, 6); //counting semphore 
	  sem_init(&semcambulance,0,4);
	  sem_init(&semcfire,0,5);
	  sem_init(&semccity,0,4);
	  
	   
	                     /*       list1= todo list                */                    
	

	  head1=readmessagefromfile (fmessage);
		
		
		
	
	/*                              list2 : doing list                                     */
	for (i=0;i<=37;i++) // read all the lines.
   {
	   char toLogFile[100];
	   for(int j=0;j<100000;j++);
	   if(j=100000)
	   {
			head2= DeleteLastNode(head1);//queue , the first element inserted is the first that comes out.
			strcat (toLogFile,head2->message);
			strcat (toLogFile," ");
			sprintf(num,"%d",head2->num); //convert int to string
			strcat (toLogFile,num);
			strcat (toLogFile,"\n");
			fputs(toLogFile, flog); //put what is in toLogFile to flog
			memset(toLogFile, 0, 100); //clean the string
			memset(num, 0, 2); //clean the n
		}
		for(int z=0;z<100000;z++);
		
	}
	fclose(flog);


//printf ("the first list is:\n"); //todo list
//print_list (head1);

//printf ("the second list is:\n"); //doing list
//print_list (head2);


free (head2); //deletes the second list
head2=NULL; // no address.
isActive=false; //destory threads
sem_destroy(&semcpolice); //destory semphore
sem_destroy(&semcambulance);
sem_destroy(&semccity);
sem_destroy(&semcfire);
return 0;

}

Dispachermessage* CreateNode(Dispachermessage* head ,char* message)
{
	char* splitedmessages;
    Dispachermessage* headTemp = (Dispachermessage*)malloc(sizeof(Dispachermessage));
	if (NULL == headTemp)
	{
		printf ("error in memory allocation\n");
		return NULL;
	}
	splitedmessages=strtok (message," ");
	headTemp->message=malloc(strlen(splitedmessages));
	strcpy(headTemp->message,splitedmessages);
	splitedmessages=strtok (NULL," "); //moves the pointer to the end of the line/second argument. 
	headTemp->num= atoi(splitedmessages); 
    headTemp->next=head;
    return headTemp;

}

Dispachermessage* MoveFromToDoToDoing(Dispachermessage* headSource, Dispachermessage* headDestination)
{
	Dispachermessage* runner = headSource;
	
	if (runner->next == NULL)
	{
		runner->next = headDestination;
	}
	
	return headSource;
}

//the function delets the last node of the fi
Dispachermessage* DeleteLastNode(Dispachermessage* head)
{
	Dispachermessage* prev= head; 
	Dispachermessage* last=prev->next; 
	
	if (prev->next==NULL) //node is the last element in the linked list
	{
		return prev; //release the lest node
	}
	
	//there are more elements before the last element in the linked list
	
	
	while(last->next!=NULL)
	{
		prev=prev->next; //previous
		last=last->next; //current
	}
	
	prev->next=NULL; //disconnects the last node form the first linked list
	return last;
	
}

Dispachermessage * readmessagefromfile (FILE *fptr)
{
    char str[100]; 
    int i=0;
    
	 if(fptr == NULL) {
      perror("Error opening file");
      return NULL;
   }

    for (i=0;i<=37;i++) // read all the lines.
   {
	    fgets (str, 100, fptr); //reads one line each time.
	    head1=CreateNode(head1,str);
	    
	
   }
   fclose(fptr);
  return head1;
	
	
	
}

void print_list (Dispachermessage *head)
{
	Dispachermessage *current= head;
	while (current!=NULL) {
		printf ("%s\n",current->message);
		printf ("%d\n",current->num);
		current=current->next;
	}
}

void* printpolice()
{
	while(isActive)
	{
		if(head2!=NULL && head2->num==Police)
		{
			sem_wait(&semcpolice); //using counting semphore 
			//printf ("this is the police thread\n");
			//printf ("%s\n",head2->message);
			DeleteLastNode(head2);
			sem_post (&semcpolice); //release counting semphore
		}
	}
		
}


void* printfire()
{
	while(isActive)
	{
		if(head2!=NULL && head2->num==Fire)
		{
			sem_wait(&semcfire); //using counting semphore 
			//printf ("this is the fire thread\n");
			//printf ("%s\n",head2->message);
			DeleteLastNode(head2);
			sem_post (&semcfire); //release counting semphore
		}
	}
	
	
	
}


void* printambulance()
{
	while(isActive)
	{
		if(head2!=NULL && head2->num==Ambulance)
		{
			sem_wait(&semcambulance); //using counting semphore 
			//printf ("this is the ambulance thread\n");
			//printf ("%s\n",head2->message);
			DeleteLastNode(head2);
			sem_post (&semcambulance);//release counting semphore
		}
	}
	
	
	
}


void* printcity()
{
	while(isActive)
	{
		if(head2!=NULL && head2->num==City)
		{
			sem_wait(&semccity); //using counting semphore 
			//printf ("this is the city thread\n");
			//printf ("%s\n",head2->message);
			DeleteLastNode(head2);
			sem_post(&semccity); //release counting semphore
			
		}
	}
	
	
}
