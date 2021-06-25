
#include "Queue.h"
void clearResources(int);
int msgq_id;
int schedPID;
int clockPID;
int main(int argc, char *argv[])
{
    signal(SIGINT, clearResources);
    // TODO Initialization
    // 1. Read the input files.
    queue*q;
    q=initialize();
    FILE *fp;
    char str[100];
    char*filename="processes.txt";
    fp=fopen(filename,"r");
    if(fp==NULL)
    {
    	printf("Could not open file %s",filename);
    	exit(1);
    }
   
    	char*buffer =NULL;
    	size_t length=0;
    	ssize_t read;
    	int count=-1;
    	read=getline(&buffer,&length,fp);
    	
    	int ch;
    	
    	while(!feof(fp))
    	{	
    		
    		
    		Process p;
    		count++;
    		fscanf(fp,"%d",&p.id);
    		fscanf(fp,"%d",&p.arrivaltime);
    		fscanf(fp,"%d",&p.runningtime);
    		fscanf(fp,"%d",&p.priority);
    		printf("process %d arrived %d running %d\n",p.id,p.arrivaltime,p.runningtime);
    		enqueue(q,p);
    		
    	}
    	
    	fclose(fp);
    
    int algo,quantum;
  
    algo=atoi(argv[1]);
    if(algo==5)
    {
    
    quantum=atoi(argv[2]);
    }
    // 2. Read the chosen scheduling algorithm and its parameters, if there are any from the argument list.
    // 3. Initiate and create the scheduler and clock processes.
    
    msgq_id = msgget(500, 0666 | IPC_CREAT);
     if (msgq_id == -1)
    {
        perror("Error in create");
        exit(-1);
    }
    
    clockPID=fork();
  
    if(clockPID==0)
    {
    	char*const clock[]={"./clk.o",NULL};
    	execvp(clock[0],clock);
    	
    }
    
    schedPID=fork();
    
    if(schedPID==0)
    {
    	char algorithm [2];
    	
    	char numberofprocesses[4];
    	sprintf(algorithm,"%d",algo);
    	if(algo==5)
    	{
    		char quant [2];
		sprintf(quant,"%d",quantum);
		sprintf(numberofprocesses,"%d",count);
    	char*const scheduler[]={"./scheduler.o",algorithm,quant,numberofprocesses,NULL};
	execvp(scheduler[0],scheduler);    	
	}
	else
	{
        sprintf(algorithm,"%d",algo);
        printf(" I am %d",algo );
		sprintf(numberofprocesses,"%d",count);
    	char*const scheduler[]={"./scheduler.o",algorithm,numberofprocesses,NULL};
    	execvp(scheduler[0],scheduler);
	}
    	
    	
    	
    }
    // 4. Use this function after creating the clock process to initialize clock.
    initClk();
    // To get time use this function. 
    
    
    // TODO Generation Main Loop
    // 5. Create a data structure for processes and provide it with its parameters.
    // 6. Send the information to the scheduler at the appropriate time.
   
    struct msgbuff message;
    int send_val;
    message.mtype=1000;
    int x = getClk();
    printf("Current Time is %d\n", x);
    while(!isempty(q))
    {
	
	if(q->front->data.arrivaltime<=getClk())
	{
	
	message.message=q->front->data;
	
    send_val = msgsnd(msgq_id, &message, sizeof(message.message), !IPC_NOWAIT);
    if(send_val==-1)
    {
    	perror("Errror in send");
    }
    dequeue(q);
    }
    }
    while(1){}
    
    // 7. Clear clock resources
    
}

void clearResources(int signum)
{
    //TODO Clears all resources in case of interruption
    msgctl(msgq_id, IPC_RMID, (struct msqid_ds *)0);
    destroyClk(true);
    kill(schedPID,SIGKILL);
    kill(clockPID,SIGKILL);
    kill(getpid(),SIGKILL);
}
