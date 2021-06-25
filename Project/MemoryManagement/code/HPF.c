// #include "headers.h"
#include "Queue.h"

  
// struct Process {
//     int ProcessID;
//   int remainingTime;
//     int arrivalTime;
//     int priority;
//     int outtime;
//     int intime;
// };
// typedef struct Process Process;

// struct node
// {
// 	Process data;
// 	struct node *next;
// };
// typedef struct node node;


// struct queue{
//     node* front;
//     node* rear;
//     int count;


// };
// typedef struct queue queue;


// queue* initialize()
// {
// 	queue*q=(queue*)malloc(sizeof(queue));
// 	q->count=0;
// 	q->front=NULL;
// 	q->rear=NULL;
// }

// void enqueuePriority(Process *newP,queue* q)
// {
//     if (q->front==NULL||q->rear==NULL)
//     {
//         node* newN=(node*)malloc(sizeof(node));
//         newN->data=*newP;
//         newN->next=NULL;
//         q->front=newN;
//         q->rear=newN;      
//     }
//     else 
//     {
//          printf("\n  The que is not empty \n") ;
//         node* tempCurrent=(node*)malloc(sizeof(node));
//         node* newN=(node*)malloc(sizeof(node));
//         newN->data=*newP;

//         node* previuos=(node*)malloc(sizeof(node));

//         tempCurrent=q->front;
//         previuos=tempCurrent;

//         while (tempCurrent!=NULL)
//         {
//             if ((newN->data.priority) < (tempCurrent->data.priority))
//             {
//             printf("\n  I have the Highest Priority :) \n") ;
              
//               newN->next=tempCurrent;
              
//               if (tempCurrent==q->front)
//               {
//                   q->front=newN;
//               }
//               else 
//               {
//                 previuos->next=newN;
//               }
//              return;
//             }
//             else 
//             {
//                 printf("\n  There is another Process that has higher priority \n") ;

//                 if (tempCurrent->next!=NULL)
//                 {
//                     previuos=tempCurrent;
//                 tempCurrent=tempCurrent->next;
//                 }
//                 else 
//                 {  printf("\n  I am the last one :( \n") ;

//                     tempCurrent->next=newN;
//                     newN->next=NULL;
//                     q->rear=newN;
//                     tempCurrent=newN->next;
//                 }
//             }
//         }


//     }

// }

// void enqueueRemaining(Process *newP,queue* q)
// {
//     if (q->front==NULL||q->rear==NULL)
//     {
//         node* newN=(node*)malloc(sizeof(node));
//         newN->data=*newP;
//         newN->next=NULL;
//         q->front=newN;
//         q->rear=newN;      
//     }
//     else 
//     {
//          printf("\n  The que is not empty \n") ;
//         node* tempCurrent=(node*)malloc(sizeof(node));
//         node* newN=(node*)malloc(sizeof(node));
//         newN->data=*newP;

//         node* previuos=(node*)malloc(sizeof(node));

//         tempCurrent=q->front;
//         previuos=tempCurrent;

//         while (tempCurrent!=NULL)
//         {
//             if ((newN->data.remainingtime) < (tempCurrent->data.remainingtime))
//             {
//             printf("\n  I have the Shortest Runtime :) \n") ;
              
//               newN->next=tempCurrent;
              
//               if (tempCurrent==q->front)
//               {
//                   q->front=newN;
//               }
//               else 
//               {
//                 previuos->next=newN;
//               }
//              return;
//             }
//             else 
//             {
//                 printf("\n  There is another Process that has shortest RunTime \n") ;

//                 if (tempCurrent->next!=NULL)
//                 {
//                     previuos=tempCurrent;
//                 tempCurrent=tempCurrent->next;
//                 }
//                 else 
//                 {  printf("\n  I am the last one :( \n") ;

//                     tempCurrent->next=newN;
//                     newN->next=NULL;
//                     q->rear=newN;
//                     tempCurrent=newN->next;
//                 }
//             }
//         }


//     }

// }


// void printPriority(queue* q)
// {
//       node* tempCurrent=(node*)malloc(sizeof(node));


//         tempCurrent=q->front;

//         while (tempCurrent!=NULL)
//         {
//             printf("\nmy priority is %d  \n" ,tempCurrent->data.priority);
//             tempCurrent=tempCurrent->next;
//         }  

// }


// void printRemaining(queue* q)
// {
//       node* tempCurrent=(node*)malloc(sizeof(node));


//         tempCurrent=q->front;

//         while (tempCurrent!=NULL)
//         {
//             printf("\nmy runtime is %d  \n" ,tempCurrent->data.remainingtime);
//             tempCurrent=tempCurrent->next;
//         }  

// }





int main()
{
   
  //  queue *q=initialize();
   Process *p1=(Process*)malloc(sizeof(Process));
   p1->id=0;
   p1->memsize=200;
   Process *p2=(Process*)malloc(sizeof(Process));
   p2->id=2;
   p2->memsize=170;
   Process *p3=(Process*)malloc(sizeof(Process));
   p3->id=1;
   p3->memsize=200;
    Process *p4=(Process*)malloc(sizeof(Process));
      p4->id=4;
   p4->memsize=100;
   Process *p5=(Process*)malloc(sizeof(Process));
   p5->remainingtime=3;
  //  enqueueRemaining(p1,q);
  //  printRemaining(q);
  //  enqueueRemaining(p2,q);
  //  printRemaining(q);
  //  enqueueRemaining(p3,q);
  //  printRemaining(q);
  //  enqueueRemaining(p4,q);
  //  printRemaining(q);
  //  enqueueRemaining(p5,q);
  //  printRemaining(q);

  root *r=rootinitialize();

if(Allocate(1,r,*p1))
{
  // printRoot(r);
}
else 
{
  printf("\n Failed in insertion\n");
}

if(Allocate(1,r,*p2))
{
  Allocate(1,r,*p3);
}
else 
{
  printf("\n Failed in insertion\n");
}
  printRoot(r);

  printf("\n DeAllocation\n");

  deAllocate(1,r,*p1);

  
  printRoot(r);

      printf("\n Allocation...\n");

  Allocate(1,r,*p4);

    printRoot(r);



      printf("\n DeAllocation\n");

      deAllocate(1,r,*p2);



          printRoot(r);




  return 0;
}
