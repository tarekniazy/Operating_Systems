
#include"Queue.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
//////////////////////////FCFS/////////////////
node*Head;
queue*outqueue;
int numberofprocesses;
int recv_val;
Process running;

queue *waitingList; 

root *r;

bool waitinglistempty=true;

int Pid;


Process nextProcess;

int lastProcessEntered=-1;


Process* currentRun;

int currentTimeStep=0;

FILE*output;
FILE*memory;
int stat_loc;
void childrenTerminating(int);
bool available=1;


void terminate(int signal)
{
    if( waitpid(-1,&stat_loc,WNOHANG) > 0 ) 
    {
        if (WIFEXITED(stat_loc))
        {
            currentRun->finishtime = WEXITSTATUS(stat_loc);
            currentRun->turnaround_time = currentRun->finishtime - currentRun->arrivaltime;
            currentRun->waitingtime = currentRun->finishtime-currentRun->runningtime - currentRun->arrivaltime;
            currentRun->weighted_turnaround_time = (float)currentRun->turnaround_time / (float)currentRun->runningtime;
            currentRun->remainingtime = 0;                
            fprintf(output,"At time %d process %d finished arr %d  remain %d wait %d ",currentRun->finishtime,currentRun->id,currentRun->arrivaltime,currentRun->remainingtime,currentRun->waitingtime);    
             //fprintf(memory,"#At time %d freed %d bytes for process %d from i to j\n",currentRun->finishtime,currentRun->memsize,currentRun->id);
            fprintf(output,"TA %d WTA %0.2f \n",currentRun->turnaround_time,currentRun->weighted_turnaround_time);
            enqueue(outqueue,*currentRun);
            numberofprocesses--;
            currentRun->state=-1;
            available =1; 
            printf("quiting in MEMORY");
           
        }

    }    
}








