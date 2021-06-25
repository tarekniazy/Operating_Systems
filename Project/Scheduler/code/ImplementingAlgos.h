
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

int Pid;

Process* currentRun;

int currentTimeStep=0;

FILE*output;
int stat_loc;
void childrenTerminating(int);
bool available=1;

queue* FCFS(int procs)
{		
	
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
    int last=-1;
     //printf("creating file..");
    //printf("entering fcfs..");
	output = fopen("scheduler.log","w");
    //printf("writing..");
    fprintf(output,"#At time x process y state arr w total z remain y wait k\n");
     //printf("written..");
    while(numberofprocesses>0)
    {	
    	recv_val = msgrcv(msgq_id, &message, sizeof(message.message), 0, !IPC_NOWAIT);
    		
    		 if (recv_val != -1) {//not received
            		//perror("Error in receive");
            	
            	
            		if(last!=message.message.id)
            		{
            			last=message.message.id;
            					message.message.remainingtime=message.message.runningtime;
            					printf("running time: %d\n",message.message.runningtime);
            			enqueue(temp,message.message);
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
    	dequeue(temp);
    	available =0;
    }
    }
    }
    }
    return outqueue;
}
/////////////////////////////////////////////////////////////////////
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
            printf("process %d, finished %d",running.id,running.finishtime);         
            fprintf(output,"At time %d process %d finished arr %d  remain %d wait %d ",running.finishtime,running.id,running.arrivaltime,running.remainingtime,running.waitingtime);    
            fprintf(output,"TA %d WTA %0.2f \n",running.turnaround_time,running.weighted_turnaround_time);
            enqueue(outqueue,running);
            numberofprocesses--;
            running.state=-1;
            available =1; 
        }

    }    
}

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
            fprintf(output,"TA %d WTA %0.2f \n",currentRun->turnaround_time,currentRun->weighted_turnaround_time);
            enqueue(outqueue,*currentRun);
            numberofprocesses--;
            currentRun->state=-1;
            
            available =1; 
        }

    }    
}

