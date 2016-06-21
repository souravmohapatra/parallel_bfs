#include<iostream>
#include<fstream>
#include<sstream>
#include<queue>
#include<list>
#include<stdlib.h>
#include<stdio.h>
#include<string>
#include<omp.h>
#include<time.h>

#define BUFF_SIZE 1000
using namespace std;


int nv, ed;
list<int> FQ;
list<int> *adj;

void display()
{
	fstream fout;
	cout << "Serial: ";
	fout.open("res.txt", ostream::out);
	for(list<int>::iterator i = FQ.begin(); i!=FQ.end(); ++i)
	{
		cout << *i << " ";
	}
}

void BFSserial(int s)
{
	int v[nv+1];
	for(int i=0;i<=nv;i++)
		v[i] = 0;
	list<int> Q;
	Q.push_back(s);
	v[s] = 1;
	
	while(!Q.empty())
	{
		int u = Q.front();
		//FQ.push_back(u);
		//cout << u << "  ";
		Q.pop_front();
		for(list<int>::iterator i = adj[u].begin(); i!=adj[u].end(); ++i)
		{
			int vv = *i;
			if(v[vv] == 0)
			{
				v[vv] = 1;
				Q.push_back(vv);
			}
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

void BFSparallel(int s)
{
	int v[nv+1];
	for(int i=0;i<=nv;i++)
		v[i] = 0;
	
	list<int> Q;
	Q.push_back(s);
	v[s] = 1;
	list<int>::iterator j;
	int count = 1;
    	#pragma omp parallel for ordered schedule(dynamic, 100)
	for(int i=0;i<nv;i++)
	{
	    if(count < nv)
	    {
		int u = Q.front();
		cout << u << "  ";
		Q.pop_front();
		for(j = adj[u].begin(); j!=adj[u].end(); ++j)
		{
			int vv = *j;
			if(v[vv] == 0)
			{
				v[vv] = 1;
				Q.push_back(vv);
				count++;
			}
			
		}
           }
	}
}





int main()
{
	struct timespec start, finish;
	double elapsed;
	takeInput();		
	int s = 1;
	cout << "BFS on a graph of " << nv << " nodes" << endl << endl;
	
	//clock_t start = clock();
	//BFSserial(s);
	//double time = double(clock() - start)/CLOCKS_PER_SEC;
	
	clock_gettime(CLOCK_REALTIME, &start);
	BFSserial(s);
	clock_gettime(CLOCK_REALTIME, &finish);
	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;	
	printf("Serial implementation time: %f\n", elapsed);
	//cin.ignore();	
	//clock_t start2 = clock();
	//BFSparallel(s);
	//time = double(clock() - start2)/CLOCKS_PER_SEC;
	
	clock_gettime(CLOCK_REALTIME, &start);
//	BFSparallel(s);
	clock_gettime(CLOCK_REALTIME, &finish);
	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;	
	printf("Parallel implementation time: %f\n", elapsed);

	//display();
	return 0;
}
