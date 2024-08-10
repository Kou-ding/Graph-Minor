# Graph-Minor-Assignment
Homework 1
----------
We are given the adjacency matrix (A) and the mapping vector (V) of a graph (G) and we are asked to produce the Graph minor of G using parallel computing. 

### C/C++
My code utilizes the basic code from Matrix Market to eventually come up with the code that produces the graph minor. First I use the arrays I[], J[], var[] from the matrix market as they have been generated and utilizing a mapping vector graph's vertices are mapped into clusters. In order to generate the graph minor I, first, create a double pointer called graphMinor and then initialize its elements to be zero. Then I traverse the arrays I[], J[] and check if their cluster is the same(if Vector[I[x]]==Vector[J[x]]). If it is not then the value of the, as originally meant to be, edge between the two vertices is added to a summation. This summation will gradually include all the edge values between every combination of the different cluster elements and these will be stored at the graph minor array element denoted by the two cluster numbers (graphMinor(cluster1)(cluster2)).

|Id |Row|Column|Value|Vector(cluster of row)|Vector(cluster of column)|
|---|----|----|----|-------------|-------------|
|1  | r1 | c1 | v1 |Cluster-of-r1|Cluster-of-c1|
|2  | r2 | c2 | v2 |Cluster-of-r2|Cluster-of-c2|
|3  | r3 | c3 | v3 |Cluster-of-r3|Cluster-of-c3|
|4  | r4 | c4 | v4 |Cluster-of-r4|Cluster-of-c4|
|5  | r5 | c5 | v5 |Cluster-of-r5|Cluster-of-c5|
|6  | r6 | c6 | v6 |Cluster-of-r6|Cluster-of-c6|
|...|... |... |... |     ...     |     ...     |

>for example here, if we run the code, it create a loop lasting until nnz depletes. Each repitition we check if the cluster of the row is the same with the cluster of the column. If it is the code does nothing but if it is not then we assign the value that connects the two vertices to the double pointer array Graph Minor the first coordinate of which being the cluster of the first vertice and the second one being the cluster of the second vertice.  

### openMP


### Calculations
When calculating the execution times I start the clock when the code begins and stop the clock just before I print out the graph minor. The reason why I do that is because the graph minor matrix is already in the memory and the only thing that remains is pulling out the elements we want to depict.

Execution times mean, Matrix: celegansneural.mtx
|C++ |openMp |openCilk| Pthreads| Julia|
|----|-------|--------|---------|------|
|0.003|||||
|0.002|||||
|0.003|||||
|0.004|||||
|0.002|||||
|Mean score|Mean score|Mean score|Mean score|Mean score|
|0.0028|||||

Execution times mean, Matrix: foldoc.mtx
|C++ |openMp |openCilk| Pthreads| Julia|
|----|-------|--------|---------|------|
|0.198|||||
|0.207|||||
|0.240|||||
|0.200|||||
|0.201|||||
|Mean score|Mean score|Mean score|Mean score|Mean score|
|0.2092|||||

Execution times mean, Matrix: language.mtx
|C++ |openMp |openCilk| Pthreads| Julia|
|----|-------|--------|---------|------|
|4.112|||||
|4.055|||||
|4.094|||||
|4.090|||||
|4.456|||||
|Mean score|Mean score|Mean score|Mean score|Mean score|
|4.1614|||||

Execution times mean, Matrix: eu-2005.mtx
|C++ |openMp |openCilk| Pthreads| Julia|
|----|-------|--------|---------|------|
|41.162|||||
|41.049|||||
|44.852|||||
|41.887|||||
|41.919|||||
|Mean score|Mean score|Mean score|Mean score|Mean score|
|42.1738|||||

Matrices' sources
- https://sparse.tamu.edu/Newman/celegansneural
- https://sparse.tamu.edu/Pajek/foldoc
- https://sparse.tamu.edu/Tromble/language
- https://sparse.tamu.edu/LAW/eu-2005

External sources
----------------
- General consulting/debugging: https://chat.openai.com
- Adjacency Matrix: https://www.youtube.com/watch?v=7AhHGp7EzZ8
- Graph Minors: https://www.youtube.com/watch?v=IJr8jXkGvX0
- Ways to store matrices: https://medium.com/@jmaxg3/101-ways-to-store-a-sparse-matrix-c7f2bf15a229
- Leiden Algorithm: https://www.nature.com/articles/s41598-019-41695-z
- Malloc 2d array: https://www.youtube.com/watch?v=aR7tkVj3UU0

# Graph-Minor-Additional information
[graph-minor-example](graph-minor.jpg)

1st Assignment Parallel and distributed programming.
4page pdf report including:

