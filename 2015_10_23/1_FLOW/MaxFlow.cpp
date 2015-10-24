template <typename T>
float resudialCapacity(const graph<T> &g,
                       const std::map<std::pair<T, T>, float> &flows,
                       const std::map<std::pair<T, T>, float> &capacity,
                       const T &src,
                       const T &dst)
{
    std::pair<T, T> pair = std::make_pair(src, dst);
    if (g.hasEdge(src, dst) == true)
        return capacity.find(pair)->second - flows.find(pair)->second;
    else if(g.hasEdge(dst, src) == true)
        return -flows.find(pair)->second;

    return 0;
}

template<typename T>
bool push(const graph<T> &g, 
          const T& src,
          const T& dst,
          std::map<T, float> &excess, 
          std::map<std::pair<T, T>, float> &flows,
          const std::map<std::pair<T, T>, float> &capacity)
                       
{
    float resCapacity = resudialCapacity(g, flows, capacity, src, dst);  
    float delta = min(excess[src], resCapacity);
    std::pair<T, T> edge = std::make_pair(src, dst);
    if (g.hasEdge(src, dst) == true)
        flows[edge] += delta;
    else
        flows[std::make_pair(dst, src)] -= delta;

    excess[src] -= delta;
    excess[dst] += delta;

    if (excess[src] <= 0)
    {
        excess.erase(src);
        return false;
    }
    return true;
}

template <typename T>
bool relabel(const graph<T> &g, const T &vertex, std::map<T, size_t> &heights, std::map<std::pair<T, T>, float> &flows, const std::map<std::pair<T, T>, float> &capacity)
{
    size_t min_height = -1; 
    for (typename graph<T>::const_edge_iterator edge = g.begin(vertex); edge != g.end(vertex); ++edge)
    {
        float resCapacity = resudialCapacity(g, flows, capacity, vertex, edge->Destination);
        if (resCapacity <= 0)
            continue;
        if (heights[vertex] > heights[edge->Destination])
            return false;
        min_height = min(min_height, heights[edge->Destination]);
    }
    if (min_height == -1)
        return false;
    heights[vertex] = 1 + min_height;
    return true;
}

template <typename T>
float maxFlow(const graph<T> &g, const T &source, const T &target)
{
    graph<T> resudial_net = g;
    std::map<std::pair<T, T>, float> flows, capacity;
    std::map<T, float> excess;
    std::map<T, size_t> heights;

    for (typename graph<T>::const_iterator vertex = g.begin(); vertex != g.end(); ++vertex)
    {
        heights[*vertex] = 0;
        for (typename graph<T>::const_edge_iterator edge = g.begin(*vertex); edge != g.end(*vertex); ++edge)
        {
            std::pair<T, T> pair = std::make_pair(edge->Source, edge->Destination);
            flows[pair] = 0;
            capacity[pair] = edge->weight;
        }
    }

    heights[source] = g.size();

    for (typename graph<T>::const_edge_iterator edge = g.begin(source); edge != g.end(source); ++edge)
    {
        std::pair<T, T> pair = std::make_pair(source, edge->Destination);
        flows[pair] = edge->weight;
        excess[pair.second] = edge->weight;
//        excess[pair.first] -= edge->weight;
    }

    float flow = 0;
    
    std::map<T, float> snapshot1 = excess;
    std::map<T, size_t> snapshot2 = heights;

    while (excess.empty() == false)
    {
        T vertex;
        for (typename std::map<T, float>::const_iterator it = excess.begin(); it != excess.end(); ++it)
        {
            float resCapactiySum = 0;
            for (typename graph<T>::const_edge_iterator edge = g.begin(it->first); edge != g.end(it->first); ++edge)
            {
                float resCapacity = resudialCapacity(g, flows, capacity, it->first, edge->Destination);
                if (resCapacity > 0)
                {
                    resCapactiySum += resCapacity;
                    break;
                }
            }

            if (resCapactiySum > 0)
            {
                vertex = it->first;
                break;
            }
        }

        for (typename graph<T>::const_edge_iterator edge = g.begin(vertex); edge != g.end(vertex); ++edge)
        {
            float resCapacity = resudialCapacity(g, flows, capacity, vertex, edge->Destination);
            if (resCapacity > 0 && heights[vertex] == heights[edge->Destination] + 1)
            {
                if (push(g, vertex, edge->Destination, excess, flows, capacity) == false)
                   break;
            }
        }

        for (typename std::map<T, float>::const_iterator it = excess.begin(); it != excess.end(); ++it)
        {
            relabel(g, it->first, heights, flows, capacity);
        }

        bool one = (snapshot1 == excess);
        bool two = (snapshot2 == heights);

        if (one && two)
        {
            break;
        }
        snapshot1 = excess;
        snapshot2 = heights;
    }

    for (typename graph<T>::const_iterator vertex = g.begin(); vertex != g.end(); ++vertex)
    {
        if (*vertex == target)
            continue;

        flow += flows[std::make_pair(*vertex, target)]; 
    }
    return flow;
}
