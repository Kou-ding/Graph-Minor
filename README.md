# Graph-Minor
![graph-minor-example](graph-minor.jpg)

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

Prologue
--------
### Adjacency Matrix
Adjacency Matrix (A):
- Aij=1 if ijεE(G) (in other words if i and j are connected we mark 1 to the relative matrix element)
- Aij=0 otherwise
If an element isn't 0 or 1, for example x, that could mean:
1. Either that there are x edges connecting the two vertices
2. Or that the edge between the two vertices is weighted and its value is x.
In the following example vertices a and b are connected with each other with two edges.
<pre>
                             a  b  c  d
a o-------------o b       a  0  2  1  0
  |\___________/|         b  2  0  0  1
  |             |         c  1  0  0  1
  |             |         d  0  1  1  0
c o-------------o d      
       Graph             Adjacency Matrix
</pre>
- The sum of the row's values from the adjacency matrix signify the **degree** (how many connections 
a particular vertex has) of the row's vertex.
- The diagonal contains only zeros because a vertex cannot be adjacent to itself.
- The matrix is symmetric across its diagonal.

### Clusters and V mapping
Then we divide the graph's vertices into clusters, which are smaller graphs that, when combined, 
form the original complete graph. The graph minor is a graph that is made when we follow any sequence
of the following moves: 
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
Sparse matrices are matrices that mainly have '0' elements. This means we can represent them in
a smart way in order to save time not having to display every single '0' element. When we face
large matrices this notation can prove a very efficient way to represent the data.
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
- Compressed Sparse Row(CSR) format: In (CSR) we change up the representation of the rows array. 
Whenever a row number is repeated, instead of rewriting that number, we now state the number of 
times a row number is repeated.
>Eg COO[0, 0, 0, 1, 2] -> CSR[3, 1, 1]
- Compressed Sparse Columns(CSC) format: Is the same as (CSR) but now we do the same thing with columns
as we previously did with rows.
<pre>
Coordinate format (COO)
unsigned rows[5]    = | 0,   0,   0,   1,   2   |;
unsigned columns[5] = | 0,   3,   5,   0,   3   |;
double   values[5]  = | 3.0, 4.0, 1.0, 1.0, 2.0 |;
Compressed Sparse Row format (CSR)
unsigned row_counts[3] = | 3,             1,   1 |;
unsigned columns[5]    = | 0,   3,   5,   0,   3 |;
double   values[5]     = | 3.0, 4.0, 1.0, 1.0, 2.0 |;
</pre>
| 0,   0,   0,   1,   2   |
| 0,   3,   5,   0,   3   |
| 3.0, 4.0, 1.0, 1.0, 2.0 |

### Mathematical calculation of Graph minor
In order to calculate the graph minor we will bw using Matrix Ω:
#### Ω: 
- If Ωij=1 then the node i belongs to the cluster j. 
- Otherwise Ωij=0.
Finally Graph minor Matrix:
- M: M = Ω(transpose) * A * Ω 
Transpose of a matrix is the swapping of the i and j coordinate of each element or the swapping
of the elements with regard to the diagonal.

### Matrix Market formfactor
Matrix market matrices are usually comprised of:
- Some general information at the start
- a row listing the matrix dimensions and the count of all non-zero values
- subsequeant rows that follow the pattern: [row, column, value]
>sometimes [value] is omitted 


Homework
--------
We are given the adjacency matrix (A) and the mapping vector (V) of a graph (G) and we are asked
to produce the Graph minor of G using parallel computing. 

There will be 4 implementations:
1. Pthreads
2. OpenMp
3. OpenCilk
4. Julia (optional)



To do list:
- [x] Matrix market tutorial
- [ ] General notion in Julia
- [x] How does V mapping works? 

External sources
----------------
- General consulting/debugging: https://chat.openai.com
- Adjacency Matrix: https://www.youtube.com/watch?v=7AhHGp7EzZ8
- Graph Minors: https://www.youtube.com/watch?v=IJr8jXkGvX0
- Ways to store matrices: https://medium.com/@jmaxg3/101-ways-to-store-a-sparse-matrix-c7f2bf15a229