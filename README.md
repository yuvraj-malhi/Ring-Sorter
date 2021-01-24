# Ring-Sorter

## Description
This is a fun project to test theoretical concepts on an operating system. Before proceeding, there are a few terms to keep in mind:

  1. Ring Topology: This referes to a number of programs connected in a circular toplogy. In our method, a program can send a message on in the anti-clock direction. Hence if a program were to send a message to it's adjacent program on the left, the message would have to be passed to the right program, which passes the message to it's right program, and so on until the message is finally delivered to it's receipient.
  
  2. TCP sockets: All the programs in Ring Sorter are connected through TCP sockets. These sockets are reliable and don't require ACK handling on an application level.
  
  3. Mergesort with Ring Topology: To summarise, a traditional merge sort algorithm takes place like this:
   ``` 
   Algorithm mergeSort(S)             // Array S of integers
      if size(S) > 1
          (S1, S2) <- partition(S)    // Partition S into smaller arrays S1,S2
          mergeSort(S1)               // Sort the smaller array S1
          mergeSort(S2)               // Sort the smaller array S2
          S <- merge(S1, S2)
          return
      else
          return
     ```
     
