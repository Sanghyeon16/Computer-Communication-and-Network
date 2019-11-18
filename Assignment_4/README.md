Simple HTTP Proxy
===============

Simple HTTP Proxy
ECEN 602 Network Programming Assignment 4


Role
===============
Mainly, we worked together, but here is the role what we did.
Minhwan Oh : Developed server programming, tested for integration
Sanghyeon Lee : Developed server programming, debugged for integration 


File info
===============
For this program, you can see how HTTP Proxy works. 
1. proxy.cpp
Main feature: Server application for proxy server.
2. client.cpp
Main feature: proxy client
3. utility.h
Main feature: header file
4. Makefile
Main feature: Compile setting description

Build info
===============
Command $make

Program scenario
===============
1. Start the server first with the commanding line: $./proxy <ip to bind> <port to bind>, where “proxy” is the name of the server program,  Port is the port number on which the server is listening.

2. Start client with the commanding line: $./client <proxy address> <proxy port> <URL to retrieve>

3. There are several test cases as below, and we comply with all test cases. 
(1) A cache hit returns the saved data to the requester.
(2) A request that is not in the cache is proxied, saved in the cache, and returned to the requester.
(3) A cache miss with 10 items already in the cache is proxied, saved in the LRU location in cache, and the data is returned to the requester.
(4) A stale Expires header in the cache is accessed, the cache entry is replaced with a fresh copy, and the fresh data is delivered to the requester.
(5) A stale entry in the cache without an Expires header is determined based on the last Web server access time and last modification time, the stale cache entry is replaced with fresh data, and the fresh data is delivered to the requester.
(6) A cache entry without an Expires header that has been previously accessed from the Web server in the last 24 hours and was last modified more than one month ago is returned to the requester.
(7) Three clients can simultaneously access the proxy server and get the correct data (you will need to access large documents to show this).
(8) If you implement the Bonus feature, develop test cases to show that it works properly; you should modify (5) ? (6) above to match the operation of the Bonus

Program details & File architecture 
===============
1. Proxy Server Design
- Server will start on a user specified port and listen for incoming HTTP requests.
- Proxy server checks the received client’s request in its cache.
- If the request is new(i.e. no valid cache entry), the request will be proxied to the intended destination, the response will be sent by the proxy to the client, and be cached by the proxy for later use.
- Proxy cache can maintain at least 10 document entries in the cache.
- Entries followed the Least Recently Used (LRU) fashion.

2. Proxy command client
- Client will take input from the command line as to the web proxy address and port as well as the URL to retrieve.
- The client will store the retrieved document in the directory where the client is executed.

3. Fresh check (Bonus Feature)
- to determine if a cache page is “fresh” is to use the date in the Expires, Last-Modified, or Date header in conjunction with an If-Modified-Since message header line in a GET request, which is known as a Conditional Get, to ask the Web server if the page has changed.

File Function Explanation
===============
1. proxy server
a) Architecture 
	- Scenario : 
		(1) Setup server_info/bind port/listen functions
		(2) Then it is going to infinite loop until the exit
	- Main function : 
		(1) int main (int argc, char* argv[]) : Including main scenario
		(2) struct  http * parse_http_request (char * burf, int bytes_num)  : Decoding the host name and the file name by parsing through the HTTP request
		(3) int checkCache (string  req) :  Check for the existing request in the cache
		(4) bool calc_expire_bool (int  cacheBlock_index) : Check for the stale cache




Test cases
===============
Current program passed below test cases
(1) A cache hit returns the saved data to the requester.
(2) A request that is not in the cache is proxied, saved in the cache, and returned to the requester.
(3) A cache miss with 10 items already in the cache is proxied, saved in the LRU location in cache, and the data is returned to the requester.
(4) A stale Expires header in the cache is accessed, the cache entry is replaced with a fresh copy, and the fresh data is delivered to the requester.
(5) A stale entry in the cache without an Expires header is determined based on the last Web server access time and last modification time, the stale cache entry is replaced with fresh data, and the fresh data is delivered to the requester.
(6) A cache entry without an Expires header that has been previously accessed from the Web server in the last 24 hours and was last modified more than one month ago is returned to the requester.
(7) Three clients can simultaneously access the proxy server and get the correct data (you will need to access large documents to show this).
(8) If you implement the Bonus feature, develop test cases to show that it works properly; you should modify (5) ? (6) above to match the operation of the Bonus



Reference
===============
Below functions are referred by network programming library, W. Richard Stevens, Bill Fenner, and Andrew M. Rudoff, Unix Network
Programming, Volume 1, The Sockets Networking API, 3rd Edition

https://tools.ietf.org/html/rfc1945 (Hypertext Transfer Protocol -- HTTP/1.0)


