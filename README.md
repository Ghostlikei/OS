# OS
A repo recording learning materials of operation system. Created on 2023.3.5.

### Expectation

Operation System is one of the most important course of computer science. In order to understand thoughts behind the design, this repo would collect excellent learning resources and my own thoughts and codes.

---

At ECNU, we use **MINIX** to finish labs and the book *Modern Operating Systems* to take courses. However, this operation systems seems to be outdated, and labs are hard to work with.

The highest rating course for introduction of OS must be *MIT 6.S081*, which designed **XV6** system running with **RISC_V** Instructions and about 10k lines of system codes. I'm planning to finish all the labs and write comments or even create gitbooks to help understand the system.

- [xv6-book 中文版 上课笔记和补充](https://mit-public-courses-cn-translatio.gitbook.io/mit6-s081/)，属于必须得看的级别

- [6.s081 website 2021 fall](https://pdos.csail.mit.edu/6.828/2021/schedule.html)
- My solution to homeworks: Save locally, will be published after all labs finished
  - Lab1: utils (Finished on 2/28)
  - Lab2: syscall (Finished on 3/7)
  - Lab3: page table (Finished on 3/15)
  - Lab4: Traps (Finished on 3/28)
  - Lab5: Copy-On-Write Fork (Finished on 4/4)

[*UCB CS162*](https://cs162.org/) is far more challenging. If you wanna overall understanding of OS, project Pintos will take you to build your own os.

---

Also, there are lots of outstanding courses and reference books to learn OS. Such as:

*OSTEP(Operation System: Three easy pieces)*. 

### Idea

Due to the outdated minix lab (which we need to do on osecnu), I planned to reinterpret the labs on xv6 and design the similar codes, but workload maybe large.

- What we need to do on minix?
  - Lab1 shell (Finished)
  - Lab2 implement EDF on MLFQ (Ongoing)
  - Lab3 I/O (unknown)
  - Lab4 Memory management? (unknown)

I will try my best to do so, but it may be put on another repo. I haven't decided yet.
