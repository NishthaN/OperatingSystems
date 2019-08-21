//
//  philosopher.c
//  
//


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX 100

int N; //Number of philosophers
int n = 0;  //variable for counting number of philosophers eaten

enum {Thinking,Eating,Hungry} state[MAX];
pthread_mutex_t mutex;
pthread_cond_t cond_var[MAX];
int count[MAX];  //Count philosopher number of eats
int flag = 1;

void *phil() {
    int i = n++;
    
    while(n > 0) {
        //Thinking
        state[i] = Thinking;
        printf("Philosoper %d is Thinking\n",i);
        sleep((rand()%3 + 1));  //generate sleep for 1 to 3 secs
        
        //Hungry
        state[i] = Hungry;
        printf("Philosoper %d is Hungry\n",i);
        
        if(n > 0) {
        pthread_mutex_lock(&mutex);  //acquire lock
        while(state[i] != Hungry || state[(i+1)%N] == Eating || state[(i+N-1)%N] == Eating )  //To check if neighbours are eating
            pthread_cond_wait(&cond_var[i],&mutex);  //Wait
        state[i] = Eating;
        printf("Philosopher %d picked up chopstick %d and %d \n",i,i,(i+1)%N);
            if(n <= 0){   //To check if N eating done
                pthread_mutex_unlock(&mutex);
                flag = 0;
                break;
            }
        --n;
        pthread_mutex_unlock(&mutex);  //Lock unlocked
            
            
        //Eating
        
            int x = (rand()%3 + 1);   //Eating count of philosopher
            count[i] += x;
        printf("Philosoper %d is Eating\n",i);
        sleep(x);  //generate sleep for 1 to 3 secs
        
        pthread_mutex_lock(&mutex);  //Locked
        printf("Philosopher %d put down chopstick %d and %d \n",i,i,(i+1)%N);
        state[i] = Thinking;
        if(count[(i+1)%N] <= count[(i+N-1)%N]) {  //For seeing which neighbour hasn't eaten
        if(state[(i+1)%N] == Hungry)    //Signal if hungry
            pthread_cond_signal(&cond_var[(i+1)%N]);
        else if(state[(i+N-1)%N] == Hungry)
            pthread_cond_signal(&cond_var[(i+N-1)%N]);
        }
        else {
            if(state[(i+N-1)%N] == Hungry)
                pthread_cond_signal(&cond_var[(i+N-1)%N]);
            else if(state[(i+1)%N] == Hungry)
                pthread_cond_signal(&cond_var[(i+1)%N]);
        }
        pthread_mutex_unlock(&mutex); //Unlock
        }
        else {   //To check if N eating done
            flag = 0;
                break;
            }
        
    }
    pthread_exit(0);
}

int main(int argc,char *argv[]) {
    time_t t1;
    time(&t1);
    time_t t2;
    N = atoi(argv[1]);  //Number of neighbours
    char c;
    
    pthread_t tid[N];
    pthread_attr_t attr[N];
    
    do{
        pthread_mutex_init(&mutex,NULL);
    for(int i = 0; i < N;++i) {  //Loop to create philosopher threads
        pthread_attr_init(&attr[i]);
        pthread_cond_init(&cond_var[i],NULL);
        pthread_create(&tid[i],&attr[i],phil,NULL);
    }
        while(flag!=0) ;
        flag = 1;
        for(int i = 0; i < N;++i)
            pthread_cancel(tid[i]);
        
    printf("Enter Y/y to continue N/n to leave: ");
        scanf(" %c",&c);
        n = 0;
    }while(c == 'Y' || c == 'y');
    
    //Summary
    printf("Number of philosophers = %d \n",N);
    for(int i = 0; i < N;++i)
        printf("Philosopher %d ate %d times \n",i,count[i]);
    time(&t2);
    printf("Time elasped : %.2f \n",difftime(t2,t1));
    
    
    return 0;
}

