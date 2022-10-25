Task1
Input is a large array of random integers.
Divide array into N parts, N = 4
Using parallel computing perform following processing: 
In each part leave only integers that are in between two medians of bound array parts,
for the first parts leave numbers lower than median of next part, for last part leave the ones greater than bound part, concatenate arrays of output parts.

Example
[55,4,10,12,33,2,15,99,4,12,44,23]

Parts
[55,4,10] [12,33,2] [15,99,4] [12,44,23]

Median
[10] [12] [15] [23]

Output parts
[4, 10] [12] [15] [23, 44]

Output
[4, 10, 12, 15, 23, 44]

Data should be processed in a multi-thread manner using all processor cores in a maximum efficient way, expected output is an array of int.

Task 2
Determine threshold size of array on which it will be more effective to use parallel computing instead of calculating in one thread

Test tasks should be posted on github