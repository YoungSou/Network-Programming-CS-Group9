# Distributed Text Processing System

##  Overview

This project implements a **distributed text processing system** using TCP sockets in Python.
The system is composed of three main components:

* **Client**: Sends a large text file to the coordinator
* **Coordinator**: Splits the file and distributes tasks
* **Workers**: Process text chunks in parallel and return results

The system demonstrates a simplified version of a distributed computing model similar to MapReduce.

---

##  Features

* Distributed processing using multiple workers
* TCP socket communication
* Message framing (length-prefixed messages)
* Multi-threading in coordinator (parallel worker handling) 
* Word count and line count
* Word frequency analysis
* Top 5 most frequent words 
* Processing time measurement 

---

##  System Architecture

Client → Coordinator → Workers → Coordinator → Client

### Workflow:

1. Client sends the text file to the Coordinator
2. Coordinator splits the file into chunks
3. Each chunk is sent to a Worker
4. Workers process data in parallel
5. Results are sent back to the Coordinator
6. Coordinator merges results and sends final output to Client

---

##  Project Structure

```
project/
│
├── client.py
├── coordinator.py
├── worker.py
├── utils.py
├── bigfile.txt
└── README.md
```

---

##  How to Run

### Step 1: Start Workers (in 2 terminals)

```bash
python worker.py 9001
python worker.py 9002
```

---

### Step 2: Start Coordinator

```bash
python coordinator.py
```

---

### Step 3: Run Client

```bash
python client.py
```

---

##  Example Output

```
=== RESULT ===
Lines: 10000
Words: 90000
Unique words: 1500
Top 5 words: [('test', 20000), ('data', 20000), ...]
Processing time (s): 0.25
```

---

##  Technical Details

###  Communication

* Uses TCP sockets
* JSON format for message exchange
* Message framing implemented using length-prefix

###  Parallel Processing

* Coordinator uses Python threading
* Each worker runs independently on different ports

###  Data Processing

Each worker calculates:

* Number of lines
* Number of words
* Word frequency

Coordinator aggregates:

* Total lines
* Total words
* Unique words
* Top 5 most frequent words

---

##  Test Data

* The system is tested with a text file containing **at least 10,000 lines**
* File used: `bigfile.txt`

---

##  Bonus Implementations

* Multi-threaded coordinator
* Top 5 frequent words
* Execution time measurement

---

##  Notes

* All components must be running before executing the client
* Ensure ports are not already in use
* Workers must be started before the coordinator

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
