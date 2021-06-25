#include "headers.h"

/* Modify this file as needed*/
int remainingtime;

int main(int agrc, char *argv[])
{
    initClk();
    initShmem();
    //TODO it needs to get the remaining time from somewhere
  
    remainingtime =0;
    int start =  getClk();
    int wait=0;
    int last =0;
    int running=atoi(argv[1]);
  
    // while (remainingtime <running )
    // {
    //     remainingtime = (getClk() - start - readShM() );
    //             // printf("\nI am inside the process and remaining time is ..%d ",remainingtime);



    // }

        while (remainingtime <readShM() )
    {
        remainingtime = (getClk());
                // printf("\nI am inside the process and remaining time is ..%d ",remainingtime);



    }  
   
    exit(getClk());

    

    
    return 0;
}
