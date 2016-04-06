In DBMS, individual relations seldom fit in main memory. In such cases, implementation of operators involve optimally reading relations in chunks from secondary storage and then performing operations on them.

This project implements in C++ the join operator in DBMS using external hashing by keeping in mind the record sizes, page size and the  number of available pages in main memory. Every hashing round hashes records into buckets till an in-memory join can be performed on the buckets.
