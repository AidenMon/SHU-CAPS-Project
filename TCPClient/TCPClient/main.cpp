#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <assert.h>
#include "TCPClient.h"
#include "barrier.hpp"

#define DEFAULT_PORT 12345
#define READERS_COUNT 5
#define WRITERS_COUNT 5
#define TOTAL_THREADS (READERS_COUNT + WRITERS_COUNT)



barrier simpleBarrier(TOTAL_THREADS);

void writer(char* argv, int thread) {
	int topic = thread*1000;
	int msg = 1;
	int sent [10]={0,0,0,0,0,0,0,0,0,0};
	int inc = 0;
	std::string request;
	TCPClient client(argv, DEFAULT_PORT);
	client.OpenConnection();

	auto now = std::chrono::steady_clock::now;
	using namespace std::chrono_literals;
	auto work_duration = 10s;
	auto prn_duration = 1s;
	auto start = now();
	auto sec = now();

	while ((now() - start) < work_duration) {
		if ((now() - sec) > prn_duration) {
			sec = now();
			//printf("Second %d: %d requests.\n", inc, sent[inc]);
			inc++;
		}
		request = "";
		if (msg % 1000 != 0) { request += "post@" + std::to_string(topic) + '#' + std::to_string(msg++); }
		else {
			topic++; request += "post@" + std::to_string(topic) + '#' + std::to_string(msg++);
		}

		//std::cout << "POST sent: " << request << std::endl;

		std::string reply = client.send(request);
		assert(inc < 10);
		sent[inc]++;

		//std::cout << "POST returned: " << reply << std::endl;	
	}
	int total=0;
	printf("Writer %d data:\n", thread);
	for (int x = 0; x < 10; x++) {
		printf("%d\tSecond %d: %d requests.\n", thread, x, sent[x]);
		total += sent[x];
	}
	printf("Reader %d averaged: %d\n", thread, total / 10);
	client.CloseConnection();
}

void reader(char* argv, int thread) {
	int topic = thread * 1000;
	int msg = 1;
	int sent[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int inc = 0;
	std::string request;
	TCPClient client(argv, DEFAULT_PORT);
	client.OpenConnection();

	auto now = std::chrono::steady_clock::now;
	using namespace std::chrono_literals;
	auto work_duration = 10s;
	auto prn_duration = 1s;
	auto start = now();
	auto sec = now();

	while ((now() - start) < work_duration) {
		if ((now() - sec) > prn_duration) {
			sec = now();
			//printf("Second %d: %d requests.\n", inc, sent[inc]);
			inc++;
		}
		request = "";
		if (msg % 1000 != 0) { request += "read@" + std::to_string(topic) + '#' + std::to_string(msg++); }
		else {
			topic++; request += "read@" + std::to_string(topic) + '#' + std::to_string(msg++);
		}

		//std::cout << "POST sent: " << request << std::endl;

		std::string reply = client.send(request);
		assert(inc < 10);
		sent[inc]++;

		//std::cout << "POST returned: " << reply << std::endl;	
	}

	printf("Reader %d data:\n", thread);
	int total = 0;
	for (int x = 0; x < 10; x++) { 
		printf("%d\tSecond %d: %d requests.\n", thread, x, sent[x]);
		total += sent[x];
	}
	printf("Reader %d averaged: %d\n", thread,total/10);

	client.CloseConnection();
}


void selfTest(char* argv) {
	std::string request;
	TCPClient client(argv, DEFAULT_PORT);
	client.OpenConnection();
	do {
		request = "";

		std::cout << "Enter to send: ";
		std::getline(std::cin, request);

		std::cout << "String sent: " << request << std::endl;
		std::cout << "Bytes sent: " << request.size() << std::endl;

		std::string reply = client.send(request);

		std::cout << "String returned: " << reply << std::endl;
		std::cout << "Bytes received: " << reply.size() << std::endl;
	} while (request != "stop");
	client.CloseConnection();
}

int main(int argc, char **argv)
{
	/*/ Validate the parameters
	if (argc != 2) {
		printf("usage: %s server-name|IP-address\n", argv[0]);
		return 1;
	}
	//*/

	if (argc != 5) {
		printf("Usage: %s server_IP num_posters num_readers time\nserver_IP - IP of the server\nnum_posters - Number of Poster Threads\nnum_readers - Number of Reader Threads\ntime - requirered runtime of the client", argv[0]);
		return 1;
	}

	//selfTest(argv[1]);

	std::vector<std::thread> readers;
	std::vector<std::thread> writers;
	
	std::string strW = argv[2];
	std::string strR = argv[3];

	int numW = std::stoi(strW);
	int numR = std::stoi(strR);
	int i;

	printf("Writers running for %s seconds each\n",argv[4]);
	for (i = 0; i < numW; i++)
		writers.emplace_back(writer, argv[1], i);

	
	//*/
	printf("Readers running for %s seconds each\n", argv[4]);
	for (i = 0; i < numR; i++)
		readers.emplace_back(reader, argv[1], i);

	for (auto& writer : writers)
		writer.join();

	for (auto& reader : readers)
		reader.join();
	//*/

	//interwind

	
	
	//std::cout << "Enter to close: ";
	//std::cin.ignore();

	return 0;
}
