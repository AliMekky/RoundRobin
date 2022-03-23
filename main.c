#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 * Process: Each process has a name and has an execution time.
 *          We keep track of how long the process still needs
 *          to run on the processor. "remaining_time" is the
 *          remaining execution time of the process.
 */
typedef struct
{
    char *name;
    int starting_time;
    int remaining_time;
} Process;
/*
 *
 */
typedef struct Node
{
    Process data;
    struct Node* next;
} Node;
/*
 *
 */
typedef struct
{
    Node* Head;
    Node* Tail;
} Queue;
/*
 *
 */

Process processConstruct(char* name,int startingTime,int remainingTime)
{
    Process p;
    p.name = (char*)malloc(sizeof(strlen(name)+1));
    strcpy(p.name,name);
    p.starting_time = startingTime;
    p.remaining_time = remainingTime;
    return p;
}


Node* constructNode(Process p)
{
    Node* n = malloc(sizeof(Node));
    n->next = NULL;
    n->data = p;
    return n;
}


Queue* init()
{
    Queue* q = malloc(sizeof(Queue));
    q->Head = NULL;
    q->Tail = NULL;
    return q;
}
/*
 *
 */
int isEmpty(Queue *q)
{
    return q->Head == NULL?1:0;

}
/*
 *
 */
Process dequeue(Queue *q)
{

    Process x = q->Head->data;
    Node* temp = q->Head;
    q->Head = temp->next; // q->Head = q->Head->next
    free(temp); // free the node pointer
    if(q->Head == NULL)
        q->Tail = NULL;
    return x;
}


/*
 *
 */
void enqueue(Queue *q, Process x)
{
    Node* n = constructNode(x);
    if(isEmpty(q))
    {
        q->Head = n;
        q->Tail = n;
    }
    else
    {
        q->Tail->next = n;
        q->Tail = n;
    }
}
/*
 *
 */
void destroy(Queue *q)
{
    while(!isEmpty(q))
    {
        Process temp = dequeue(q);
        free(temp.name);
        // delete all elements in the queue
    }
    free(q); // destroy the queue itself
}
/*
 * RoundRobin Scheduling
 */
void RoundRobin(char* filename)
{
    FILE* f = fopen(filename,"r");
    int noLines=0,timeSlots;
    char line[50];
    int startingTime,remainingTime;
    char name[10];

    Queue* q = init();


    while(!feof(f))     // counting number of lines.
    {
        fgets(line,50,f);
        noLines++;
    }

    Process Processes[noLines--]; //initializing array of processes
    //with size equal to number of processes in the file

    fseek(f,22,SEEK_SET);         //moving the cursor to read the time slots
    fscanf(f,"%d",&timeSlots);

    int i=0,j;
    while(!feof(f))              // storing the data in the array
    {
        fscanf(f,"%s %d %d",name,&startingTime,&remainingTime);
        Processes[i] = processConstruct(name,startingTime,remainingTime);
        i++;
    }

    for(i=0; i<timeSlots; i++)
    {
        for(j=0; j<noLines; j++) // check if new process has to be added
        {
            if(Processes[j].starting_time == i)
                enqueue(q,Processes[j]);
        }
        if(isEmpty(q))   //if the queue is empty prints idle
            printf("idle (%d-->%d)\n",i,i+1);
        else
        {
            Process temp = dequeue(q); // if the queue is not empty dequeue the process at the head to be operated
            printf("%s (%d-->%d) ",temp.name,i,i+1);    //prints the process
            if(--temp.remaining_time>0)
            {
                enqueue(q,temp);        // if the remaining time is bigger than zero enqueue the process again
                printf("\n");
            }
            else
            {
                printf("%s aborts\n",temp.name); // if the remaining time is zero prints aborts
                free(temp.name);
            }
        }
    }
    printf("Stop\n");
    destroy(q);
}
/*
 *
 */
int main()
{
    char filename[261];
    puts("Enter file name or Ctrl+Z to exit:");
    puts("----------------------------------");
    while(fgets(filename, 260, stdin) != NULL)
    {
        filename[strlen(filename)-1]='\0';
        if(fopen(filename,"r"))
            RoundRobin(filename);
        else
        {
            puts("File Not Found!");
            puts("----------------------------------");
        }
        puts("Enter file name or Ctrl+Z to exit:");
        puts("----------------------------------");
    }

    return 0;
}

