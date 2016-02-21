Individual Assignment

James Oliver
1279196




Files

buffer.h
buffer.c
readme.txt



Compiling the program

enther the following code into your terminal:
		
		gcc -g -o buffer buffer.c -pthread

To run the code you will need to type in ./buffer then sleep time producer threads and then consumer threads: 
For Example

		./buffer 20 1 1




Test Run

Just decided to test run a simple example to help me better understand so i used (./buffer 20 1 1)
the resulted in:

	producer produced 1681692777
	consumer consumed 1681692777

Analysis:

My interpretation of this result is that consumer will not begin until producer has produced something
this is because it cant consume when there is nothing in the buffer so it will just sleep until the producer
produces something and it wakes to consume it. We used the semaphores so that we could run the producer function
without the consumer function being active until it has something to produce.