/////////////////////////////SJF///////////////////////////
queue* SJF(int procs)
{		
	
	
    initClk();
    numberofprocesses = procs;
    
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
    fprintf(output,"#At time x process y state arr w total z remain y wait k\n");

    while ( numberofprocesses > 0)
    {
      
        recv_val = msgrcv(msgq_id, &message,sizeof(message.message),0, IPC_NOWAIT);
        if (recv_val != -1)
        {
            if (message.message.id != last)
            {

                last = message.message.id;
                message.message.remainingtime = message.message.runningtime;  
           
            			

           ///////////////////////////////////////////////
           //babadel elnodes law elrunningtime a2al
    node* newNode = (node*)malloc(sizeof(node)); 
    newNode->data = message.message;
    newNode->priority = message.message.runningtime;
    newNode->next = NULL;
    if (Head == NULL)
    {
    
        Head  = newNode;
        printf("Head NULL: %d\n",Head->data.id);
	printf("Head priority: %d, newpriority: %d\n",Head ->priority,newNode->priority);
	
    }
   
   
    
    
    else if (Head ->priority > newNode->priority)
    {
    	printf("Head fel elseif: %d\n",Head->data.id);
	printf("Head arrival: %d, newarrival: %d\n",Head ->data.arrivaltime,newNode->data.arrivaltime);
        newNode->next = Head ;
        Head  = newNode;
    }
    else
    {
    printf("Head fel else: %d\n",Head->data.id);
	printf("Head priority: %d, newpriority: %d\n",Head ->priority,newNode->priority);
        node* temp = Head ;
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
           ////////////////////////////////////////////////// 			
            			
            			}
            			}
            			
            			
            			
		
		
	
		
    
    if (available == 1)
    {
    if(Head!=NULL)
    {
    if(Head->data.arrivaltime<=getClk())
	{
	
    	running=Head->data;
    	running.starttime=getClk();
    	int Pid=fork();
        writeShM(running.remainingtime+getClk());

    	if (Pid == 0)
                {
                    
                    char rem[4];
                    sprintf(rem,"%d",Head->data.runningtime);
                    char* const p[] = {"./process.o",rem,NULL};
                    execvp(p[0],p);

                }
    	running.waitingtime=running.starttime-running.arrivaltime;
    	fprintf(output,"At time %d process %d %s arr %d total %d remain %d wait %d\n",getClk(),running.id,"started",running.arrivaltime,running.runningtime,running.runningtime,running.waitingtime);
    	//pop(temp);
    	pop(&Head);
    	available =0;
    }
    }
    }
    }
    
    return outqueue;
}
///////////////////////////////////HPF/////////////////////////

queue* HPF(int procs)
{		
	
	printf("HPF I am here ......");
    initClk();
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
    //printf("writing..");
    fprintf(output,"#At time x process y state arr w total z remain y wait k\n");
     //printf("written..");
  
    
    while(numberofprocesses>0)
    {	currentTimeStep=getClk();
    	recv_val = msgrcv(msgq_id, &message, sizeof(message.message), 0, !IPC_NOWAIT);
    		
    		 if (recv_val != -1) 
             {//not received
            		//perror("Error in receive");
            	
            	
            		if(last!=message.message.id)
            		{
            		last= message.message.id;
                        message.message.waitingtime=0;
                        message.message.state=2;
                        
            			message.message.remainingtime=message.message.runningtime;
                        if (!isempty(temp))
                        {
                            if ((message.message.priority)<(temp->front->data.priority))
                            {
                             kill(temp->front->data.pid,SIGTSTP);

                            temp->front->data.remainingtime-=(getClk()-temp->front->data.resumed);

                                printf("\nI am process with priority %d,I have been stopped by priority.. %d\n " ,temp->front->data.priority,message.message.priority);
                                printf("\nTime now is.. %d\n " ,getClk());
        fprintf(output,"At time %d process %d %s arr %d total %d remain %d wait %d\n",getClk(),temp->front->data.id,"stopped",temp->front->data.arrivaltime,temp->front->data.runningtime,temp->front->data.remainingtime,temp->front->data.waitingtime);


                               
                                printf(" \nMy Id ...%d\n",message.message.id);

                             

                                temp->front->data.state=1;  //Stopped
                                temp->front->data.stoppingTime=getClk();
                            }

                        }
                     
                        
                        enqueuePriority(&(message.message),temp);
                        printPriority(temp);

            		}
            }
		
        if (temp->front->data.state==-1)
        {
            
            printf("\nI am quiting now :( %d  and time is %d  and remaining time is %d \n " ,temp->front->data.priority,getClk(),temp->front->data.runningtime);
            
            dequeue(temp);
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

                
                 currentRun->stoppingTime=0;
                currentRun->resumed=getClk();

                currentRun->starttime=getClk();
                writeShM(currentRun->remainingtime+getClk());
                    temp->front->data.pid=Pid;
                    temp->front->data.state=0;

            
        fprintf(output,"At time %d process %d %s arr %d total %d remain %d wait %d\n",getClk(),currentRun->id,"started",temp->front->data.arrivaltime,temp->front->data.runningtime,temp->front->data.remainingtime,temp->front->data.waitingtime);

            
            if (Pid == 0)
                    {
                       
                       
                         currentRun=&(temp->front->data);


                        printf("\nForking .. %d\n " ,temp->front->data.pid);

                        printf("\nI am process with priority .. %d I started  running now and the time is ...%d and remaining time .. %d \n " ,temp->front->data.priority,getClk(),temp->front->data.remainingtime);
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
                        
                        
                    // temp->front->data.remainingtime-=(getClk()-temp->front->data.resumed);


                        printf("\nI am process with priority .. %d I am running now and the time is ...%d and remaining time .. %d \n " ,temp->front->data.priority,getClk(),temp->front->data.remainingtime);

                    }
    
        }
        }   
    }        
    return outqueue;
}

////////////////////////////////////////SRTN///////////////////////

