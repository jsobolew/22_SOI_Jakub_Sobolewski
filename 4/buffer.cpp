#include <iostream>
#include <vector>
#include <string>
#include "monitor.h"

int const threadsCounts = 4;

int const bufferSize = 9;


class Buffer : Monitor
{
private:

	Condition empty, full, frontA, frontB;

	std::vector<std::string> values;

	void print(std::string name)
	{
		std::cout << "\n ###" << name << " " << values.size() << "[";
		for (auto v : values)
			std::cout << v << ",";
		std::cout << "] ###\n";
	}

public:
	Buffer() = default;

	void produceA()
	{
		enter();
		if (values.size() == bufferSize)
		{
			wait(full);
		}
		values.push_back("A");
		print("produced A");

		if (values.size() > 3)
		{
			if(values.front() == "A")
			{
				signal(frontA);
			}
			else
			{
				signal(frontB);
			}
		}
		leave();
	}

	void produceB()
	{
		enter();
		if (values.size() == bufferSize)
		{
			wait(full);
		}
		values.push_back("B");
		print("produced B");

		if (values.size() > 3)
		{
			if(values.front() == "A")
			{
				signal(frontA);
			}
			else
			{
				signal(frontB);
			}
		}
		leave();
	}

	std::string consumeA()
	{
		enter();

		if (values.size() < 3 || values.front() != "A")
		{
			wait(frontA);
		}

		auto value = values.front();
		values.erase(values.begin());
		print("consumed A");
		signal(full);

		std::string front = values.front();
		if (values.size() > 3)
		{
			if(front == "B")
			{
				signal(frontB);
			}
			// else
			// {
			// 	signal(frontA);
			// }
		}

		leave();
		return value;
	}

	std::string consumeB()
	{
		enter();
		if (values.size() < 3 || values.front() != "B")
		{
			wait(frontB);
		}

		auto value = values.front();
		values.erase(values.begin());
		print("consumed A");
		signal(full);

		std::string front = values.front();
		if (values.size() > 3)
		{
			if(front == "A")
			{
				signal(frontA);
			}
			// else
			// {
			// 	signal(frontB);
			// }
		}

		leave();
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
	// tid[4] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadConsA, 0, 0, &id);
	// tid[5] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadConsB, 0, 0, &id);

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