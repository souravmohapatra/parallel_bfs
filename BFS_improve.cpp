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
	
#define NUM_THREADS	8
#define BUFF_SIZE	1000
using namespace std;

pthread_mutex_t mux;
pthread_mutex_t thd;
pthread_cond_t cond;
list<int> Q;
list<int> *th;
list<int> *adj;
int *visited;
int th_complete, nv;


void t_pool(void *x)
{
	int id = (int)x;
	while(1)
	{
		pthread_mutex_lock(&mux);
			while(1)
			{
				pthread_cond_wait(&cond, &mux);
				break;
			}
		pthread_mutex_unlock(&mux);
		list<int>::iterator i;
		for(i = th[id].begin(); i!=th[id].end(); ++i)
		{
			int ver = *i;
			list<int>::iterator j;
			for(j = adj[ver].begin(); j!=adj[ver].end(); ++j)
			{
				int v = *j;
				if(visited[v] == 0)
				{
					//pthread_mutex_lock(&update);
					visited[v] = 1;
					Q.push_back(v);
					//pthread_mutex_unlock(&update);
				}
			}
		}
		pthread_mutex_lock(&thd);
		th_complete++;
		pthread_mutex_unlock(&thd);
	}		

}



void wakeSignal()
{
	pthread_mutex_lock(&mux);
		pthread_cond_broadcast(&cond);
	pthread_mutex_unlock(&mux);
}

void BFS(int s)
{
	visited = new int[nv+1];
	for(int i = 0;i<=nv;i++)
		visited[i] = 0;
	Q.push_back(s);
	visited[s] = 1;
	int th_count = 0;
	while(1)
	{
		th_complete = 0;
		while(!Q.empty())
		{
			int temp = Q.front();
			th[th_count%(NUM_THREADS-1)].push_back(temp);
			Q.pop_front();
//			cout << temp << "   ";
			th_count++;
		}
		th_count = th_count%(NUM_THREADS-1);
		wakeSignal();
		while(th_complete!=(NUM_THREADS-1));
		if(Q.empty())
		{
			break;	
		}
	}

}


void takeInput()
{
        fstream fin; int rc, u;
        fin.open("graph100.txt", fstream::in);
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


int main()
{
	pthread_t p[NUM_THREADS];
	struct timespec start, finish;
	double elapsed;
	th = new list<int>[NUM_THREADS];
	pthread_mutex_init(&mux, NULL);
	pthread_mutex_init(&thd, NULL);
	pthread_cond_init(&cond, NULL);

	
	takeInput();
	for(int i=0;i<NUM_THREADS;i++)
	{
		pthread_create(&p[i], NULL, t_pool, (void *)i);
	}
	
	cout << "File processed, awaiting proceeed input";
	cin.ignore();

	
	clock_gettime(CLOCK_REALTIME, &start);
	BFS(1);
	clock_gettime(CLOCK_REALTIME, &finish);
	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;	
	printf("Pthread implementation time: %f\n", elapsed);

	return 0;
}