queue* SRTN(int procs)
{		
	
	printf("SRTN I am here ......");
    initClk();
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
     //printf("creating file..");
    //printf("entering fcfs..");
	output = fopen("scheduler.log","w");
    //printf("writing..");
    fprintf(output,"#At time x process y state arr w total z remain y wait k\n");
     //printf("written..");
  
    
    while(numberofprocesses>0)
    {	currentTimeStep=getClk();
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
                        if (!isempty(temp))
                        {
                        printf("\nnew process entered with remaining %d   compared with the front with reamining%d\n " ,message.message.remainingtime,temp->front->data.remainingtime);
temp->front->data.remainingtime-=(getClk()-temp->front->data.resumed);
                           
                            if ((message.message.remainingtime)<(temp->front->data.remainingtime))
                            {
                                

                                printf("\nI am process with remaining %d,I have been stopped by remaining.. %d\n " ,temp->front->data.remainingtime,message.message.remainingtime);
                                printf("\nTime now is.. %d\n " ,getClk());
                                fprintf(output,"At time %d process %d %s arr %d total %d remain %d wait %d\n",getClk(),currentRun->id,"stopped",temp->front->data.arrivaltime,temp->front->data.runningtime,temp->front->data.remainingtime,temp->front->data.waitingtime);

                                kill(temp->front->data.pid,SIGTSTP);
                                // temp->front->data.remainingtime-=(getClk()-temp->front->data.resumed);

                                temp->front->data.state=1;  //Stopped
                                temp->front->data.stoppingTime=getClk();
                            }

                        }
                     
                        
                        enqueueRemaining(&(message.message),temp);
                        printRemaining(temp);
            		}
            }
		
        if (temp->front->data.state==-1)
        {
            // temp->front->data.remainingtime-=(getClk()-temp->front->data.resumed);

            printf("\nI am quiting now :( %d  and time is %d  and remaining time is %d \n " ,temp->front->data.priority,getClk(),temp->front->data.runningtime);
            
            dequeue(temp);
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
/////////////////////////RR///////////////////////////////
//////////////////////////// MOSTAFA /////////////////////////////////////
queue* RR(int procs, int quantum)
{
    printf("RR I am here ......");
    initClk();
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

    outqueue = initialize();
    queue *temp = initialize();

    int last = -1;
    output = fopen("schedulerrr.log","w");
    fprintf(output,"#At time x process y state arr w total z remain y wait k\n");

    int quantum_max = quantum;
    int current_quantum = 0;
    while(numberofprocesses>0)
    {
        currentTimeStep=getClk();
        recv_val = msgrcv(msgq_id, &message, sizeof(message.message), 0, !IPC_NOWAIT);

        if (recv_val != -1)
        {
            if(last != message.message.id)
            {
                message.message.waitingtime = 0;
                message.message.state = 2;
                last=message.message.id;
                message.message.remainingtime=message.message.runningtime;

                enqueue(temp, message.message);
                printRemaining(temp);
            }
        }

        if (temp->front->data.state == -1)
        {
            printf("\nI am quiting now :( %d  and time is %d  and remaining time is %d \n ", temp->front->data.priority, getClk(), temp->front->data.runningtime);

            dequeue(temp);
            current_quantum = 0;
        }

        if(!isempty(temp))
        {
            if (current_quantum == quantum_max)
            {
                currentRun->remainingtime = (getClk() - currentRun->resumed);
    
                printf("\nI am process with remaining %d,I have been stopped by remaining.. %d\n ", currentRun->remainingtime, message.message.remainingtime);
                printf("\nTime now is.. %d\n ", getClk());
                fprintf(output, "At time %d process %d %s arr %d total %d remain %d wait %d\n", getClk(), currentRun->id, "stopped", temp->front->data.arrivaltime, temp->front->data.runningtime, temp->front->data.remainingtime, temp->front->data.waitingtime);

                kill(temp->front->data.pid, SIGTSTP);
                temp->front->data.state = 1;
                temp->front->data.stoppingTime = getClk();

                enqueue_roundrobin(temp);

            }
            if (temp->front->data.arrivaltime <= getClk())
            {
                currentRun = &(temp->front->data);
                currentRun->starttime = getClk();
                if (temp->front->data.state == 1) // if blocked
                {
                    currentRun->resumed = getClk();
                    temp->front->data.waitingtime += getClk() - temp->front->data.stoppingTime;

                    writeShM(temp->front->data.remainingtime + getClk());

                    kill(temp->front->data.pid, SIGCONT);

                    fprintf(output, "At time %d process %d %s arr %d total %d remain %d wait %d\n", getClk(), currentRun->id, "resumed", temp->front->data.arrivaltime, temp->front->data.runningtime, temp->front->data.remainingtime, temp->front->data.waitingtime);
                    currentRun->state = 0;
                    printf("\nI am process with remaining %d,I am continuing my Job. and time is..%d. and my remaining time =.. %d\n ", temp->front->data.priority, getClk(), temp->front->data.remainingtime);

                    if(currentRun->remainingtime < quantum_max)
                    {
                        quantum_max = currentRun->remainingtime;
                    }
                    else
                    {
                        quantum_max = quantum;
                    }

                }

            }
            else if (temp->front->data.state != 0)
            {
                Pid = fork();

                currentRun->stoppingTime = 0;
                currentRun->resumed = getClk();
                writeShM(currentRun->remainingtime + getClk());
                temp->front->data.pid = Pid;
                temp->front->data.state = 0;
                fprintf(output, "At time %d process %d %s arr %d total %d remain %d wait %d\n", getClk(), currentRun->id, "started", temp->front->data.arrivaltime, temp->front->data.runningtime, temp->front->data.remainingtime, temp->front->data.waitingtime);

                if (Pid == 0)
                {

                    printf("\nForking .. %d\n ", temp->front->data.pid);

                    printf("\nI am process with remaining .. %d I started  running now and the time is ...%d and remaining time .. %d \n ", temp->front->data.remainingtime, getClk(), temp->front->data.remainingtime);
                    char rem[4];
                    sprintf(rem, "%d", temp->front->data.runningtime);

                    char *const p[] = {"./process.o", rem, NULL};
                    execvp(p[0], p);
                }
            }

            if (currentRun->state == 0)
            {
                printf("\nI am process with remaining .. %d I am running now and the time is ...%d and remaining time .. %d \n ", temp->front->data.priority, getClk(), temp->front->data.remainingtime);
                alarm(quantum);
                currentRun->remainingtime -= quantum;

                current_quantum = current_quantum + 1;
            }

        }

    }
return outqueue;

}
//////////////////////////// MOSTAFA /////////////////////////////////////