1. Execution times of your implementations with respect to:
  - The number of vertices **n**
  - The number of edges **m**
  - The number of clusters **c**
  - The number of threads **p**
2. Argue about the validity and effectiveness of your code
3. Cite external sources
4. Mention other important information 

There will be 4 implementations:
1. C/C++
2. OpenMP
3. OpenCilk
4. Pthreads
5. Julia (optional)

Analysis of Individual knowledge needed for the assignment
--------
### Adjacency Matrix
Adjacency Matrix (A):
- Aij=1 if ijεE(G) (in other words if i and j are connected we mark 1 to the relative matrix element)
- Aij=0 otherwise
If an element isn't 0 or 1, suppose it is equal to x, that could mean:
1. Either that there are x edges connecting the two vertices
2. Or that the edge between the two vertices is weighted and its value is x.
In the following example-vertices, a and b are connected with each other with two edges.
<pre>
                             a  b  c  d
a o-------------o b       a  0  2  1  0
  |\___________/|         b  2  0  0  1
  |             |         c  1  0  0  1
  |             |         d  0  1  1  0
c o-------------o d      
       Graph             Adjacency Matrix
</pre>
- The sum of the row's values from the adjacency matrix signifies the **degree** (how many connections a particular vertex has) of the row's vertex.
- The diagonal contains only zeros because a vertex cannot be adjacent to itself.
- The matrix is symmetric across its diagonal.

### Clusters and V mapping
Then we divide the graph's vertices into clusters, which are smaller graphs that, when combined, form the original complete graph. The graph minor is a graph that is made when we follow any sequence of the following moves: 
- Delete a vertex
- Delete an edge
- Contract an edge
<pre>
a o-------o b                              
  | \   / |           V={1,2,1,1,2,2} -->    c1 o------o c2
  |  \ /  |              a,b,c,d,e,f             \____/
c o---o   o----o    Which cluster does each vertex belong to
      d   e    f
</pre>

### Sparse matrices
Sparse matrices are matrices that mainly have '0' elements. This means we can smartly represent them in order to save time not having to display every single '0' element. When we face large matrices this notation can prove a very efficient way to represent the data.
<pre>
    a b c d 
  a 0 0 0 1        row     |a b b c d
  b 1 0 0 5       column   |d a d b d 
  c 0 8 0 0       values   |1 1 5 8 3 
  d 0 0 0 3       (COO) Representation
Sparse Matrix             
</pre>

### Other representations
- Coordinate(COO) format: In (COO) we represent a matrix with three arrays: 
  - a rows array
  - a columns array
  - and a values array.
- Compressed Sparse Row(CSR) format: In (CSR) we change up the representation of the rows array. Whenever a row number is repeated, instead of rewriting that number, we now state the number of times a row number is repeated.
>Eg COO[0, 0, 0, 1, 2] -> CSR[3, 1, 1]
- Compressed Sparse Columns(CSC) format: It is the same as (CSR) but now we do the same thing with columns as we previously did with rows.
<pre>
Coordinate format (COO)
unsigned rows[5]    = [ 0,   0,   0,   1,   2   ];
unsigned columns[5] = [ 0,   3,   5,   0,   3   ];
double   values[5]  = [ 3.0, 4.0, 1.0, 1.0, 2.0 ];
Compressed Sparse Row format (CSR)
unsigned row_counts[3] = [ 3,             1,   1 ];
unsigned columns[5]    = [ 0,   3,   5,   0,   3 ];
double   values[5]     = [ 3.0, 4.0, 1.0, 1.0, 2.0 ];
</pre>

### Mathematical calculation of Graph minor
In order to calculate the graph minor we will bw using Matrix Ω:
#### Ω: 
- If Ωij=1 then the node i belongs to the cluster j. 
- Otherwise Ωij=0.
Finally Graph minor Matrix:
- M: M = Ω(transpose) * A * Ω 
Transpose of a matrix is the swapping of the i and j coordinate of each element or the swapping of the elements with regard to the diagonal.

### Matrix Market form factor
Matrix market matrices are usually comprised of:
- Some general information at the start
- a row listing the matrix dimensions and the count of all non-zero values (number of non-zeros **nnz**)
- subsequent rows that follow the pattern: [row, column, value]
>sometimes [value] is omitted 

# Tickboxes
The knowledge we acquired on the way:
- [x] Git - GitHub
- [x] Markdown file syntax
- [x] Programming in Julia

Things yet to be implemented:
- [ ] Sub-matrices  
- [ ] dense function for smaller matrices
- [x] transfer code from Windows to Unix
- [x] Julia code
