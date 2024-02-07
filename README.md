# Concurrent and Parallel Systems - A Sheffield Hallam University Project
This is a project that I was assigned to complete for my university in which we were to develop our own multithreaded server and a client/"Test Harness" utilizing C++ language.
We were provided the means to connect the two servers via TCP communications but were not given any code for handeling individual clients connecting to a server.

>Your task is to implement a C++ server for a simple discussion forum geared towards high traffic and short messages. The server should allow multiple clients to connect concurrently over the network, and to post and read messages on multiple topics.
You must test your server in high-load scenarios and evaluate its throughput, depending on the number of connected clients and the nature of their interaction.\

>To support your development efforts, on the blackboard you can find:
> - A C++ parser for the request format, with a “main” function showcasing how to use it.
> - TCP server and client libraries to provide you with basic network communication facilities between the server and the client. Each Visual C++ solution implements a “main” function. If they are run on two different command lines (first the server, then the client), they are configured so that the client asks the user for a string to be sent to the server. The server reverts the string and sends it back to the client.
> - A protocol verifier, which connects to your TCP server and verifies that the communication follows the required protocol.
> - A reference implementation, which will provide the basis for the comparison of the speed of your implementation.\

**The reference implementation was nothing more than two EXE files and no code whatsoever**

### —Contents of Archive—
```
|- TCPServerMutithreaded - The overall Server Folder
|  |-TCPServerMutithreaded - The VS project folder and all files
|  |-TCPServerMutithreaded.exe - The Server executable
|  |-TCPServerMutithreaded.sln - The VS Solution
|
|- TCPClient - The overall test harness / client folder
|  |-TCPClient - The VS project folder
|  |-TCPClient.exe - The client executable
|  |-TCPClient.sln - The VS Solution
|
|- MoncavageCAPSReport - The throughput experiment report
|
|- MoncavageTestingData - The spreadsheet of all my tests when running my server to
| 				Sergio’s reference client and the accompanying graphs.
|
|- TestHarnessResults
|  |-My1 - Screenshot of testing my test harness and my server
|  |-My2 - Screenshot of testing my test harness and my server
|  |-My3 - Screenshot of testing my test harness and my server
|  |-1 - Screenshot of testing my test harness and Sergio’s server
|  |-2 - Screenshot of testing my test harness and Sergio’s server
|  |-3 - Screenshot of testing my test harness and Sergio’s server
```

### —How to run Server—
1. Open the “TCPServerMultithreaded” folder
2. To run either:
Double click TCPServerMultithreaded.exe
Open a command prompt at the folder and type in “TCPServerMutithreaded.exe” 

### —How to run Test Harness / Client—
1. Open the “TCPClient” folder
2. Open a command prompt at folder and enter the following:
TCPClient.exe ip X Y Z
TCPClient.exe 127.0.0.1 5 5 10
  Where
TCPClient.exe - is the name of the executable
ip - is the IP address of the server (127.0.0.1 assuming local machine)
X - is the number of Writers / Poster thread
Y - is the number of Readers / Reader threads
Z - is the runtime of the test harness

**If 5 parameters are not sent to the client the client will output the proper format to call it just in case**
