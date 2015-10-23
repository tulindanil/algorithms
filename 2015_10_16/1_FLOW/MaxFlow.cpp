template <typename T>
bool isReachable(const graph<T> &g, const T &s, const T &t, const std::map<T, T> destinyMap)
{
    if (destinyMap.find(t) != destinyMap.end())
        return true;
    return false;
}

template <typename T>
std::vector<T> findPath(const graph<T> &g, const T &s, const T &t)
{
    std::map<T, T> destinyMap = g.BFS(s);
    if (isReachable(g, s, t, destinyMap) == false)
        return std::vector<T>();

    std::vector<T> path;

    T current_vertex = t;
    while (current_vertex != s)
    {
        path.push_back(current_vertex);
        current_vertex = destinyMap[current_vertex];
    }
    path.push_back(s);
    std::reverse(path.begin(), path.end());

    return path;
}

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

template <typename T>
float maxFlow(const graph<T> &g, const T &source, const T &target)
{
    graph<T> resudial_net = g;
    std::map<std::pair<T, T>, float> flows, capacity;
    for (typename graph<T>::const_iterator vertex = g.begin(); vertex != g.end(); ++vertex)
    {
        for (typename graph<T>::const_edge_iterator edge = g.begin(*vertex); edge != g.end(*vertex); ++edge)
        {
            std::pair<T, T> pair = std::make_pair(edge->Source, edge->Destination);
            std::pair<T, T> r_pair = std::make_pair(pair.second, pair.first);  
            flows[pair] = 0;
            flows[r_pair] = 0;

            capacity[pair] = edge->weight;
        }
    }

    std::vector<T> path;
    while((path = findPath(resudial_net, source, target)).size() > 0)
    {
        float min_flow = FLT_MAX;

        for (typename std::vector<T>::const_iterator vertex = path.begin(); vertex != path.end() - 1; ++vertex)
        {
            std::pair<T, T> pair = std::make_pair(*vertex, *(vertex + 1));
            float c_flow = resudialCapacity(g, flows, capacity, pair.first, pair.second); 
            min_flow = min(min_flow, c_flow);
        }
        
        for (typename std::vector<T>::const_iterator vertex = path.begin(); vertex != path.end() - 1; ++vertex)
        {
            std::pair<T, T> pair = std::make_pair(*vertex, *(vertex + 1));
            std::pair<T, T> rev_pair = std::make_pair(*(vertex + 1), *vertex);

            flows[pair] += min_flow;
            flows[rev_pair] = -flows[pair];

            if (resudialCapacity(g,
                                 flows, 
                                 capacity, 
                                 pair.first, 
                                 pair.second) > 0)
                resudial_net.AddEdge(pair.first, pair.second, 1);
            else
                resudial_net.deleteEdge(pair.first, pair.second);
       }
    }

    float flow = 0;

    for (typename graph<T>::const_iterator vertex = g.begin(); vertex != g.end(); ++vertex)
    {
        if (*vertex == source)
            continue;

        flow += flows[std::make_pair(source, *vertex)]; 
    }
    return flow;
}
