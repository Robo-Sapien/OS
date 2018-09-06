DETAILS:


1) Linux kernel used: linux-4.10.13

2) new system call id of "myfork": 332

3) new system call's C code file name: myfork.c

4) C program to test the added system call: test.c

5) test.c uses "customfork()" function that has the same functionality as the "fork()" system call.

6) after running test.c, we can see PID of child being returned to parent and 0 being returned to child. (which happens in fork() as well)

7) run dmesg to see our custom kernel message "custom forking done", indicating that forking is successfull.




STEPS TO COMPILE AND RUN test.c


1) make sure that test.c, customlib.c, customlib.h are in the same directory

2) gcc -o customlib.o -c customlib.c

3) gcc test.c customlib.o






NOTE: You can also check by changing the test.c such that both parent and child processes sleep for x seconds and running "ps -ef" would 
reveal both processes running along with their PIDs and PPIDs