queue* FCFS(int procs,int policynumber)
{		
	
    r=rootinitialize();
    waitingList=initialize();

	queue*temp;
    initClk();
     numberofprocesses=procs;
    
     signal(SIGCHLD, childrenTerminating);
    
    int msgq_id = msgget(500, 0666 | IPC_CREAT);
     if (msgq_id == -1)
    {
        perror("Error in create");
        exit(-1);
    }
    
    struct msgbuff message;
    int send_val;
    
    //Head=(node*)malloc(sizeof(node));
    //Head=NULL;
   outqueue=initialize();
   temp=initialize();
     //printf("creating file..");
    int last=-1;
    output = fopen("scheduler.log","w");
    memory=fopen("memory.log","w");
    //printf("writing..");
    fprintf(output,"#At time x process y state arr w total z remain y wait k\n");
    fprintf(memory,"#At time x allocated y bytes for process z from i to j\n");

    while(numberofprocesses>0)
    {	





        if (numberofprocesses>waitingList->count)
        {

    	recv_val = msgrcv(msgq_id, &message, sizeof(message.message), 0, !IPC_NOWAIT);
        
    		 	
        
    		 if (recv_val != -1) 
             {//not received
            		//perror("Error in receive");
            
            	
            		if(last!=message.message.id)
            		{
                        message.message.waitingtime=0;
                        message.message.state=2;
            			last=message.message.id;
            			message.message.remainingtime=message.message.runningtime;
                        enqueueArriving((message.message),waitingList);
                     printf("\n Time now is %d \n",getClk());
                     printf("\n I process %d arrived at time %d \n",message.message.id,getClk());



              
                    }
             }
        }


         if (!isempty(waitingList)) 
                      {
        
                          
                            nextProcess=peekQueue(waitingList);
                            dequeue(waitingList);
                            
                            printf("\n De queuing from waiting list process that has id %d\n",nextProcess.id);
                       
                      }   



    		  if (lastProcessEntered!=nextProcess.id)
                       {

                        if (Allocate(policynumber,r,nextProcess))
                        {
                            lastProcessEntered=nextProcess.id;

                            printRoot(r);
                            printf("\nMemory\n");
                            fprintf(memory,"#At time %d allocated %d bytes for process %d from i to j\n",getClk(),nextProcess.memsize,nextProcess.id);
                           		printf("running time: %d\n",nextProcess.runningtime);
            			    enqueue(temp,message.message);
                        
                        }
                        else 
                        {
                              printf("\n Failed in insertion\n");

                            enqueueArriving(nextProcess,waitingList);
                           
                          
                         }
                       }
		
		
		
		
    
    if (available == 1)
    {
    if(!isempty(temp))
    {
    if(temp->front->data.arrivaltime<=getClk())
	{
    	running=temp->front->data;
    	running.starttime=getClk();
    	printf("process: %d starttime:%d\n",running.id,running.starttime);
    	int Pid=fork();
    	writeShM(running.runningtime+getClk());
    	if (Pid == 0)
                {
                	printf("I'm forking");
                    char rem[4];
                    sprintf(rem,"%d",temp->front->data.runningtime);
                    char* const p[] = {"./process.o",rem,NULL};
                    execvp(p[0],p);

                }
    	running.waitingtime=running.starttime-running.arrivaltime;
    	fprintf(output,"At time %d process %d %s arr %d total %d remain %d wait %d\n",getClk(),running.id,"started",running.arrivaltime,running.runningtime,running.runningtime,running.waitingtime);
            deAllocate(policynumber,r,temp->front->data);
             printRoot(r);

             dequeue(temp);

              if (!isempty(waitingList)) 
                {

                    nextProcess=peekQueue(waitingList);
                    dequeue(waitingList);
                    
                    printf("\n De queuing from waiting list process that has id %d\n",nextProcess.id);


                    if (Allocate(policynumber,r,nextProcess))
                    {
                            lastProcessEntered=nextProcess.id;

                            printRoot(r);
                            printf("\nMemory\n");
                            fprintf(memory,"#At time %d allocated %d bytes for process %d from i to j\n",getClk(),nextProcess.memsize,nextProcess.id);
                           		printf("running time: %d\n",nextProcess.runningtime);
            			    enqueue(temp,message.message);
                    }
                    else 
                    {
                    printf("\n Failed in insertion\n");

                            enqueueArriving(nextProcess,waitingList);
                    }

                
                }

            printf("\n DeAloocate\n");
              printf("\n Waiting list\n");

                        printPriority(waitingList);
                     printf("\n ----\n");
    	available =0;
    }
    }
    }
    }
    return outqueue;
}






void childrenTerminating(int signal)
{
    if( waitpid(-1,&stat_loc,WNOHANG) > 0 ) 
    {
        if (WIFEXITED(stat_loc))
        {
            running.finishtime = WEXITSTATUS(stat_loc);
            running.turnaround_time = running.finishtime - running.arrivaltime;
            running.waitingtime = running.finishtime-running.runningtime - running.arrivaltime;
            running.weighted_turnaround_time = (float)running.turnaround_time / (float)running.runningtime;
            running.remainingtime = 0;   
                           
            fprintf(output,"At time %d process %d finished arr %d  remain %d wait %d ",running.finishtime,running.id,running.arrivaltime,running.remainingtime,running.waitingtime); 
           fprintf(memory,"#At time %d freed %d bytes for process %d from i to j\n",running.finishtime,running.memsize,running.id);  
            fprintf(output,"TA %d WTA %0.2f \n",running.turnaround_time,running.weighted_turnaround_time);
            enqueue(outqueue,running);
            numberofprocesses--;
            running.state=-1;
            available =1; 
            
        }

    }    
}




