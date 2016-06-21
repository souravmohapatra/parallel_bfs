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
	
#define NUM_THREADS	16
#define BUFF_SIZE	1000
using namespace std;


pthread_mutex_t mutex_work, mutex_queue, mutex_update;
pthread_cond_t cond_work, cond_queue;
int _sync_count;

int _sig_busy, nv, ed;
sem_t _check, _check_a;
int _shared_vertex;
int _th_busy;
int *visited;

list<int> Q;
list<int> _global_Q;
list<int> *adj;

void t_pool(void *x)
{
	while(1)
	{
	/*	pthread_mutex_lock(&mutex_work);
			while(1)
			{
				_sync_count++;
				pthread_cond_wait(&cond_work, &mutex_work);
				break;
			}
		pthread_mutex_unlock(&mutex_work);
		_th_busy = 1;
	*/
		int curr_vertex = _shared_vertex;	
		pthread_mutex_lock(&mutex_work);
		if(_shared_vertex != -1)
			_shared_vertex = -1;
		else
		{
			pthread_mutex_unlock(&mutex_work);
			continue;
		}
		pthread_mutex_unlock(&mutex_work);
		

		for(list<int>::iterator i = adj[curr_vertex].begin(); i!=adj[curr_vertex].end(); i++)
		{
			int v = *i;
			if(visited[v] == 0)
			{
				visited[v] = 1;
				_global_Q.push_back(v);
			}
		}

		
	}	
}


void BFS(int s)
{
	_sig_busy = 0;
	visited = new int[nv+1];
	for(int i = 0;i<=nv;i++)
		visited[i] = 0;
	Q.push_back(s);
	visited[s] = 1;
	
	while(1)
	{	
		while(1)
		{
			
			_shared_vertex = Q.front();
			_th_busy = 0;
		
		/*	pthread_mutex_lock(&mutex_work);
				pthread_cond_signal(&cond_work);
			pthread_mutex_unlock(&mutex_work);	
		
			usleep(30);

			if(_th_busy == 0)
				continue;
		
			_sync_count--;
		*/	
		//	cout  << Q.front() << "  ";
			Q.pop_front();
			if(Q.empty())
				break;
			
			while(_shared_vertex != -1);
		}
		while(_sync_count != NUM_THREADS);
		
	//	_global_Q.unique();
		Q = _global_Q;
		_global_Q.clear();
	
		if(Q.empty())
		{
			break;
		}
	}	
}


void takeInput()
{
        fstream fin; int rc, u;
        fin.open("f2.txt", fstream::in);
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
	
	pthread_mutex_init(&mutex_work, NULL);
	pthread_mutex_init(&mutex_update, NULL);
	pthread_cond_init(&cond_work, NULL);
	pthread_mutex_init(&mutex_queue, NULL);
	pthread_cond_init(&cond_queue, NULL);

	_sync_count = 0;
	_shared_vertex = -1;
	sem_init(&_check, 0, 0);
	sem_init(&_check_a, 0, 0);
	takeInput();
	for(int i=0;i<NUM_THREADS;i++)
	{
		pthread_create(&p[i], NULL, t_pool, (void *)NULL);
	}
	
	cout << "File processed, awaiting proceeed input";
	cin.ignore();

	_sig_busy = 0;
	
	clock_t start = clock();
	BFS(1);
	double time = double(clock() - start)/CLOCKS_PER_SEC;

	cout << "Time taken: " << time << endl;
	return 0;
}
