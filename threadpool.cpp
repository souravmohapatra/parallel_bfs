#include<pthread.h>
#include<unistd.h>
#include<iostream>

using namespace std;

pthread_mutex_t mutex;
pthread_cond_t cond;
int count;
int x;

void fun1(void *x)
{
	cout << "Fun 1 entered " <<endl;
	while(1)
	{
		pthread_mutex_lock(&mutex);
		while(x!=10)
		{
			cout << "Blocked" << endl;
			pthread_cond_wait(&cond, &mutex);
			cout << "Woke up!" << endl;
			break;
		}
		cout << "I am increasing count: " << pthread_self();
		count++;
		pthread_mutex_unlock(&mutex);
	}	
}

void fun2(void *x)
{
	cout << "Fun 2 entered" << endl;
	while(1)
	{
		pthread_mutex_lock(&mutex);
		while(x!=10)
		{
			cout << "Blocked2" << endl;
			pthread_cond_wait(&cond, &mutex);
			cout << "Woke up!" << endl;
			break;
		}
		cout << "I am increasing count: " << pthread_self();
		count++;
		pthread_mutex_unlock(&mutex);
	}	
	
}

void BFS()
{
	sleep(2);
	while(x!=-1)
	{
		pthread_mutex_lock(&mutex);
		cout << "Enter a number: ";
		cin >> x;
		if(x == 10)
		{
			cout << "Signalling a thread to wake up" << endl;
			pthread_cond_signal(&cond);
		}
		pthread_mutex_unlock(&mutex);
		sleep(2);
	}
		
}
int main()
{
	x = 0;
	count = 0;
	pthread_t p1, p2;
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);
	pthread_create(&p1, NULL, fun1, (void *)NULL);
	pthread_create(&p2, NULL, fun2, (void *)NULL);
	BFS();
	return 0;
}