queue* SJF(int procs,int policyNumber)
{		
	
	r=rootinitialize();
    waitingList=initialize();
    initClk();
    numberofprocesses = procs;
    queue*temp;

    temp=initialize();

    signal(SIGCHLD, childrenTerminating);
    
    int msgq_id = msgget(500, 0666 | IPC_CREAT);
     if (msgq_id == -1)
    {
        perror("Error in create");
        exit(-1);
    }
    
    struct msgbuff message;
    int send_val;
    
    Head = (node*)malloc(sizeof(node));  
    Head = NULL;
    
    outqueue = initialize();

    
    int last=-1;
    output = fopen("scheduler.log","w");
    memory=fopen("memory.log","w");
    //printf("writing..");
    fprintf(output,"#At time x process y state arr w total z remain y wait k\n");
    fprintf(memory,"#At time x allocated y bytes for process z from i to j\n");

    while ( numberofprocesses > 0)
    {
      
   if (numberofprocesses>waitingList->count)
        {

    	recv_val = msgrcv(msgq_id, &message, sizeof(message.message), 0, !IPC_NOWAIT);
        
    		 	
        
    		 if (recv_val != -1) 
             {//not received
            		//perror("Error in receive");
            
            	
            		if(last!=message.message.id)
            		{
                        message.message.waitingtime=0;
                        message.message.state=2;
            			last=message.message.id;
            			message.message.remainingtime=message.message.runningtime;
                        enqueueArriving((message.message),waitingList);
                     printf("\n Time now is %d \n",getClk());
                     printf("\n I process %d arrived at time %d \n",message.message.id,getClk());



              
                    }
             }
        }

                     if (!isempty(waitingList)) 
                      {
        
                          
                            nextProcess=peekQueue(waitingList);
                            dequeue(waitingList);
                            
                            printf("\n De queuing from waiting list process that has id %d\n",nextProcess.id);
                       
                      }       

                        if (lastProcessEntered!=nextProcess.id)
                       {

                        if (Allocate(policyNumber,r,nextProcess))
                        {
                            lastProcessEntered=nextProcess.id;

                            printRoot(r);
                            printf("\nMemory\n");
                            fprintf(memory,"#At time %d allocated %d bytes for process %d from i to j\n",getClk(),nextProcess.memsize,nextProcess.id);
                            enqueueRunning(&nextProcess,temp);
                              printPriority(temp);
                        
                        }
                        else 
                        {
                              printf("\n Failed in insertion\n");

                            enqueueArriving(nextProcess,waitingList);
                           
                          
                         }
                     }

		
    
    if (available == 1)
    {
    if(!isempty(temp))
    {
    if(temp->front->data.arrivaltime<=getClk())
	{
	
    	running=temp->front->data;
    	running.starttime=getClk();
    	int Pid=fork();
        writeShM(running.remainingtime+getClk());

    	if (Pid == 0)
                {
                    
                    char rem[4];
                    sprintf(rem,"%d",temp->front->data.runningtime);
                    char* const p[] = {"./process.o",rem,NULL};
                    execvp(p[0],p);

                }
    	running.waitingtime=running.starttime-running.arrivaltime;
    	fprintf(output,"At time %d process %d %s arr %d total %d remain %d wait %d\n",getClk(),running.id,"started",running.arrivaltime,running.runningtime,running.runningtime,running.waitingtime);
    	 
             deAllocate(policyNumber,r,temp->front->data);
             printRoot(r);

             dequeue(temp);

              if (!isempty(waitingList)) 
                {

                    nextProcess=peekQueue(waitingList);
                    dequeue(waitingList);
                    
                    printf("\n De queuing from waiting list process that has id %d\n",nextProcess.id);


                    if (Allocate(policyNumber,r,nextProcess))
                    {
                            lastProcessEntered=nextProcess.id;
                                printRoot(r);
                            printf("\nMemory\n");
                            fprintf(memory,"#At time %d allocated %d bytes for process %d from i to j\n",getClk(),nextProcess.memsize,nextProcess.id);
                          enqueueRunning(&nextProcess,temp);    
                              printPriority(temp);
                    }
                    else 
                    {
                    enqueueArriving(nextProcess,waitingList);
                    }

                
                }

            printf("\n DeAloocate\n");
              printf("\n Waiting list\n");

                        printPriority(waitingList);
                     printf("\n ----\n");
    	available =0;
    	
    }
    }
    }
    }
    
    return outqueue;
}


