#include"headers.h"


struct node
{
	Process data;
	struct node *next;
	int priority;
};
typedef struct node node;

struct queue
{
	int count;
	node*front;
	node*rear;
};
typedef struct queue queue;
queue* initialize()
{
	queue*q=(queue*)malloc(sizeof(queue));
	q->count=0;
	q->front=NULL;
	q->rear=NULL;
}
bool isempty(queue*q)
{	
	return(q->rear == NULL || q->front == NULL);
}

void enqueue(queue*q,Process value)
{
	node*temp;
	temp=(node*)malloc(sizeof(node));
	temp->data=value;
	temp->next=NULL;
	if(!isempty(q)){
		q->rear->next=temp;
		q->rear=temp;
	}
	else
	{
		q->front=q->rear=temp;
	}
	q->count++;
}

void dequeue(queue*q)
{	
	node*temp=q->front;
	if(q->front->next !=NULL)
	{	
		
		q->front= q->front->next;	
	}
	else
	{
		q->front=NULL;
		q->rear=NULL;
	}
	free(temp);
	q->count--;
}
void prioritypush(node** Head,Process data,int priority)
{
	node* newNode = (node*)malloc(sizeof(node)); 
    newNode->data = data;
    newNode->priority = priority;
    newNode->next = NULL;
    if ((*Head) == NULL)
    {
    
        (*Head) = newNode;
        
    }
    
    
    else if ((*Head) ->priority > newNode->priority)
    {
    	
        newNode->next = (*Head) ;
        (*Head)  = newNode;
    }
    else
    {
    
        node* temp = (*Head) ;
        while(temp->next != NULL)
		{
			
			if(newNode->priority <= temp->next->priority)
			{
				newNode->next = temp->next;
				temp->next = newNode;
				break;
			}
			temp = temp->next;
		}
    }
}

void pop(node** head)
{
    node* temp = *head;
    *head = (*head)->next;
    free(temp);
}


void enqueuePriority(Process *newP,queue* q)
{
    if (q->front==NULL||q->rear==NULL)
    {         printf("\n  The que is empty \n") ;

        node* newN=(node*)malloc(sizeof(node));
        newN->data=*newP;
        newN->next=NULL;
        q->front=newN;
        q->rear=newN;      
    }
    else 
    {
         printf("\n  The que is not empty \n") ;
        node* tempCurrent=(node*)malloc(sizeof(node));
        node* newN=(node*)malloc(sizeof(node));
        newN->data=*newP;

        node* previuos=(node*)malloc(sizeof(node));

        tempCurrent=q->front;
        previuos=tempCurrent;

        while (tempCurrent!=NULL)
        {
            if ((newN->data.priority) < (tempCurrent->data.priority))
            {
            printf("\n  I have the Highest Priority :) \n") ;
              
              newN->next=tempCurrent;
              
              if (tempCurrent==q->front)
              {
                  q->front=newN;
              }
              else 
              {
                previuos->next=newN;
              }
             return;
            }
            else 
            {
                printf("\n  There is another Process that has higher priority \n") ;

                if (tempCurrent->next!=NULL)
                {
                    previuos=tempCurrent;
                tempCurrent=tempCurrent->next;
                }
                else 
                {  printf("\n  I am the last one :( \n") ;

                    tempCurrent->next=newN;
                    newN->next=NULL;
                    q->rear=newN;
                    tempCurrent=newN->next;
                }
            }
        }


    }

}

void printPriority(queue* q)
{
      node* tempCurrent=(node*)malloc(sizeof(node));


        tempCurrent=q->front;

        while (tempCurrent!=NULL)
        {
            printf("\nmy priority is %d  \n" ,tempCurrent->data.priority);
            tempCurrent=tempCurrent->next;
        }  

}

void enqueueRemaining(Process *newP,queue* q)
{
    if (q->front==NULL||q->rear==NULL)
    {
        node* newN=(node*)malloc(sizeof(node));
        newN->data=*newP;
        newN->next=NULL;
        q->front=newN;
        q->rear=newN;      
    }
    else 
    {
         printf("\n  The que is not empty \n") ;
        node* tempCurrent=(node*)malloc(sizeof(node));
        node* newN=(node*)malloc(sizeof(node));
        newN->data=*newP;

        node* previuos=(node*)malloc(sizeof(node));

        tempCurrent=q->front;
        previuos=tempCurrent;

        while (tempCurrent!=NULL)
        {
            if ((newN->data.remainingtime) < (tempCurrent->data.remainingtime))
            {
            printf("\n  I have the Shortest Runtime :) \n") ;
              
              newN->next=tempCurrent;
              
              if (tempCurrent==q->front)
              {
                  q->front=newN;
              }
              else 
              {
                previuos->next=newN;
              }
             return;
            }
            else 
            {
                printf("\n  There is another Process that has shortest RunTime \n") ;

                if (tempCurrent->next!=NULL)
                {
                    previuos=tempCurrent;
                tempCurrent=tempCurrent->next;
                }
                else 
                {  printf("\n  I am the last one :( \n") ;

                    tempCurrent->next=newN;
                    newN->next=NULL;
                    q->rear=newN;
                    tempCurrent=newN->next;
                }
            }
        }


    }

}

void printRemaining(queue* q)
{
      node* tempCurrent=(node*)malloc(sizeof(node));


        tempCurrent=q->front;

        while (tempCurrent!=NULL)
        {
            printf("\nmy runtime is %d  \n" ,tempCurrent->data.remainingtime);
            tempCurrent=tempCurrent->next;
        }  

}
//////////////////////////// MOSTAFA /////////////////////////////////////
void enqueue_roundrobin(queue *q)
{
    if(q->front == q->rear)
    {
        //do nothing
    }
    else
    {
        node *temp = q->front;
        q->front = q->front->next;
        temp->next = NULL;

        q->rear->next = temp;
        q->rear = temp;
    }
}
//////////////////////////// MOSTAFA /////////////////////////////////////
