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
  
     The Node program creates a number of nodes that are connected in a ring (with coordinator also present in the ring).
     
     The Coordinator reads the file of integers from "data.txt", and initiates the transfer of data. All other nodes wait for the data and once received will split it among the nodes as specified above. The coordinator prints the final sorted output to "output.txt". Every node should print the source ip and port when they receive data. both these programs print the IP and port number on which they send and receive data. 
     
## Usage 
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




