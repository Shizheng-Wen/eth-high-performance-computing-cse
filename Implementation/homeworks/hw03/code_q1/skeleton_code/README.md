# Modules
module load gcc/6.4.0 (or newer)

# Interactive Shell
bsub -n 12 -W 00:30 --Is bash

# Make commands

**Compilation**

make

**Run simulation** (for Question 1d) 

make run

**Collect statistics** (for Question 2e)

make stat

make statjob (alternative)

make statjobfull (alternative, request full node, may take long)

**Plot statistics** (for Question 2e)

make plotstat

**Cleanup**

make clean
