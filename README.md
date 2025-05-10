# Virtual Memory Manager — COMP30023 Project 1

This is a simulation of a **virtual memory management system** written in **C** for Project 1 of COMP30023: Computer Systems at the University of Melbourne.

The program simulates translation from logical to physical addresses using:

- A **Page Table**
- A **Translation Lookaside Buffer (TLB)** with **LRU replacement**
- A **FIFO Page Replacement** algorithm when memory is full

## 🧠 Tasks Implemented

- **Task 1**: Parse 32-bit logical addresses into page number and offset  
- **Task 2**: Implement page table and frame allocation  
- **Task 3**: Add FIFO-based page eviction when frames are full  
- **Task 4**: Add TLB with LRU and flushing on eviction

## 🚀 Usage

```bash
make
./translate -f input.txt -t task4
