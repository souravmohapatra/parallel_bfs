#include<iostream>
#include<vector>
#include<queue>
#include<stack>
#include<set>
#include<omp.h>
#define INF 10000000
using namespace std;

class Graph
{
	int nv;
	int *mat;
	vector <int> V;
	queue <int> Q;
	vector <int> L;
	vector <int> P;
	set <int> F;
    public:
	Graph(int nv);
	void BFS(int s);	
}

Graph::Graph(int nv)
{
	this->nv = nv;
	mat = new int[nv][nv];
}

void Graph::BFS(int s)
{
	V.assign(nv, 0);
	L.assign(nv, INF);
	Q.push(s);
	V[s] = 1;
	L[s] = 0;
	P[s] = -1;
	#pragma omp parallel for
	{
		for(int i=0;i<nv;i++)
		{
			if(Q.empty())
				break;
			int node = Q.front();
			#pragma omp critical
				Q.pop();
			
			#pragma omp parallel for
			{
				for(int j=0;j<nv;j++)
				{
					if(mat[node][j] && !V[j])
					{
						V[j] = true;
						#pragma omp critical
						{
							F.push_back
							P[j] = node;
							L[j] = L[node] + 1;
							Q.push(j);
						}
					}
				}
			}
		
		}
	}
	
	for(int i=0;i<nv;i++)
	{	
		S.push(P[i]
		
}



int main()
{
		
	return 0;
}

