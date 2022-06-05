#include <iostream>
#include <vector>
#include <string>
#include "monitor.h"

int const threadsCounts = 4;

int const bufferSize = 9;


class Buffer
{
private:
	Semaphore mutex;
	Semaphore empty;
	Semaphore full;
	Semaphore semA;
	Semaphore semB;

	std::vector<std::string> values;

	void print(std::string name)
	{
		std::cout << "\n ###" << name << " " << values.size() << "[";
		for (auto v : values)
			std::cout << v << ",";
		std::cout << "] ###\n";
	}

public:
	Buffer() : mutex(1), empty(0), full(bufferSize), semA(0), semB(0) {}

	void produceA()
	{
		full.p();
		mutex.p();
		values.push_back("A");
		print("produced A");

		if (values.size() == 1) {
			semA.v();
		}
		if (values.size() > 3){
			empty.v();
		}
		mutex.v();
	}

	void produceB()
	{
		full.p();
		mutex.p();
		values.push_back("B");
		print("produced B");

		if (values.size() == 1) {
			semB.v();
		}
		if (values.size() > 3){
			empty.v();
		}
		mutex.v();
	}

	std::string consumeA()
	{
		semA.p();
		empty.p();
		mutex.p();

		auto value = values.front();
		values.erase(values.begin());
		print("consumed A");
		full.v();

		std::string front = values.front();
        if(front == "A"){semA.v();}
        else{semB.v();}

		mutex.v();
		return value;
	}

	std::string consumeB()
	{
		semA.p();
		empty.p();
		mutex.p();

		auto value = values.front();
		values.erase(values.begin());
		print("consumed B");
		full.v();

		std::string front = values.front();
        if(front == "A"){semA.v();}
        else{semB.v();}

		mutex.v();
		return value;
	}
};

Buffer buffer;

void* threadProdA(void* arg)
{
	for (int i = 0; i < 20; ++i)
	{
		buffer.produceA();
	}
	return NULL;
}

void* threadProdB(void* arg)
{
	for (int i = 0; i < 20; ++i)
	{
		buffer.produceB();
	}
	return NULL;
}

void* threadConsA(void* arg)
{
	for (int i = 0; i < 30; ++i)
	{
		auto value = buffer.consumeA();
	}
	return NULL;
}

void* threadConsB(void* arg)
{
	for (int i = 0; i < 30; ++i)
	{
		auto value = buffer.consumeB();
	}
	return NULL;
}

int main()
{
#ifdef _WIN32
	HANDLE tid[threadsCounts];
	DWORD id;

	// utworzenie w�tk�w
	tid[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadProdA, 0, 0, &id);
	tid[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadProdB, 0, 0, &id);
	tid[2] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadConsA, 0, 0, &id);
	tid[3] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadConsB, 0, 0, &id);

	// czekaj na zako�czenie w�tk�w
	for (int i = 0; i <= threadsCounts; i++)
		WaitForSingleObject(tid[i], INFINITE);
#else
	pthread_t tid[threadsCounts];

	// utworzenie w�tk�w
	pthread_create(&(tid[0]), NULL, threadProdA, NULL);
	pthread_create(&(tid[1]), NULL, threadProdB, NULL);
	pthread_create(&(tid[2]), NULL, threadConsA, NULL);
	pthread_create(&(tid[3]), NULL, threadConsB, NULL);

	//czekaj na zako�czenie w�tk�w
	for (int i = 0; i < threadsCounts; i++)
		pthread_join(tid[i], (void**)NULL);
#endif
	return 0;
}

