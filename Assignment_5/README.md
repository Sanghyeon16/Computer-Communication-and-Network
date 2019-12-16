5th  Group Homework Report
NS-2 Familiarization
ECEN 602 Network Simulation Assignment 1
===============

Mainly, we worked together, but here is the role what we did.
Minhwan Oh : Developed client programming, tested for integration
Sanghyeon Lee : Developed server programming, debugged for integration 

File info
===============
For this program, you can see how NS-2 simulator works 
1. ns2.tcl
Main feature: NS-2 simulation source code

Build info
===============
NS2 install reference: https://www.youtube.com/watch?v=nIdWtN_PwkM
Command line: $ns ns2.tcl <TCP flavor> <Case number>

Simulation scenario
===============

1. Using NS-2 simulator, build the following configuration
- Two routers (R1, R2) connected with a 1Mbps link and 5ms of latency
- Two senders (src1, src2) connected to R1 with 10 Mbps links
- Two receivers (rcv1, rcv2) connected to R2 with 10 Mbps links
- Application sender is FTP over TCP

2. Run 400s simulations for the following variable parameters
- TCP version = (TCP SACK | TCP VEGAS)
- Case 1:
	: src1-R1 and R2-rcv1 end-2-end delay = 5 ms
: src2-R1 and R2-rcv2 end-2-end delay = 12.5 ms
- Case 2:
	: src1-R1 and R2-rcv1 end-2-end delay = 5 ms
: src2-R1 and R2-rcv2 end-2-end delay = 20 ms
- Case 3:
	: src1-R1 and R2-rcv1 end-2-end delay = 5 ms
: src2-R1 and R2-rcv2 end-2-end delay = 27.5 ms

Note that in the cases above the end-to-end RTTs of the two sources are in the ratio 1:2, 1:3 and 1:4, respectively

3. Give the answers to the below requests
(i)	For each of the TCP flavors (VEGAS and SACK) simulate the three RTT cases and find the ratio of the average throughput of src1 to src2. Make two separate tables (one for each TCP flavor) showing the throughput for each test case.
(ii)	Discuss the relationship between TCP throughput and RTT in light of your results in (i) above for each TCP flavor. Also compare and discuss the throughput performance of the two flavors of TCP for Case 1. Explain why one of the TCP flavors performs better than the other for Case 1. There is information about TCP Vegas in our textbook, pp. 523-530. RFC 6675 describes the current recommended TCP SACK operation, which is basically TCP RENO + SACK. I am not sure if ns-2 implements RFC 6675 or the earlier RFC 3517 that was made obsolete by RFC 6675, however.

Note: Run the simulations for 400 seconds, and ignore the first 100 seconds while measuring metrics.

Program details & File architecture 
===============
1. Simulation design

-	Using OTcl and C++ script, describe the TCP network
-	After 100s from beginning, it measures the RTT and check its average throughput to see their ratio.
-	In simulation code, we made new simulator, and made trace value for visualization through simulator.
-	Then, we designed the network satisfying above conditions by defining noes, links, and queuing.
-	We can make VEGAS and SACK TCP versions by setting ns options via OTcl code.
-	We set senders and receivers color depend on its case options; case 1: is red, case 2 is green, and case 3 is blue.
-	out.nam stores the traces so that we can visually see it on NS simulator.


File Function Explanation
===============
1. ns2.tcl
a) Architecture 
	- Scenario : 
		(1) Setup TCP networks
		(2) Track the outputs to visualize it on NS simulator

Test Case
===============
(i)	For each of the TCP flavors (VEGAS and SACK) simulate the three RTT cases and find the ratio of the average throughput of src1 to src2. Make two separate tables (one for each TCP flavor) showing the throughput for each test case.
(ii)	Discuss the relationship between TCP throughput and RTT in light of your results in (i) above for each TCP flavor. Also compare and discuss the throughput performance of the two flavors of TCP for Case 1. Explain why one of the TCP flavors performs better than the other for Case 1. There is information about TCP Vegas in our textbook, pp. 523-530. RFC 6675 describes the current recommended TCP SACK operation, which is basically TCP RENO + SACK. I am not sure if ns-2 implements RFC 6675 or the earlier RFC 3517 that was made obsolete by RFC 6675, however.

The code is included in Team_5.pdf report.
