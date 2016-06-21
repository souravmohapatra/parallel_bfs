#include<iostream>
#include<pthread.h>
#include<list>
#include<stack>
#include<vector>

#define GRANULARITY	4
#define INF 		1000000
#define MAX_THREAD	4
using namespace std;

class Graph
{
	pthread_mutex_t mutex1, mmutex2;
	pthread_t t[MAX_THREAD];
	int nv;
	list <int> *adj;
	vector <int> status;
	vector <int> level;
	vector <int> pred;
 	vector <list<int>> out;	
	void merge();
	void divide();
	void parallel_part();
	int out_count;

   public:
	Graph(int nv);
	void init();
	void addEdge(int v1, int v2);
	void BFS(int s):
};

Graph::Graph(int nv)
{
	this->nv = nv;
	adj = new list<int>[nv];
	out = new vector<int>[nv];
}

void Graph::addEdge(int v1, int v2);
{
	adj[v1].push_back(v2);
	adj[v2].push_back(v1);
}

void Graph::init()
{
	out_count = 0;
	iterator::list<int> i;
	int k;
	status.assign(nv, 0);
	level.assign(nv, INF);
	pred.assign(nv, -1);

}
void Graph::BFS(int s)
{
	status[s] = 1;
	level[s] = 0;
	
	list<int> inp, out;
	inp.push_back(s);
	while(!inp.empty)
	{
		divide();
		merge();
	}
	
	stack<int> stk;
	iterator::vector<int> i;
	for(i=pred.end(); i!=pred.begin(); --i)
	{
		stk.push(*i);
	}
	
	while(!stk.empty())
	{
		printf("%d-", stk.pop());	
	}
}


void Graph::divide()
{
	pthread_mutex_init(&mutex1, NULL);
	pthread_mutex_init(&mutex2, NULL);
	iterator::list<int> i;
	int count = 0;
	int t_count = 0;
	for(i=inp.begin();i!=inp.end();++i)
	{
			/* create a thread and run parallel_part */
			int x = *i
			int rc = pthread_create(&t[t_count], NULL, parallel_part, (void *)x)
	}
	
	/* wait for the threads to synchronize */
}

void Graph::merge()
{	
	/* union of all the out to produce the next inp */
}

void Graph::parallel_part(void *x)
{
	list<int> out_tmp;
	int inp;
	*out = new list<int>;
	inp = (int*)x;

		int u = inp;
		for(iterator::list<int> j = adj[u].begin(); i!=adj[u].end(); ++i)
		{
			int u = *j;
			if(status[v] == 0)
			{
				status[v] = 1;
				level[v] = level[u] + 1;
				pthread_mutex_lock(&mutex);
				pred[v] = u;	
				pthread_mutex_unlock(&mutex);
				out_tmp.push_back(v);
			}
		}
		status[u] = 2;
	
	pthread_mutex_lock(&mutex2);
		out.push_back(out_tmp);
	pthread_mutex_unlock(&mutex2);
}	
