#include <iostream>
#include <vector>
#include <algorithm>

struct Segment
{
    Segment(const float &length): length(length) { }
    
    float length;
    
    size_t qtyOfBytes(float lengthOfByte) const
    {
        return length / lengthOfByte;
    }
    
    bool operator <(const Segment &rhs) const
    {
        return length < rhs.length;
    }
};

void readSegments(std::istream &is, std::vector<Segment> &segments, size_t &qtyOfBytes)
{
    size_t segmentsQty = 0;
    is >> segmentsQty;
    is >> qtyOfBytes;
    
    for (size_t i = 0; i < segmentsQty; i++)
    {
        float length;
        is >> length;
        segments.push_back(Segment(length));
    }
}

size_t partition(const std::vector<Segment> &s, float point)
{
    size_t qtyOfBytes = 0;
    for (std::vector<Segment>::const_reverse_iterator segment = s.rbegin(); segment != s.rend(); ++segment)
    {
        qtyOfBytes += segment->qtyOfBytes(point);
        if (qtyOfBytes == 0)
            break;
    }
    return qtyOfBytes;
}

int main()
{
    size_t qtyOfBytes = 0;
    std::vector<Segment> segments;
    
    readSegments(std::cin, segments, qtyOfBytes);
    std::sort(segments.begin(), segments.end());
    
    float leftPoint = 0, rightPoint = segments.back().length;
    
    size_t currentQty = 0;
    while ((int)rightPoint != (int)leftPoint)
    {
        currentQty = partition(segments, (rightPoint - leftPoint)/2 + leftPoint);
        if (qtyOfBytes <= currentQty)
            leftPoint += (rightPoint - leftPoint)/2;
        else
            rightPoint -= (rightPoint - leftPoint)/2;
    }
    
    std::cout << (int)leftPoint << std::endl;
    
    return 0;
}