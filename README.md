# Course Information

[ETH Course Catalogue](https://www.vorlesungen.ethz.ch/Vorlesungsverzeichnis/lerneinheit.view?semkez=2022W&ansicht=KATALOGDATEN&lerneinheitId=163373&lang=en)

## Abstract

 This course gives an introduction into algorithms and numerical methods for parallel computing on shared and distributed memory architectures. The algorithms and methods are supported with problems that appear frequently in science and engineering.

## Objective

With manufacturing processes reaching its limits in terms of transistor density on today’s computing architectures, efficient utilization of computing resources must include parallel execution to maintain scaling. The use of computers in academia, industry and society is a fundamental tool for problem solving today while the “think parallel” mind-set of developers is still lagging behind.

The aim of the course is to introduce the student to the fundamentals of parallel programming using shared and distributed memory programming models. The goal is on learning to apply these techniques with the help of examples frequently found in science and engineering and to deploy them on large scale high performance computing (HPC) architectures.

## Content

1. Hardware and Architecture: Moore’s Law, Instruction set architectures (MIPS, RISC, CISC), Instruction pipelines, Caches, Flynn’s taxonomy, Vector instructions (for Intel x86)

2. Shared memory parallelism: Threads, Memory models, Cache coherency, Mutual exclusion, Uniform and Non-Uniform memory access, Open Multi-Processing (OpenMP)

3. Distributed memory parallelism: Message Passing Interface (MPI), Point-to-Point and collective communication, Blocking and non-blocking methods, Parallel file I/O, Hybrid programming models

4. Performance and parallel efficiency analysis: Performance analysis of algorithms, Roofline model, Amdahl’s Law, Strong and weak scaling analysis

5. Applications: HPC Math libraries, Linear Algebra and matrix/vector operations, Singular value decomposition, Neural Networks and linear autoencoders, Solving partial differential equations (PDEs) using grid-based and particle methods

# Lecture Notes

Personal Lecture note is in this repository. Official documents of these course can be viewed in this website: https://www.cse-lab.ethz.ch/teaching/hpcse-i-hs22/