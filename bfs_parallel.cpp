#include<pthread.h>
#include<unistd.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include<queue>
#include<list>
#include<stdlib.h>
#include<stdio.h>
#include<string>
#include<time.h>
#include<semaphore.h>
#include "tbb/tbb.h"
#define NUM_THREADS     	8
#define BUFF_SIZE       	1000
#define NUM_INNER_THREADS 	4	
using namespace std;
using namespace tbb;

int nv, ed;
list<int> FQ;
pthread_mutex_t mux, update, mux_inner;
pthread_mutex_t thd, update_inner;
pthread_cond_t cond, inner;
pthread_barrier_t barrier;
int th_complete_inner;
//list<int> Q;

concurrent_queue<int> Q;
list<int> *adj;
list<int> *th1, *th2;
int *visited;
int th_complete;
int die = 1;
struct timespec start, finish;
double elapsed;
int s = 1;
int choice = -1;
fstream ff;



void display(int dis)
{
//	ff << dis << "   ";
	cout << dis << "  ";
}

void BFSserial(int s)
{
	list<int> Qc;
	Qc.clear();
        for(int i=0;i<=nv;i++)
                visited[i] = 0;

        Qc.push_back(s);
        visited[s] = 1;

        while(!Qc.empty())
        {
                int u = Qc.front();
                display(u);
		//cin.ignore();
                Qc.pop_front();
                for(list<int>::iterator i = adj[u].begin(); i!=adj[u].end(); ++i)
                {
                        int vv = *i;
                        if(visited[vv] == 0)
                        {
                                visited[vv] = 1;
                                Qc.push_back(vv);
                        }
                }
        }
}

void BFSparallelOpenMP(int s)
{
        for(int i=0;i<=nv;i++)
                visited[i] = 0;

        Q.push(s);
        visited[s] = 1;
        
	list<int>::iterator j;
        while(!Q.empty())
        {
                int u;
		Q.try_pop(u);
                display(u);
		int size = adj[u].size();
		j = adj[u].begin();
		int vv;
		#pragma omp parallel for schedule(dynamic, 24)
                for(int k=0; k<size; k++)
                {
			
			#pragma omp critical (iterate)
			{
                        	vv = *j;
				++j;
			}

			if(visited[vv] == 0)
                        {
                       		visited[vv] = 1;
				Q.push(vv);
                       	}
			

                }
        }
}

void* t_pool(void *x)
{
        int id = *((int*)x);

        while(die)
        {
                pthread_mutex_lock(&mux);
                        while(1)
                        {
                                pthread_cond_wait(&cond, &mux);
                                break;
                        }
                pthread_mutex_unlock(&mux);
                
		list<int>::iterator i;
		if(choice == 1)
		{
                	while(!th1[id].empty())
                	{
                       	 	int ver = th1[id].front();
				th1[id].pop_front();
				list<int>::iterator j;
				for(j = adj[ver].begin(); j != adj[ver].end(); ++j)
				{
					int vv = *j;	
					if(visited[vv] == 0)
					{
						visited[vv] = 1;
						Q.push(vv);
					}
				}		
                	}
		}
		else
		{
                	while(!th2[id].empty())
                	{
                       	 	int ver = th2[id].front();
				th2[id].pop_front();
				list<int>::iterator j;
				for(j = adj[ver].begin(); j != adj[ver].end(); ++j)
				{
					int vv = *j;	
					if(visited[vv] == 0)
					{
						visited[vv] = 1;
						Q.push(vv);
					}
				}		
                	}
		}
		
		pthread_mutex_lock(&update);
                th_complete++;
		pthread_mutex_unlock(&update);
        }
	
	pthread_exit(NULL);

}

void wakeSignal()
{
        pthread_mutex_lock(&mux);
                pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mux);
}

