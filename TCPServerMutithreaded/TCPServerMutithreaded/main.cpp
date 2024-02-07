#include <iostream>
#include <algorithm>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <shared_mutex>

#include "config.h"
#include "TCPServer.h"
#include "TCPClient.h"

//my includes 
#include <map>

#define DEFAULT_PORT 12345

//my data
std::shared_mutex mapMutex;
std::map<std::string, std::vector<std::string>> msgs;
//this is where you're going to need to add locks ^


bool terminateServer = false;

void serverThreadFunction(TCPServer* server, ReceivedSocketData && data);

//my functions

int main()
{
	

	//-----------------

	TCPServer server(DEFAULT_PORT);

	ReceivedSocketData receivedData;

	std::vector<std::thread> serverThreads;

	std::cout << "Starting server. Send \"exit\" (without quotes) to terminate." << std::endl;

	while (!terminateServer)
	{
		receivedData = server.accept();

		if (!terminateServer)
		{
			serverThreads.emplace_back(serverThreadFunction, &server, receivedData);
		}
	}

	for (auto& th : serverThreads)
		th.join();

	std::cout << "Server terminated." << std::endl;

	return 0;
}



//https://stackoverflow.com/questions/5324139/fastest-data-structure-for-retrieval-info-c
//this is why I'm using stl::map



void serverThreadFunction(TCPServer* server, ReceivedSocketData && data)
{
	unsigned int socketIndex = (unsigned int) data.ClientSocket;

	do {
		server->receiveData(data, 1);

		if (data.request != "" && data.request != "exit" && data.request != "EXIT")
		{
			//std::cout << "[" << socketIndex << "] Bytes received: " << data.request.size() << std::endl;
			//std::cout << "[" << socketIndex << "] Data received: " << data.request << std::endl;
			
			if ( (data.request.substr(0, 5) == "post@" || data.request.substr(0, 5) == "POST@")) {	
				std::string topicId = data.request.substr(4, data.request.find('#'));
				std::string rest = data.request.substr(data.request.find('#')+1);
				//std::cout << "message?: " << rest << "\n";
				topicId = (topicId.length() < 140) ? topicId : topicId.substr(0, 140);
				rest = (rest.length() < 140) ? rest : rest.substr(0, 140);
				mapMutex.lock();
				if (msgs.end() == msgs.find(topicId)) {
					std::vector<std::string>s;
					s.emplace_back(rest);
					msgs.insert({ topicId, s });
					data.reply = "0";
				}
				else {
					msgs[topicId].emplace_back(rest);
					//std::cout << msgs[topicId].size() << "  " << msgs.at(topicId).size();
					data.reply = std::to_string(msgs.at(topicId).size() - 1);
				}
				mapMutex.unlock();

			}//data.request.find('#') != std::string::npos &&
			else if ( (data.request.substr(0, 5) == "read@" || data.request.substr(0, 5) == "READ@")) {
				std::string rest = data.request.substr(4);
				std::string topicId = rest.substr(0, rest.find('#'));
				int read = std::stoi(data.request.substr(data.request.find('#') + 1));
				//std::cout << topicId << "  " << read;

				topicId = (topicId.length() < 140) ? topicId : topicId.substr(0, 140);
				mapMutex.lock_shared();
				if (msgs.end() == msgs.find(topicId) || read>msgs.at(topicId).size()-1) {
					data.reply = "";
				}
				else {
					//std::cout << msgs.at(topicId)[read];
					data.reply = msgs.at(topicId)[read];
				}
				mapMutex.unlock_shared();
			}
			else if (data.request.substr(0, 4) == "list" || data.request.substr(0, 4) == "LIST") {
				std::string out;
				for (auto it = msgs.begin(); it != msgs.end(); it++) {
					out += it->first+"#";
				}
				
				data.reply = out.substr(0,out.size()-1);
			}
			else if (data.request.substr(0, 6) == "count@" || data.request.substr(0, 6) == "COUNT@") {
				std::string topicId = data.request.substr(5);
				
				topicId = (topicId.length() < 140) ? topicId : topicId.substr(0, 140);

				if (msgs.find(topicId)!=msgs.end()) {
					data.reply = std::to_string(msgs.at(topicId).size()); 
				}
				else { data.reply = "0"; }
			}
			else {
				data.reply = "";
			}
			// data.reply = data.request;
			

			server->sendReply(data);
		}
		else if (data.request == "exit" || data.request == "EXIT")
		{
			std::cout << "[" << socketIndex << "] Data received: " << data.request << std::endl;
			std::cout << "[" << socketIndex << "] Exiting... Bye bye!" << std::endl;

			data.reply = data.request;
			server->sendReply(data);
		}
		else { data.reply = ""; }
	} while (data.request != "exit" && data.request != "EXIT" && !terminateServer);

	if (!terminateServer && (data.request == "exit" || data.request == "EXIT"))
	{
		terminateServer = true;

		TCPClient tempClient(std::string("127.0.0.1"), DEFAULT_PORT);
		tempClient.OpenConnection();
		tempClient.CloseConnection();
	}

	server->closeClientSocket(data);
}