function graphminor(filename)
    # Open the file
    f = open(filename, "r")

    # Initialize arrays 
    I = Int[]
    J = Int[]
    val = Float64[]

    # Read matrix info
    for line in eachline(f)
        # if inside our loop to skip comments
        if line[1] == '%'
            continue
        end
        i, j, v = [parse(Int, x) for x in split(line)]
        push!(I, i)
        push!(J, j)
        push!(val, v)
    end

    # Close the file
    close(f)

    # Calculate the maximum value in Vector
    vector_max = maximum(union(I, J))

    # Initialize Vector
    Vector = rand(1:3, vector_max)

    # Check if Vector is properly initialized
    @assert !isempty(Vector)

    # Initialize graph minor
    gm = zeros(Float64, vector_max, vector_max)

    for x in eachindex(I)
        if Vector[I[x]] != Vector[J[x]]
            gm[Vector[I[x]], Vector[J[x]]] += val[x]
        end
    end

    return gm
end

# Example usage
filename = ARGS[1]
println(graphminor(filename))

elapsed_time = @elapsed begin
   graphminor(filename)
end
println("Execution time: $elapsed_time")