queue* HPF(int procs,int policyNumber)
{		
	int i,j;
	printf("HPF I am here ......");
    initClk();

    r=rootinitialize();
    waitingList=initialize();
    // int Pid;
     numberofprocesses=procs;
 
     signal(SIGCHLD, terminate);
    
    int msgq_id = msgget(500, 0666 | IPC_CREAT);
     if (msgq_id == -1)
    {
        perror("Error in create");
        exit(-1);
    }
    
    struct msgbuff message;
    int send_val;
    
    //Head=(node*)malloc(sizeof(node));
    //Head=NULL;
   
   outqueue=initialize();
   
   queue *temp=initialize();

    int last=-1;
    int lastPid=-1;
     //printf("creating file..");
    //printf("entering fcfs..");
	output = fopen("scheduler.log","w");
	memory=fopen("memory.log","w");
    //printf("writing..");
    fprintf(output,"#At time x process y state arr w total z remain y wait k\n");
    fprintf(memory,"#At time x allocated y bytes for process z from i to j\n");
     //printf("written..");
  
    
    while(numberofprocesses>0)
    {	
        // printf("\n Entered  %d,  %d\n",numberofprocesses ,);

        // if (numberofprocesses>)

        if (numberofprocesses>waitingList->count)
        {

    	recv_val = msgrcv(msgq_id, &message, sizeof(message.message), 0, !IPC_NOWAIT);
        
    		 	
        
    		 if (recv_val != -1) 
             {
            
            	
            		if(last!=message.message.id)
            		{
                        message.message.waitingtime=0;
                        message.message.state=2;
            			last=message.message.id;
            			message.message.remainingtime=message.message.runningtime;
                        enqueueArriving((message.message),waitingList);
                     printf("\n Time now is %d \n",getClk());
                     printf("\n I process %d arrived at time %d \n",message.message.id,getClk());



              
                    }
             }
        }
                       
                        printf("\n The waiting List is not empty  \n");


                      if (!isempty(waitingList)) 
                      {
                           

                          
                            nextProcess=peekQueue(waitingList);
                            dequeue(waitingList);
                            
                            printf("\n De queuing from waiting list process that has id %d\n",nextProcess.id);
                       
                      }
                    


                         
                       

                       if (lastProcessEntered!=nextProcess.id)
                       {

                        if (Allocate(policyNumber,r,nextProcess))
                        {
                            lastProcessEntered=nextProcess.id;

                            printRoot(r);
                            printf("\nMemory\n");
                        if (!isempty(temp))
                        {
                               


                            if ((nextProcess.priority)<(temp->front->data.priority)&& (temp->front->data.remainingtime>0))
                            {

                         	kill(temp->front->data.pid,SIGTSTP);

                                printf("\nI am process with id %d  and my remaining time is %d,I have been stopped by id.. %d\n " ,temp->front->data.id,temp->front->data.remainingtime,nextProcess.id);
                                printf("\nTime now is.. %d\n " ,getClk());
                                
                             temp->front->data.remainingtime-=(getClk()-temp->front->data.resumed);

                             fprintf(output,"At time %d process %d %s arr %d total %d remain %d wait %d\n",getClk(),temp->front->data.id,"stopped",temp->front->data.arrivaltime,temp->front->data.runningtime,temp->front->data.remainingtime,temp->front->data.waitingtime);
  

                                
                                printf(" \nMy Id ...%d\n",nextProcess.id);


                                temp->front->data.state=1;  //Stopped
                                temp->front->data.stoppingTime=getClk();
                              
                            }

                           
                           getMemoryLocation(&i,&j,r,nextProcess);
                          fprintf(memory,"#At time %d allocated %d bytes for process %d from %d to %d\n",getClk(),nextProcess.memsize,nextProcess.id,i,j);
                        enqueuePriority(&(nextProcess),temp);
                        printPriority(temp);
                        }
                        else
                        {
                         printf("\n whyyyyyyyyyyyyyyyyyy\n");
			            fprintf(memory,"#At time %d allocated %d bytes for process %d from %d to %d\n",getClk(),nextProcess.memsize,nextProcess.id,i,j);
                        enqueuePriority(&(nextProcess),temp);
                        printPriority(temp);
                        }
                        }
                        else 
                        {
                              printf("\n Failed in insertion\n");

                            enqueueArriving(nextProcess,waitingList);
                         }
                     }
                      
                     
                   
                              printf("\n Waiting list\n");

                        printPriority(waitingList);
                     printf("\n ----\n");
            		

                     printRoot(r);
                    printf("\nMemory\n");
            
		
      

            if (temp->front->data.state==-1)
        {
            
            printf("\nI am process with id %d quiting now :( %d  and time is %d  and remaining time is %d \n " ,temp->front->data.id,temp->front->data.priority,getClk(),temp->front->data.remainingtime);
             
            getMemoryLocation(&i,&j,r,nextProcess);
            fprintf(memory,"#At time %d freed %d bytes for process %d from %d to %d\n",currentRun->finishtime,currentRun->memsize,currentRun->id,i,j);
            deAllocate(policyNumber,r,(temp->front->data));
            printRoot(r);
     
            dequeue(temp);

              if (!isempty(waitingList)) 
                {
                    

                    
                    nextProcess=peekQueue(waitingList);
                    dequeue(waitingList);
                    
                    printf("\n De queuing from waiting list process that has id %d\n",nextProcess.id);


                if (Allocate(policyNumber,r,nextProcess))
                {
                     lastProcessEntered=nextProcess.id;
                       getMemoryLocation(&i,&j,r,nextProcess);
                          fprintf(memory,"#At time %d allocated %d bytes for process %d from %d to %d\n",getClk(),nextProcess.memsize,nextProcess.id,i,j);
                      
                    enqueuePriority(&(nextProcess),temp);
                    printPriority(temp);
                }
                else 
                {
                enqueueArriving(nextProcess,waitingList);
                }

                
                }

           

            printf("\n DeAloocate\n");
              printf("\n Waiting list\n");

                        printPriority(waitingList);
                     printf("\n ----\n");
           

        }

		
    
   
        if(!isempty(temp))
        {
        if(temp->front->data.arrivaltime<=getClk())
        {

             currentRun=&(temp->front->data);
             currentRun->starttime=getClk();
           
            if(temp->front->data.state==1)
            {
                
               currentRun->resumed=getClk();
               

                
                temp->front->data.waitingtime+=getClk()-temp->front->data.stoppingTime;

                writeShM(temp->front->data.remainingtime+getClk());

              

                 kill(temp->front->data.pid,SIGCONT);
                  currentRun->state=0;
        fprintf(output,"At time %d process %d %s arr %d total %d remain %d wait %d\n",getClk(),currentRun->id,"resumed",temp->front->data.arrivaltime,temp->front->data.runningtime,temp->front->data.remainingtime,temp->front->data.waitingtime);


                printf("\nI am process %d,I am continueing my Job. and time is..%d. and my remaining time =.. %d\n " ,temp->front->data.priority,getClk() ,temp->front->data.remainingtime);
              

            }
            else if (temp->front->data.state!=0)
            {

             

            Pid=fork();



            currentRun->pid=Pid;
            currentRun->state=0;
             currentRun->resumed=getClk();

            
            currentRun->stoppingTime=0;
            writeShM(currentRun->remainingtime+getClk());
            
            temp->front->data.pid=Pid;
            temp->front->data.state=0;
               
            temp->front->data.pid=Pid;
            temp->front->data.state=0;
            temp->front->data.resumed=getClk();



                
           

            
        fprintf(output,"At time %d process %d %s arr %d total %d remain %d wait %d\n",getClk(),currentRun->id,"started",temp->front->data.arrivaltime,temp->front->data.runningtime,temp->front->data.remainingtime,temp->front->data.waitingtime);

            
            if (Pid == 0)
                    {
                       
                       
                    


                        printf("\nForking .. %d\n " ,temp->front->data.pid);

                        // printf("\nI am process with priority .. %d I started  running now and the time is ...%d and remaining time .. %d \n " ,temp->front->data.priority,getClk(),temp->front->data.remainingtime);
                        char rem[4];
                        sprintf(rem,"%d",temp->front->data.remainingtime);
                    
                        char* const p[] = {"./process.o",rem,NULL};
                        execvp(p[0],p);
                       
                     
                    }
                   
            }

                    if (currentRun->state==0)
                    {
                        // if (getClk()-currentTimeStep==1)
                        // {
                        //     currentRun->remainingtime--;
                        // }
                        
                        


                        printf("\nI am process with priority .. %d I am running now and the time is ...%d and remaining time .. %d \n " ,temp->front->data.priority,getClk(),temp->front->data.remainingtime);

                    }
    
        }
        }   
    }        
    return outqueue;
}



