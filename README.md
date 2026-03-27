#  Distributed Text Processing System (C Version)

##  Overview

This project implements a distributed text processing system using TCP sockets in C.

The system processes a large text file by distributing the workload across multiple workers, improving performance through parallel execution.

---

##  System Architecture

Client → Coordinator → Workers → Coordinator → Client

### Components:

- Client  
  - Reads input file  
  - Sends data to Coordinator  
  - Receives final result  

- Coordinator  
  - Splits input data  
  - Sends tasks to workers  
  - Collects and merges results  
  - Returns aggregated result  

- Workers  
  - Process text chunks  
  - Count lines and words  
  - Compute word frequency  

---

##  Features

- TCP socket communication (Winsock)  
- Message framing (length + data)  
- Multi-threading (Coordinator)  
- Distributed processing  
- Word frequency analysis  
- Top 5 most frequent words  
- Processing time measurement  

---

##  Technologies Used

- C programming language  
- Winsock API  
- Multi-threading  
- TCP/IP networking  

---

##  Project Structure

project/
│
├── client.c  
├── coordinator.c  
├── worker.c  
├── utils.c  
├── utils.h  
├── bigfile.txt  
└── README.md  

---


##  Team Members

* Member 1: Le Tung Lam
* Member 2: Vu Hong Phuc
* Member 3: Tang Nguyen Duc Anh
* Member 4: Truong Tuan Minh

---

##  Conclusion

This project demonstrates how distributed systems can improve performance by splitting tasks across multiple workers and processing them in parallel.

---
