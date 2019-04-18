---
COLIN CASAZZA
CS570 - OPERATING SYSTEMS
CSSC1070
ASSIGNMENT FOUR
4/16/19

Single Programmer Affidavit

I the undersigned promise that the attached assignment is my own work. While I was free to discuss ideas with others, the work contained is my own. I recognize that should this not be the case, I will be subject to penalties as outlined in the course syllabus. 
---
#### Question 1

Suppose proposed critical section entry/exit routines are defined as    follows using a shared variable locked that is initialized to zero (unlocked):

```
entry(int *lock) {
    // assume TSL generates an assembler
    // test and set lock instruction
    while (TSL(lock));
}

exit(int *lock) {
    *lock = 0; // reset lock
}
```
Does this implement a critical section? Justify your answer.

Answer: No, there is a problem with this implementation. If "lock" is instantiated to zero any process using lock will go to entry function, break out of the while loop and enter the critical section without the "lock" variable being changed from 0 to one. 

In this case any other thread could enter it's own critical section breaking the rule of multual exclusion. I.e. this implementation is wrong.

#### Question 2

A robot relay race requires 3 robots to run 1/3 of a track each. The first robot starts when a sensor indicates that a gun has fired. The second robot is not allowed to start until the first robot has reached the
1/3 point of the track, and the third is not allowed to start until the second has reached the 2/3 point. Using Dijkstra’s counting semaphores, write pseudocode for each robot to implement barriers that would allow the race to be completed without any false starts. Be sure to initialize your semaphores to an appropriate value. Use the following functions:

    WaitForGun() – A function that returns once the gun has been fired (no need to use a barrier here). 

    FollowTrack() – Function that moves the robot one meter along the track. It returns True if the robot has reached the finish with respect to its 1/3 of the track, or False if there is more track to run.
    Robot1(), Robot2(), Robot3() or Robot(idx) – Provide functions for robots 1, 2, and 3 either separately or as a common function with the robot number passed in as idx. 
    
Semaphores can be initialized to an integer value and have only two functions that can be used on them: up() and down().

Answer:
Initialize the semaphore for robot one to one, so that it's the only robot that can travel when the gun fires. 

Robot(i).WaitForGun() -> this function calls up for robot_semaphores[i] while the race is active

```
i = 0;
robot_semaphores[3] = [1, 0, 0];
// triggers up call for Robot(i)
Robot(i).WaitForGun(); 
down(robot_semaphores[i]);
while(!FollowTrack())
{
    if(i<2) Robot(i+1);
}
```

#### Question 3

Write pseudocode to implement a remote procedure call (RPC) to another process listening on named message queue “rpc”. The RPC implements a an interface to a flood control dam and takes two arguments that you must package into a buffer that is to be sent and unpacked on the other side: water level and chance of rain, both of which are obtained by calls to sensorHoHLevel() and predictionRain(). 

On the remote side, a message processing loop takes the arguments passed through the message interface and calls floodControl(walter level, chance of rain) and returns a number indicating the percent to which the flood gates should be opened. 

This interface should run every 3 m. Assume the following functions which are loosely inspired by a POSIX specification with several simplifications to the interface and an assumption of guaranteed delivery:
    
    message queue functions:

    mq_type mqOpen(char *qname, char *mode, int maxnummsg) – Open named
    message queue qname in mode “r” read, “w” write, or “rw” read-write.
    Variable maxnummsg indicates the message capacity of the channel in
    number of messages (not bytes).

    void * mqReceive(mq_type queue) – Blocks until a message is available at which point the address of the message is returned.

    void mqSend(mq_type queue, void *msg, size_t msgSize) – Send the msgSize bytes contained in the buffer pointed to by msg to the specified message queue.

You may also assume void sleep(int N) causes the process to sleep for N seconds.

realistic implementation ```
// reads HoH and Rain Prediction and writes to rpc
sensors()
{
    return sensorHoHLevel(), predictionRain()
}
// reads from messages from sensor in rpc and uses them to control gate
control(water_level, chance_of_rain)
{
    floodControl(water_level, chance_of_rain)
}
// main application
main() 
{
    // bind control callback to rpc chanel
    rpc::server srv("0.0.0.0:4200");
    // binds control callback function to the rpc chanel
    srv.bind("rpc", control).run();
    // start reading rain predictions and HoH levels
    // and send them to the server
    rpc::client c("0.0.0.0:4200");
    while(1){
        c.call("rpc", sensors);
        sleep(60*3);
    }
}
```

non standard interface ```
// reads HoH and Rain Prediction and writes to rpc
sensors()
{
    return sensorHoHLevel(), predictionRain()
}

// sensor reading program
main() 
{
    mq_type ss = mqOpen("rpc", "w");
    while(1){
        data = sensors();
        mqSend("rpc", data, sizeof(data));
        sleep(60*3);
    }
}

// reads from messages from sensor in rpc and uses them to control gate
control(water_level, chance_of_rain)
{
    floodControl(water_level, chance_of_rain)
}
// flood gate control program
main()
{
    mq_type ss = mqOpen("rpc", "r")
    while(1)
    {
        control(mqRecieve(rpc));
    }
}
```

#### Question 4

An adaptive mutex is a semaphore that makes a decision as to whether or not to busy wait depending upon the state of other processes in a multi-processor system. When a process attempts to wait on a semaphore, the following is done (for simplicity, we consider a mutex between two processes only):

```
wait(Semaphore S)
{
    S.value = S.value - 1;
    if (S.value < 0)
    {
        /* Some other process P holds the semaphore */
        // P = process in critical region for semaphore S;
        if (state(P) == running)
        {
            while (S.value < 0)
            {
                // no - op;
            }
        }       
        else
        {
            // add calling process to wait list for S;
            // block calling process;
        }
    }
}
```

What is the rationale for providing such a semaphore?

Answer: For other concurrency architectures, a lot of time can be wasted waiting for the current thread to gain access to a lock variable. In an adaptive mutex this problem is solved by providing a non blocking function call if the lock is not available to the current process, so other process can do something else besides waiting. 

To be honest this isn't really an adaptive mutex, and this problem should probably be reworked or extended. In real adaptive mutex locks, the key point is a spin cycle estimator that predicts the amount of time that a process should block other threads for waiting for the lock before it goes into a non blocking mode. 

To get estimator value for process, measure avg wait cycles, then for each new value estimator += (new_val - estimator)/8. There is some cap to ensure new_value is not infinite, and estimator is smoothed out exponentially with each new attempt to get lock.

super great details written by the dude who wrote the original lib -> https://stackoverflow.com/a/25168942