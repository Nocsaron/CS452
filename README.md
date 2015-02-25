# CS452
Operating System Created as project for CS452

Noah Sleiman
Nicholas Callahan

Test1 and Test3 functionality confirmed. Basic handler and semaphore structure in place, but not full functionality as of now. 

The OS works through a static array, the index of each matches up with a Process ID. Each block contains a PCB object which contains all the necessary information for the process. Rather than manipulate all information on each process directly (loading a new PCB with each dispatch), we use pointers to reference one area of memory. In this way, our operating system uses less space. 

As if to compensate for this, we liberally use lists in our implementation. Each parent keeps track of a list of their children, although each node in the list maintains only the PID and priority of each (the PCB array can be accessed with the PID, and the priority allows us to implement a priority queue). Each PCB block also contains a semaphore, and each semaphore maintains a list of processes that are currently waiting on it. The interactions between the ready list and the semaphores queue are pretty much verbatim as described in the slides, although not functional in real testing. 

To handle the priority sorting, we opted to leave very little in the dispatcher's hands, instead relying on our list's methods themselves to handle the queuing for us. As such, when we invoke a replace-the-process-into-ready-list method "insert", it will automatically find the spot between the last member >= its own priority, and insert itself there. Thus, all the dispatcher need do is pop its first member of the queue, and work with that process. To avoid infinite looping, the ready list has no knowledge of the running process, and selects the next process before replacing the currently running one. 

