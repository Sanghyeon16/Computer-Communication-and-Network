Trivial File Transfer Protocol (TFTP) Server
===============

Trivial File Transfer Protocol (TFTP)
ECEN 602 Network Programming Assignment 2


Role
===============
Mainly, we worked together, but here is the role what we did.
Minhwan Oh : Developed server programming, tested for integration
Sanghyeon Lee : Developed server programming, debugged for integration 


File info
===============
For this program, you can see how trivial File Transfer Protocol (TFTP) server. There is one main file. 
1. tftp_server.c
Path: cd Assignment_3/tftp_server.c
Main feature: Server application for TFTP server.
2. makefile
Main feature: Compile setting description

Build info
===============
Command $make

Program scenario
===============
This program includes only server for Trivial File Transfer Protocol (TFTP) because we are going to use a standard TFTP client, which does the following:

1. Start the server first with the commanding lien: $tftp_server <Port> <Repository>, where “tftp_server” is the name of the server program,  Port is the port number on which the tftp server is listening, and repository is the folder that server can store or transfer files.

2. Assume that TFTP client is installed in linux environment, after server programming runs, tftp client also needs to run. Since we put port number as 8000 in server program, client also needs to set same port name. To test properly, please go cd Assignment_3/Client then run TFTP client

3. Current our TFTP program can transfer files in both directions because we implemented Read Request Function (RRQ) and Write Request Function (WRQ) as well. The server allows retrieval of files in the assigned local directory.

4. There are several test cases as below, and we comply with all test cases. 
(1) Transfer a binary file of 2048 bytes and check that it matches the source file
(2) Transfer a binary file of 2047 bytes and check that it matches the source file
(3) Transfer a netascii file that includes two CR’s and check that the resulting file matches the input file
(4) Transfer a binary file of 34 MB and see if block number wrap-around works
(5) check that you receive an error message if you try to transfer a file that does not exist and that your server cleans up and the child process exits
(6) Connect to the TFTP server with three clients simultaneously and test that the transfers work correctly (you will probably need a big file to have them all running at the same time)
(7) Terminate the TFTP client in the middle of a transfer and see if your TFTP server recognizes after 10 timeouts that the client is no longer there (you will need a big file)
(8) Transfer a binary file of 2048 bytes to server and check that it matches the source file ( Bonus Feature )
(9) Transfer a binary file of 2047 bytes to server and check that it matches the source file ( Bonus Feature )
(10) Connect to the TFTP server with three clients simultaneously and test that the transfers work correctly – transmitting file to server simultaneously ( Bonus Feature )
(11) Terminate the TFTP server in the middle of a transfer and see if your TFTP client recognizes after 10 timeouts that the client is no longer there ( Bonus Feature )


Program details & File architecture 
===============
1. User Datagram Protocol (UDP) Programming
- Recall that UDP is aconnectionless, unreliable, datagram protocol. In contrast, TCP is connection- oriented and provides a reliable byte stream.
- The UDP server does not issue a listen() or an accept(). Instead, the UDP server just calls the recvfrom() function, which blocks until data arrives from some client. The recvfrom() function returns the address of the client (IP address and port number), as well as the datagram, so the server can respond to the client.

2. Multiple simultaneous connections
- To support multiple simultaneous connections, used fork() function in server which a child process to handle each client transfer, similar to TCP Echo Server.

3. Timeout
- The sender of a TFTP DATA packet (the TFTP Server in the case of a RRQ request or the TFTP Client in the case of a WRQ request) needs to (1) send a DATA packet, (2) set a timeout timer, and (3) wait for an ACK. If the ACK arrives before the timeout timer expires, the timer is cancelled and you then process the next packet. If the timeout timer expires, you must retransmit the last DATA packet.
- We implemented timeout in select() function : Call select() to wait for either of the following two events: (1) data ready for reading on the socket descriptor created by the child server process and(2) a timeout of say one second from when select() is called.

4. File transmission
- For octet transmission, we used the standard Unix/Linux open(), read() (RRQ), and write() (Bonus WRQ) system calls.
- For netascii transmission, however, we used the Unix/Linux fopen(), getc() and putc() system calls.

File Function Explanation
===============
1. tftp_server
a) Architecture 
	- Scenario : 
		(1) Setup server_info/bind port/broadcast/listen functions
		(2) Then it is going to infinite loop until the exit
	- Main function : 
		(1) int main (int argc, char** argv) : Including main scenario
		(2) void ErrSend (int errNum, struct sockaddr_in client_Addr)  : Send an error message
		(3) int DataSend (char* fileDir, char* mode_Str, struct sockaddr_in client_Addr) : Send the file to client
		(4) int recvData (char* fileDir, char* mode_Str, struct sockaddr_in client_Addr) : Receive file from client

Test cases
===============
Current program passed below test cases
(1) Transfer a binary file of 2048 bytes and check that it matches the source file 
(2) Transfer a binary file of 2047 bytes and check that it matches the source file
(3) Transfer a netascii file that includes two CR’s and check that the resulting file matches the input file
(4) Transfer a binary file of 34 MB and see if block number wrap-around works
(5) check that you receive an error message if you try to transfer a file that does not exist and that your server cleans up and the child process exits
(6) Connect to the TFTP server with three clients simultaneously and test that the transfers work correctly (you will probably need a big file to have them all running at the same time)
(7) Terminate the TFTP client in the middle of a transfer and see if your TFTP server recognizes after 10 timeouts that the client is no longer there (you will need a big file)
(8) Transfer a binary file of 2048 bytes to server and check that it matches the source file ( Bonus Feature )
(9) Transfer a binary file of 2047 bytes to server and check that it matches the source file ( Bonus Feature )
(10) Connect to the TFTP server with three clients simultaneously and test that the transfers work correctly – transmitting file to server simultaneously ( Bonus Feature )
(11) Terminate the TFTP server in the middle of a transfer and see if your TFTP client recognizes after 10 timeouts that the client is no longer there ( Bonus Feature )




Reference
===============
Below functions are referred by network programming library, W. Richard Stevens, Bill Fenner, and Andrew M. Rudoff, Unix Network
Programming, Volume 1, The Sockets Networking API, 3rd Edition


