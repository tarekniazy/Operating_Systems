#include"headers.h"



struct root
{
	int size;
    Process data;
	struct root *rightChild;
	struct root *leftChild;
    bool isBusy;
};


typedef struct root root;

root* rootinitialize()
{
    root*r=(root*)malloc(sizeof(root));

    r->size=1024;
    r->isBusy=false;
    r->rightChild=NULL;
    r->leftChild=NULL;

}

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

Process peekQueue(queue*q)
{
  static Process temp;
    if (q->front!=NULL)
    {
       temp=q->front->data;
       return temp;

    }
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
            if (((newN->data.priority) < (tempCurrent->data.priority)) && tempCurrent->data.remainingtime>0)
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



void enqueueRunning(Process *newP,queue* q)
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
            if (((newN->data.runningtime) < (tempCurrent->data.runningtime)) )
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
            printf("\nmy Id is %d  \n" ,tempCurrent->data.id);
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




void enqueueArriving(Process newP,queue* q)
{
    if (q->front==NULL||q->rear==NULL)
    {
        node* newN=(node*)malloc(sizeof(node));
        newN->data=newP;
        newN->next=NULL;
        q->front=newN;
        q->rear=newN;   
        q->count++;   
    }
    else 
    {
         printf("\n  The que is not empty \n") ;
        node* tempCurrent=(node*)malloc(sizeof(node));
        node* newN=(node*)malloc(sizeof(node));
        newN->data=newP;

        node* previuos=(node*)malloc(sizeof(node));

        tempCurrent=q->front;
        previuos=tempCurrent;

        while (tempCurrent!=NULL)
        {
            if ((newN->data.arrivaltime) < (tempCurrent->data.arrivaltime))
            {
            printf("\n  I have the oldest arrive time :) \n") ;
              q->count++;
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
                printf("\n  There is another Process that has oldest arrive time \n") ;

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
                    q->count++;
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


bool reserveFirstFit(root*r,Process p)
{
    root* temp=r;
  
  

  while (temp != NULL )
  {
      printf("\nThe size of the process %d\n",p.memsize);

    if (!temp->isBusy)
    {
        if (temp->size > p.memsize)
        {

           

            temp->data=p; 

            temp->size-=p.memsize;

            if (temp->size>0)
            {
                            

                root*nextRoot=rootinitialize();
                nextRoot->size=temp->size;
                nextRoot->data=temp->data;
                nextRoot->rightChild=temp->rightChild;
                temp->rightChild=nextRoot;
                
            } 
            temp->size=p.memsize;
             temp->isBusy=true;
            temp->data=p;
            return true;
        }
        temp=temp->rightChild;
    }
    else if (temp->size<1024) 
    {
          temp=temp->rightChild;

    }
  }

  return false;

}

void freeFirstFit(root*r,Process p)
{
       root* temp=r;
  
  while (temp != NULL )
  {
    if (temp->isBusy)
    {
     if (temp->data.id==p.id)
      {
        

          temp->isBusy=false;
          printf("\nI am process %d and I am leaving the memoey now..\n ",temp->data.id);

          return;
          
      }
      else 
      {
         temp=temp->rightChild;
      }
    }
    else 
    {
        temp=temp->rightChild;
    }

  }
}

void printRoot(root*r)
{
      root* temp=r;
  
  while (temp != NULL )
  {

    if (temp->isBusy)
    {
        printf("\n I am the root %d and my reserved size = %d\n",temp->data.id,temp->size);
        temp=temp->rightChild;
    }
    else 
    {
          printf("\n I am the root and I have free size of  = %d\n",temp->size);
          temp=temp->rightChild;
    }

   
  }


}

void rebaseFirstFit(root*r)
{
      root* temp=r;
      root*last=NULL;
  
  while (temp != NULL )
  {

    if (!temp->isBusy)
    {
        if (last!=NULL)
        {
            last->size+=temp->size;
            last->rightChild=temp->rightChild;
           temp=last;
           
        }
        last=temp;
        temp=temp->rightChild;
    }
    else{
last=NULL;
 temp=temp->rightChild;
    }
   
  }
}


bool reserveNextFit(root*r,Process p)
{
    root* temp=r;
  
  while (temp != NULL )
  {

    if (!temp->isBusy&&temp->rightChild==NULL)
    {
        if (temp->size >= p.memsize)
        {
            temp->isBusy=true;

            temp->data=p; 
            

            temp->size-=p.memsize;

            if (temp->size>0)
            {
                root*nextRoot=rootinitialize();
                nextRoot->size=temp->size;
                nextRoot->rightChild=temp->rightChild;
                temp->rightChild=nextRoot;
      
                
            } 
            temp->size=p.memsize;
            return true;
        }
        temp=temp->rightChild;
    }
    else if (temp->size<1024) 
    {
          temp=temp->rightChild;

    }
  }


  root* temp2=r;
  
  
  while (temp2 != NULL )
  {

    if (!temp2->isBusy)
    {
        if (temp2->size >= p.memsize)
        {
            temp2->isBusy=true;

            temp2->data=p; 
            

            temp2->size-=p.memsize;

            if (temp2->size>0)
            {
                root*nextRoot=rootinitialize();
                nextRoot->size=temp2->size;
                nextRoot->rightChild=temp2->rightChild;
                temp2->rightChild=nextRoot;
      
                
            } 
            temp2->size=p.memsize;
            return true;
        }
        temp2=temp2->rightChild;
    }
    else if (temp2->size<1024) 
    {
          temp2=temp2->rightChild;

    }
  }
  return false;

}

///////////////Best Fit//////////////////////
bool reserveBestFit(root*r,Process p)
{
    root* temp=r;
    root* temp2;
    int MinSize=1025;
  while (temp != NULL )
  {
    if (!temp->isBusy)
    { 
        if (temp->size >= p.memsize){
            if(temp->size<MinSize)
            {
                MinSize=temp->size;
                temp2=temp;
            
            }

        }
    }

    temp=temp->rightChild;
  }
  if(temp2!=NULL){
        temp2->isBusy=true;

        temp2->data=p; 
        

        temp2->size-=p.memsize;

        if (temp2->size>0)
        {
            root*nextRoot=rootinitialize();
            nextRoot->size=temp2->size;
            nextRoot->rightChild=temp2->rightChild;
            temp2->rightChild=nextRoot;
        } 
        temp2->size=p.memsize;
        return true;
  }
  return false;

}



bool Allocate(int policyNumber,root*r,Process p)
{
    if (policyNumber==1)
    {
        return reserveFirstFit(r,p);


    }
    else if (policyNumber==2)
    {
        return reserveNextFit(r,p);
    }
     else if (policyNumber==3)
    {
        return reserveBestFit(r,p);
    }
     else if (policyNumber==4)
    {
        //TODO return (Budy System Allocation function)
    }
}


void deAllocate(int policyNumber,root*r,Process p)
{
    if (policyNumber==1)
    {
    freeFirstFit(r,p);
    rebaseFirstFit(r);
    }
    else if (policyNumber==2)
    {
        freeFirstFit(r,p);
        rebaseFirstFit(r);
    }
     else if (policyNumber==3)
    {
        freeFirstFit(r,p);
        rebaseFirstFit(r);
    }
     else if (policyNumber==4)
    {
        //TODO return (Budy System Allocation function)
    }
}


void getMemoryLocation(int *x,int *y,root*r,Process p)
{ 

  root* temp=r;
  *x=0;
  while (temp != NULL )
  {
    if (temp->isBusy)
    {
     if (temp->data.id==p.id)
      {
        

          *y=*x+p.memsize;
          printf("\nI am process %d and I my locations are %d and %d now..\n ",p.id,*x,*y);

          return;
          
      }
      else 
      {
         temp=temp->rightChild;
          *x+=temp->size;
         
      }
    }
    else 
    {
        temp=temp->rightChild;
        *x+=temp->size;

    }

  }

}