void BFSparallelPthreads(int s)
{
	Q.clear();
        for(int i = 0;i<=nv;i++)
                visited[i] = 0;
        Q.push(s);
        visited[s] = 1;
        int th_count = 0;
        while(1)
        {
                int temp;
                th_complete = 0;
                while(!Q.empty())
                {
			Q.try_pop(temp);
			if(choice == 1)
                        	th1[th_count%(NUM_THREADS-1)].push_back(temp);
			else
				th2[th_count%(NUM_THREADS-1)].push_back(temp);
                        display(temp);
                        th_count++;
                }
                th_count = th_count%(NUM_THREADS-1);
                wakeSignal();
                while(th_complete!=(NUM_THREADS)-1)
		{
			if(!Q.empty())
			{
				Q.try_pop(temp);
				display(temp);
				if(choice == -1)
                        		th1[th_count%(NUM_THREADS-1)].push_back(temp);
				else
					th2[th_count%(NUM_THREADS-1)].push_back(temp);
				th_count++;
			}
		}

		choice *= -1;

                if(Q.empty())
                {
                        break;
                }
        }

}
void takeInput(char *s)
{
        fstream fin; int rc, u;
        fin.open(s, fstream::in);
        stringstream ss;
        char buff[BUFF_SIZE];
        char bkp[BUFF_SIZE];
        fin.getline(buff, BUFF_SIZE);

        ss << buff;
        ss.getline(buff,10,',');
        nv = atoi(buff);

        adj = new list<int>[nv+1];
        for(int i=0;i<nv;i++)
        {
                fin.getline(buff, BUFF_SIZE);

                stringstream sts(buff);
                sts.getline(buff,10,',');
                u = atoi(buff);
                int temp;
                while(1)
                {
                        sts.getline(buff,10,',');
                        temp = atoi(buff);
                        if(!temp)
                                break;
                        adj[u].push_back(temp);
                }
        }

}


void openMP()
{
	clock_gettime(CLOCK_REALTIME, &start);
        BFSparallelOpenMP(s);
        clock_gettime(CLOCK_REALTIME, &finish);
        elapsed = (finish.tv_sec - start.tv_sec);
        elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
        printf("OpenMP implementation time: %f\n", elapsed);
}

void ser()
{
	clock_gettime(CLOCK_REALTIME, &start);
        BFSserial(s);
        clock_gettime(CLOCK_REALTIME, &finish);
        elapsed = (finish.tv_sec - start.tv_sec);
        elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
        printf("Serial implementation time: %f\n", elapsed);

}

void pthd()
{
	clock_gettime(CLOCK_REALTIME, &start);
        BFSparallelPthreads(s);
        clock_gettime(CLOCK_REALTIME, &finish);
        elapsed = (finish.tv_sec - start.tv_sec);
        elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
        printf("Pthread implementation time: %f\n", elapsed);
}

int main(int argc, char *argv[])
{
        takeInput(argv[1]);
        cout << "Perform BFS on a graph of " << nv << " vertices, awaiting proceed input";
        cin.ignore();
	int s = 1;	
	visited = new int[nv+1];
	
	ser();
	//sleep(1);
	
	//openMP();
	//sleep(1);
        
	pthread_t p[NUM_THREADS];
        th1 = new list<int>[NUM_THREADS];
        th2 = new list<int>[NUM_THREADS];
        pthread_mutex_init(&mux, NULL);
        pthread_mutex_init(&mux_inner, NULL);
        pthread_mutex_init(&update_inner, NULL);
        pthread_cond_init(&inner, NULL);
        pthread_cond_init(&cond, NULL);
        pthread_mutex_init(&thd, NULL);
        pthread_mutex_init(&update, NULL);
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_barrier_init(&barrier, NULL, NUM_THREADS);

	int arg[NUM_THREADS];	
        for(int i=0;i<NUM_THREADS;i++)
        {
		arg[i] = i;
                pthread_create(&p[i], &attr, t_pool, (void *)(arg+i));
        }
	
	
	pthd();
	//sleep(1);


	die = 0;
	wakeSignal();
        for(int i=0;i<NUM_THREADS;i++)
        {
                pthread_join(p[i], NULL);
        }
	
        return 0;
}


