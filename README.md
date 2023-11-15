# Graph-Minor
1st Assignment Parallel and distributed programming.
4page pdf report:
-execution times of your implementations with respect to
    the number of vertices n
    the number of edges m
    the number of clusters c
    the number of threads
-argue about the validity and effectiveness of your code
-cite external sources
-mention other important information 
Prologue
--------
Gist of the problem
Adjacency Matrix (A):
Aij=1 if ijεE(G) (in other words if i and j are connected we mark 1 to the relative matrix element)
Aij=0 otherwise

If an element isn't 0 or 1, for example x, that could mean:
either that there are x edges connecting the two vertices
or that the edge between the two vertices is weighted and its value is x.

                             a  b  c  d
a o-------------o b       a  0  1  1  0
  |             |         b  1  0  0  1
  |             |         c  1  0  0  1
  |             |         d  0  1  1  0
c o-------------o d      
       Graph             Adjacency Matrix

The sum of the row's ones from the adjacency matrix signify the degree of the row's vertex.
The diagonal contains only zeros because a vertex cannot be adjacent to itself.
The matrix is symmetric across it's diagonal.

Then we divide the graph's vectors into clusters, which are smaller graphs that, when combined, 
form the original complete graph. The graph minor is a graph that is made when we combine some
of the original graph's vertices into a clusterso that there exists an edge that it's virtices
belong to the same cluster.

Homework
--------
Ω: If Ωij=1 then the node i belongs to the cluster j. Otherwise Ωij=0.
Transpose of a matrix is the swapping of the i and j coordinate of each element or the swapping
of the elements with regard to the diagonal.
Finally Graph minor-Matrix M: M = Ω(transpose) * A * Ω 

![Example](graph-example.png)

a o-------o b
  | \   / |
  |  \ /  |
c o---o   o----o
      d   e    f
     
 



External sources
----------------
https://chat.openai.com
Adjacency Matrix: https://www.youtube.com/watch?v=7AhHGp7EzZ8
Graph Minors: https://www.youtube.com/watch?v=IJr8jXkGvX0