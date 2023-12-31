# Parallel-And-Distributed-Computing
Parallel and Distributed Computing course final project.
# What does the project do?
### Parallel Implementation of Proximity Criteria
A set of N points is placed in a two-dimensional plane. Coordinates (x, y) of each point P are defined as follows:
* x = ((x2 – x1) / 2 ) * sin (t*π /2) + (x2 + x1) / 2) 
* y = a*x + b

where (x1, x2, a, b) are constant parameters predefined for each point P.

### Problem Definition
We will say that some point P from the set satisfies a **Proximity Criteria** if there exist at least **K** points in the set with a distance from the point **P** less than a given value **D**.

Given a value of parameter **t**, we want to find if there exist at least 3 points that satisfy the Proximity Criteria. 
### Requirements
* Perform checks for Proximity Criteria for tCount + 1 values of  t:

  t = 2 * i / tCount  - 1,          i = 0,  1,  2,  3, …,  tCount

  where tCount is a given integer number.
* For each value of t find if there are three points that satisfy the Proximity Criteria. If such three points are found – don't continue evaluation for this specific value of t.
* The input file input.txt initially is known for one process only. The results must be written to the file output.txt by the same process.

# Input data and Output Result of the project
The input file contains **N** in the first line - the number of points in the set, **K** – the minimal number of points to satisfy the Proximity Criteria, distance **D**  and **TCount**. The next **N** lines contain parameters for every point in the set. One or more blanks are between the numbers in a file.
## Input.txt
N   K   D   TCount

id   x1    x2    a    b

id   x1    x2    a    b

id   x1    x2    a    b

…

id   x1    x2    a    b


#### For example

4      2      1.23     100

0    2.2     1.2      2       45.07

1    -1       26.2    4.4    -3.3

2    -43.3   12.2   4.7     20

3    11.0    -6.6    12.5   23

## Output.txt
The output file contains information about results found for points that satisfy the Proximity Criteria. 
*	For each t that 3 points satisfying the Proximity Criteria were found, it contains a line with the parameter t and ID of these 3 points
  
      Points  pointID1, pointID2, pointID3 satisfy Proximity Criteria at t = t1
      
      Points  pointID4, pointID5, pointID6 satisfy Proximity Criteria at t = t2
      
      Points  pointID7, pointID8, pointID9 satisfy Proximity Criteria at t = t3


*	In case that the points were not found for any t, the program outputs:
  
      "There were no 3 points found for any t."
