#include "pathfinding/AStar.h"
#include <algorithm>
#include <cmath>

namespace pe::pathfinding {

namespace {
inline int Heuristic(const glm::ivec2& a, const glm::ivec2& b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

inline int Index(int x, int y, int w) { return y * w + x; }
}

std::vector<glm::ivec2> FindPath(const Grid& grid, const glm::ivec2& start, const glm::ivec2& goal)
{
    std::vector<glm::ivec2> result;
    if(grid.empty() || grid[0].empty()) return result;
    int width = static_cast<int>(grid[0].size());
    int height = static_cast<int>(grid.size());
    auto valid = [&](const glm::ivec2& p){
        return p.x >=0 && p.y >=0 && p.x < width && p.y < height && grid[p.y][p.x] == 0;
    };
    if(!valid(start) || !valid(goal)) return result;

    std::vector<Node> nodes(static_cast<size_t>(width*height));
    std::vector<int> heap; heap.reserve(nodes.size());

    int startIdx = Index(start.x,start.y,width);
    Node& s = nodes[startIdx];
    s.pos = start; s.g = 0; s.f = Heuristic(start, goal); s.inOpen = true;
    heap.push_back(startIdx);

    const glm::ivec2 dirs[4] = {{1,0},{-1,0},{0,1},{0,-1}};

    auto heap_push = [&](int idx){
        heap.push_back(idx);
        size_t i = heap.size()-1;
        while(i>0){
            size_t p=(i-1)/2;
            if(nodes[heap[p]].f <= nodes[heap[i]].f) break;
            std::swap(heap[p],heap[i]);
            i=p;
        }
    };

    auto heap_pop = [&](){
        int idx = heap.front();
        heap[0] = heap.back();
        heap.pop_back();
        size_t i=0;
        while(true){
            size_t l=2*i+1, r=l+1;
            if(l>=heap.size()) break;
            size_t smallest=l;
            if(r<heap.size() && nodes[heap[r]].f < nodes[heap[l]].f) smallest=r;
            if(nodes[heap[i]].f <= nodes[heap[smallest]].f) break;
            std::swap(heap[i],heap[smallest]);
            i=smallest;
        }
        return idx;
    };

    while(!heap.empty()) {
        int currentIdx = heap_pop();
        Node& current = nodes[currentIdx];
        current.inOpen=false; current.closed=true;
        if(current.pos==goal){
            int idx=currentIdx;
            while(idx!=-1){
                result.push_back(nodes[idx].pos);
                idx=nodes[idx].parent;
                if(result.size()>1024) break;
            }
            std::reverse(result.begin(), result.end());
            return result;
        }
        for(auto d:dirs){
            glm::ivec2 np=current.pos+d;
            if(!valid(np)) continue;
            int ni=Index(np.x,np.y,width);
            Node& n=nodes[ni];
            if(n.closed) continue;
            int tentative = current.g + 1;
            if(!n.inOpen || tentative < n.g){
                n.pos=np; n.g=tentative; n.f=tentative+Heuristic(np,goal); n.parent=currentIdx;
                if(!n.inOpen){ heap_push(ni); n.inOpen=true; }
            }
        }
    }
    return result; // empty = no path
}

} // namespace pe::pathfinding
