#External Hash Based Join (Simulation in C++)


In DBMS, individual relations seldom fit in main memory. In such cases, implementation of operators involve optimally reading relations in chunks from secondary storage and then performing operations on them.

This project implements in C++ the join operator in DBMS using external hashing by keeping in mind the record sizes, page size and the  number of available pages in main memory. Every hashing round hashes records into buckets till an in-memory join can be performed on the buckets. In-memory join is performed by sorting values in both the buckets and then finding pairs by using a two-pointer approach in O(n).

### Hash Function
The hash function applied must depend on both the value to be hashed and the hashing round number. This is important as we want values that mapped to the same bucket in round_(x) to map to different buckets in round_(x+1) so that eventually individual buckets can fit into main memory. 
Possible hash functions could be:
- h1(key,roundNo,noOfBuckets) = floor(key * roundNo * pi) % noOfBuckets (Or any other irrational number instead of pi)
- h2(key,roundNo,noOfBuckets) = binaryToDecimal( (noOfBuckets + roundNo) LSB of key) % noOfBuckets

Example:

- dataGeneration.cpp creates two relation files by generating values (using a random number generator) lying in a specified range. 

- relation1.txt and relation2.txt both contain 50 integer values, each lying in the range of [1,200].

- h2 has been used as the hashing function

- outputLog.txt shows step by step the procedure followed by the algorithm, the final join output and the number of IO's used.
