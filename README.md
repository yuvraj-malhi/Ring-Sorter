# Ring-Sorter

## Description
This is a fun project to test theoretical concepts on an operating system. Before proceeding, there are a few terms to keep in mind:

  1. Ring Topology: This referes to a number of programs connected in a circular toplogy. In our method, a program can send a message on in the anti-clock direction. Hence if a program were to send a message to it's adjacent program on the left, the message would have to be passed to the right program, which passes the message to it's right program, and so on until the message is finally delivered to it's receipient.
  
  2. TCP sockets: All the programs in Ring Sorter are connected through TCP sockets. These sockets are reliable and don't require ACK handling on an application level.
  
  3. Mergesort with Ring Topology: To summarise, a traditional merge sort algorithm takes place like this:

   Algorithm mergeSort(S)          // Array S of integers
      if size(S) > 1
          (S1, S2) <- partition(S)    // Partition S into smaller arrays S1,S2
          mergeSort(S1)               // Sort the smaller array S1
          mergeSort(S2)               // Sort the smaller array S2
          S <- merge(S1, S2)
          return
      else
          return
          
          
   [For more reference on merge sort, refer to https://www.youtube.com/watch?v=TzeBrDU-JaY]
     
     
   Now, to use this algorithm in mergesort requires some understanding. 
   Let's say 'N' nodes are connected in a ring topology (N=2^n where n is an integer. N is taken as user input). The root node (specified on command line) sends half of the data to a node in the middle, which in turn sends half of the data to a node in middle of the rest. This is done until the data becomes singleton. After that, the data is merged back in the same fashion in which it was divided. Hence, resulting in complete sorting of array.

   For example in a ring of 8 nodes (i.e integer array of 8 elements):

![image](https://user-images.githubusercontent.com/76866159/105621676-1d6c2580-5e30-11eb-9b9e-19c449cd8a6e.png)

  The transfer of data happens as follows:
  
![image](https://user-images.githubusercontent.com/76866159/105621700-4c829700-5e30-11eb-8464-03a233735bf0.png)

  4. Coordinator and Node: To do all the above tasks, the work has been divided into two programs: coordinator.c and node.c
  
     The Node program creates a number of node processes (using fork()) that are connected in a ring (with coordinator also present in the ring).
     
     The Coordinator reads the file of integers from "data.txt", and initiates the transfer of data. All other nodes wait for the data and once received will split it among the nodes as specified above. The coordinator prints the final sorted output to "output.txt". Every node should print the source ip and port when they receive data. both these programs print the IP and port number on which they send and receive data. 
     
## Setup 
### Step I
Copy all the 6 files into a new folder in your system.
Do this manually or just run this command in a new folder:

```sudo git clone https://github.com/yuvrajmalhi/Ring-Sorter.git```

![image](https://user-images.githubusercontent.com/76866159/105621974-2e6a6600-5e33-11eb-81a6-22af33a0197a.png)

### Step II
While still in the directory in which you have the new files, run the command:

```sudo make```

The output must be as follows:

![image](https://user-images.githubusercontent.com/76866159/105622019-899c5880-5e33-11eb-87f5-2ea9ac9c6ef2.png)

## Usage
### Step I - Input data
The input datat has to be specified in the file "data.txt".
**_The number of integers HAVE to be in powers of 2, like 2 or 4 or 8...._**
By default 8 random integers have been put in "data.txt", so just for the sake of testing you may use data.txt as it is (so just skip this step if you want to test).

### Step II - Split terminal
Since we have two programs, split terminal into two or open a new terminal and navigate to the folder where all files are stored. 

![image](https://user-images.githubusercontent.com/76866159/105622112-8bb2e700-5e34-11eb-8e22-2581f3ab75cc.png)

### Step III - Node
In one of the terminals, run the command:
```sudo ./node ```

![image](https://user-images.githubusercontent.com/76866159/105622139-cae13800-5e34-11eb-8389-c2cb7ef309ae.png)

Now the node is waiting for cordinator to open and and read the integers.

### Step IV - Coordinator
In the other terminal, run the command:

``` sudo ./cordinator 8```

I am typing 8 because I have 8 integers in my data.txt, you should type 2 or 4 or 8 or 16... depending on the number of integers that you have input in "data.txt".

![image](https://user-images.githubusercontent.com/76866159/105622204-670b3f00-5e35-11eb-8f5a-17939f71a980.png)

Note that the first step that heppens here is reading of the data, and the read integers from "data.txt" are displayed.

Then initalisation happens and a ring topology is connected.

lastly the merge sort starts under the heading "SORTING" and as described in the description, the data moves around till it is sorted. Now note that each message is being tracked and it's details are shown when any node receives or sends data.

![image](https://user-images.githubusercontent.com/76866159/105622259-029caf80-5e36-11eb-98fc-9e4f19fa6e79.png)

### Step V - Result
On the coordinator's output, you will see the final sorted result!.

![image](https://user-images.githubusercontent.com/76866159/105622277-324bb780-5e36-11eb-9f77-9def2d0b8a65.png)


Do share if you liked my work. Thanks!

:smile:
