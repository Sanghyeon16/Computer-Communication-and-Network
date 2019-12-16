#New simulator Object
set ns [new Simulator]


#Check Input Argument
if {$argc!=2} {
	puts "[System] : Argument Setting Error"
	puts "		 Set Argument : ns ns2.tcl <TCP_flavor> <case_no>"
	puts "		 Example: ns ns2.tcl VEGAS 2"
	exit 0
}

#Set Output
set tf [open out.tr w]
set output1 [open src1_output.tr w]
set output2 [open src2_output.tr w]
set output3 [open ratio_output.tr w]

set nf [open out.nam w]

#Enable Trace
$ns trace-all $tf
$ns namtrace-all $nf

#Initialize variables
set sum1 0
set sum2 0
set counter 0

#Calculate Throughput
proc calculate {} {

	#Variables
	global ns sink1 sink2 output1 output2 output3 sum1 sum2 counter

	set bw0 [$sink1 set bytes_]
	set bw1 [$sink2 set bytes_]


	#Step time
	set time 0.5

	#Current time
	set now [$ns now]


	#Ignore the first 100 seconds
	if {$now == 100} {
		$sink1 set bytes_ 0
		$sink2 set bytes_ 0
	}

	#Measure Throughput from 100 to 400 seconds
	if {$now > 100 && $now<= 400 } {
		set throughput1 [expr $bw0/$time *8/1000000]
		set throughput2 [expr $bw1/$time *8/1000000]
		set ratio [expr $throughput1/$throughput2]

		set sum1 [expr $sum1 + $throughput1]
		set sum2 [expr $sum2 + $throughput2]
		set counter [expr $counter + 1]

		#Store output values
		puts $output1 "$now $throughput1"
		puts $output2 "$now $throughput2"
		puts $output3 "$ratio"

		$sink1 set bytes_ 0
		$sink2 set bytes_ 0
	}

	if { $now == 400.5 } {
		set avgthroughput1 [ expr $sum1/$counter]
		set avgthroughput2 [ expr $sum2/$counter]
		set ratio [expr $avgthroughput1/$avgthroughput2]
		puts "The ratio of  the average throughput of src1 to src2 "
		puts "		$avgthroughput1 : $avgthroughput2 = 1 : $ratio"	
	}	

	#Recursion call
	$ns at [expr $now + $time] "calculate"
}



#Set nodes
set R1 [$ns node]
set R2 [$ns node]
set src1 [$ns node]
set src2 [$ns node]
set rcv1 [$ns node]
set rcv2 [$ns node]

#Set Drop Tail
$ns duplex-link $R1 $R2 1Mb 5ms DropTail

#Select TCP flavor
#VEGAS
if {[lindex $argv 0] eq "VEGAS"} {
	puts "TCP flavor : VEGAS"
	set tcp1 [new Agent/TCP/Vegas]
	set tcp2 [new Agent/TCP/Vegas]
}
#SACK
if {[lindex $argv 0] eq "SACK"} {
	puts "TCP flavor : SACK"
	set tcp1 [new Agent/TCP/Sack1]
	set tcp2 [new Agent/TCP/Sack1]
}

#Select CASE
#Case 1 : Red
if {[lindex $argv 1] eq "1"} { 
	puts "Case 1:"
	puts "		- src1-R1 and R2-rcv1 end-2-end delay = 5 ms"
	puts "		- src2-R1 and R2-rcv2 end-2-end delay = 12.5 ms"
	puts "		- RTT ratio 1:2"
	$rcv1 color Red
	$rcv2 color Red
	$src1 color Red
	$src2 color Red

	$ns duplex-link $src1 $R1 10Mb 5ms DropTail
	$ns duplex-link $R2 $rcv1 10Mb 5ms DropTail
	
	$ns duplex-link $src2 $R1 10Mb 12.5ms DropTail
	$ns duplex-link $R2 $rcv2 10Mb 12.5ms DropTail

}

#Case 2 : Green
if {[lindex $argv 1] eq "2"} { 
	puts "Case 2:"
	puts "		- src1-R1 and R2-rcv1 end-2-end delay = 5 ms"
	puts "		- src2-R1 and R2-rcv2 end-2-end delay = 20 ms"
	puts "		- RTT ratio 1:3"

	$rcv1 color Green
	$rcv2 color Green
	$src1 color Green
	$src2 color Green

	$ns duplex-link $src1 $R1 10Mb 5ms DropTail
	$ns duplex-link $R2 $rcv1 10Mb 5ms DropTail
	
	$ns duplex-link $src2 $R1 10Mb 20ms DropTail
	$ns duplex-link $R2 $rcv2 10Mb 20ms DropTail

}

#Case 3 : Blue
if {[lindex $argv 1] eq "3"} { 
	puts "Case 3:"
	puts "		- src1-R1 and R2-rcv1 end-2-end delay = 5 ms"
	puts "		- src2-R1 and R2-rcv2 end-2-end delay = 27.5 ms"
	puts "		- RTT ratio 1:4"

	$rcv1 color Blue
	$rcv2 color Blue
	$src1 color Blue
	$src2 color Blue
	
	$ns duplex-link $src1 $R1 10Mb 5ms DropTail
	$ns duplex-link $R2 $rcv1 10Mb 5ms DropTail
	
	$ns duplex-link $src2 $R1 10Mb 27.5ms DropTail
	$ns duplex-link $R2 $rcv2 10Mb 27.5ms DropTail
}



#TCP Sinks initialization
set sink1 [new Agent/TCPSink]
set sink2 [new Agent/TCPSink]

#Attach
$ns attach-agent $src1 $tcp1
$ns attach-agent $src2 $tcp2
$ns attach-agent $rcv1 $sink1
$ns attach-agent $rcv2 $sink2

#Connect
$ns connect $tcp1 $sink1
$ns connect $tcp2 $sink2

#Application sender is FTP over TCP
set ftp1 [new Application/FTP]
set ftp2 [new Application/FTP]
$ftp1 attach-agent $tcp1
$ftp2 attach-agent $tcp2


#Structure
$R1 shape square
$R2 shape square
$R1 color black
$R2 color black

$ns duplex-link-op $R1 $R2 orient right
$ns duplex-link-op $src1 $R1 orient 320deg
$ns duplex-link-op $src2 $R1 orient 40deg

$ns duplex-link-op $rcv1 $R2 orient 220deg
$ns duplex-link-op $rcv2 $R2 orient 140deg

#Close simulation
proc Complete {} {

	global ns nf tf
	$ns flush-trace

	close $nf
	close $tf

	#exec <nam folder> out.nam &
	exit 0
}

#MAIN----------------------------------------
#Start
$ns at 0.5 "$ftp1 start"
$ns at 0.5 "$ftp2 start"

#Measurement
$ns at 100 "calculate"

$ns at 401 "$ftp1 stop"
$ns at 401 "$ftp2 stop"

#Close
$ns at 405 "Complete"
$ns run