queue* SRTN(int procs,int policyNumber)
{		
	
	printf("SRTN I am here ......");
    initClk();
    // int Pid;
     numberofprocesses=procs;


    r=rootinitialize();
    waitingList=initialize();
 
     signal(SIGCHLD, terminate);
    
    int msgq_id = msgget(500, 0666 | IPC_CREAT);
     if (msgq_id == -1)
    {
        perror("Error in create");
        exit(-1);
    }
    
    struct msgbuff message;
    int send_val;
    
    //Head=(node*)malloc(sizeof(node));
    //Head=NULL;
   
   outqueue=initialize();
   
   queue *temp=initialize();

    int last=-1;
     //printf("creating file..");
    //printf("entering fcfs..");
	output = fopen("scheduler.log","w");
	memory=fopen("memory.log","w");
    //printf("writing..");
    fprintf(output,"#At time x process y state arr w total z remain y wait k\n");
    fprintf(memory,"#At time x allocated y bytes for process z from i to j\n");
     //printf("written..");
  
    
    while(numberofprocesses>0)
    {	currentTimeStep=getClk();
    	        if (numberofprocesses>waitingList->count)
        {

    	recv_val = msgrcv(msgq_id, &message, sizeof(message.message), 0, !IPC_NOWAIT);
        
    		 	
        
    		 if (recv_val != -1) 
             {//not received
            		//perror("Error in receive");
            
            	
            		if(last!=message.message.id)
            		{
                        message.message.waitingtime=0;
                        message.message.state=2;
            			last=message.message.id;
            			message.message.remainingtime=message.message.runningtime;
                        enqueueArriving((message.message),waitingList);
                     printf("\n Time now is %d \n",getClk());
                     printf("\n I process %d arrived at time %d \n",message.message.id,getClk());



              
                    }
             }
        }
                       
                        printf("\n The waiting List is not empty  \n");


                      if (!isempty(waitingList)) 
                      {
                           

                          
                            nextProcess=peekQueue(waitingList);
                            dequeue(waitingList);
                            
                            printf("\n De queuing from waiting list process that has id %d\n",nextProcess.id);
                       
                      }
    		
    	
            	
            	
            	    if (lastProcessEntered!=nextProcess.id)
                       {

                        if (Allocate(policyNumber,r,nextProcess))
                        {
                            lastProcessEntered=nextProcess.id;

                            printRoot(r);
                            printf("\nMemory\n");
           
                        if (!isempty(temp))
                        {
                        printf("\nnew process entered with remaining %d   compared with the front with reamining%d\n " ,message.message.remainingtime,temp->front->data.remainingtime);

                               temp->front->data.remainingtime=temp->front->data.runningtime-(getClk()-temp->front->data.resumed);
                            if ((message.message.remainingtime)<(temp->front->data.remainingtime))
                            {

                                printf("\nI am process with remaining %d,I have been stopped by remaining.. %d\n " ,temp->front->data.remainingtime,message.message.remainingtime);
                                printf("\nTime now is.. %d\n " ,getClk());
                                fprintf(output,"At time %d process %d %s arr %d total %d remain %d wait %d\n",getClk(),currentRun->id,"stopped",temp->front->data.arrivaltime,temp->front->data.runningtime,temp->front->data.remainingtime,temp->front->data.waitingtime);

                                kill(temp->front->data.pid,SIGTSTP);

                                temp->front->data.state=1;  //Stopped
                                temp->front->data.stoppingTime=getClk();
                            }

                        }
                     
                        fprintf(memory,"#At time %d allocated %d bytes for process %d from i to j\n",getClk(),nextProcess.memsize,nextProcess.id);
                        enqueueRemaining(&(message.message),temp);
                        printRemaining(temp);
                          }
                        else 
                        {
                              printf("\n Failed in insertion\n");

                            enqueueArriving(nextProcess,waitingList);
                         }
                     }
            	
            
		
        if (temp->front->data.state==-1)
        {

            printf("\nI am quiting now :( %d  and time is %d  and remaining time is %d \n " ,temp->front->data.priority,getClk(),temp->front->data.remainingtime);
            
            deAllocate(policyNumber,r,(temp->front->data));
             printRoot(r);
fprintf(memory,"#At time %d freed %d bytes for process %d from i to j\n",currentRun->finishtime,currentRun->memsize,currentRun->id);
            dequeue(temp);

              if (!isempty(waitingList)) 
                {
                    

                    
                    nextProcess=peekQueue(waitingList);
                    dequeue(waitingList);
                    
                    printf("\n De queuing from waiting list process that has id %d\n",nextProcess.id);


                         if (Allocate(policyNumber,r,nextProcess))
                {
                     lastProcessEntered=nextProcess.id;
                     fprintf(memory,"#At time %d allocated %d bytes for process %d from i to j\n",getClk(),r->size,nextProcess.id);
                    enqueuePriority(&(nextProcess),temp);
                    printPriority(temp);
                }
                else 
                {
                enqueueArriving(nextProcess,waitingList);
                }

                
                }

           

            printf("\n DeAloocate\n");
              printf("\n Waiting list\n");

                        printPriority(waitingList);
                     printf("\n ----\n");
           
        }
		
		
		
    
   
        if(!isempty(temp))
        {
        if(temp->front->data.arrivaltime<=getClk())
        {

            currentRun=&(temp->front->data);
            currentRun->starttime=getClk();
            if(temp->front->data.state==1)
            {
                
               currentRun->resumed=getClk();
               

                
                temp->front->data.waitingtime+=getClk()-temp->front->data.stoppingTime;

                writeShM(temp->front->data.remainingtime+getClk());


                 kill(temp->front->data.pid,SIGCONT);

        fprintf(output,"At time %d process %d %s arr %d total %d remain %d wait %d\n",getClk(),currentRun->id,"resumed",temp->front->data.arrivaltime,temp->front->data.runningtime,temp->front->data.remainingtime,temp->front->data.waitingtime);

                  currentRun->state=0;


                printf("\nI am process with remaining %d,I am continueing my Job. and time is..%d. and my remaining time =.. %d\n " ,temp->front->data.priority,getClk() ,temp->front->data.remainingtime);
              

            }
            else if (temp->front->data.state!=0)
            {
            Pid=fork();

                
                 currentRun->stoppingTime=0;
                currentRun->resumed=getClk();
                writeShM(currentRun->remainingtime+getClk());
                    temp->front->data.pid=Pid;
                    temp->front->data.state=0;
        fprintf(output,"At time %d process %d %s arr %d total %d remain %d wait %d\n",getClk(),currentRun->id,"started",temp->front->data.arrivaltime,temp->front->data.runningtime,temp->front->data.remainingtime,temp->front->data.waitingtime);

            
            if (Pid == 0)
                    {
                       
                       
                        


                        printf("\nForking .. %d\n " ,temp->front->data.pid);

                        printf("\nI am process with remaining .. %d I started  running now and the time is ...%d and remaining time .. %d \n " ,temp->front->data.remainingtime,getClk(),temp->front->data.remainingtime);
                        char rem[4];
                        sprintf(rem,"%d",temp->front->data.remainingtime);
                    
                        char* const p[] = {"./process.o",rem,NULL};
                        execvp(p[0],p);
                       
                     
                    }
            }

                    if (currentRun->state==0)
                    {
                        // if (getClk()-currentTimeStep==1)
                        // {
                        //     currentRun->remainingtime--;
                        // }
                        
                        


                        printf("\nI am process with remaining .. %d I am running now and the time is ...%d and remaining time .. %d \n " ,temp->front->data.priority,getClk(),temp->front->data.remainingtime);

                    }
    
        }
        }   
    }        
    return outqueue;
}
