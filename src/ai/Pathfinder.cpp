#include "ai/Pathfinder.h"
#include <algorithm>

namespace pe::ai {

namespace {
inline int Heuristic(const glm::ivec2& a, const glm::ivec2& b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}
}

void Pathfinder::Init(const Promethean::CollisionLayer* collision)
{
    m_collision = collision;
    if (collision) {
        auto s = collision->GetSize();
        m_width = s.x;
        m_height = s.y;
        m_nodes.resize(static_cast<size_t>(m_width * m_height));
        m_heap.resize(static_cast<size_t>(m_width * m_height));
    }
}

static void HeapPush(std::vector<int>& heap, std::vector<Pathfinder::Node>& nodes, int idx)
{
    heap.push_back(idx);
    size_t i = heap.size() - 1;
    while (i > 0) {
        size_t p = (i - 1) / 2;
        if (nodes[heap[p]].f <= nodes[heap[i]].f) break;
        std::swap(heap[p], heap[i]);
        i = p;
    }
}

static int HeapPop(std::vector<int>& heap, std::vector<Pathfinder::Node>& nodes)
{
    int result = heap.front();
    heap[0] = heap.back();
    heap.pop_back();
    size_t i = 0;
    while (true) {
        size_t l = 2 * i + 1;
        size_t r = l + 1;
        if (l >= heap.size()) break;
        size_t smallest = l;
        if (r < heap.size() && nodes[heap[r]].f < nodes[heap[l]].f)
            smallest = r;
        if (nodes[heap[i]].f <= nodes[heap[smallest]].f) break;
        std::swap(heap[i], heap[smallest]);
        i = smallest;
    }
    return result;
}

PathResult Pathfinder::FindPath(const glm::ivec2& start, const glm::ivec2& goal) const
{
    PathResult res;
    if (!m_collision || !m_collision->IsWalkable(start) || !m_collision->IsWalkable(goal)) {
        res.error = PathError::InvalidEndpoint;
        return res;
    }
    std::fill(m_nodes.begin(), m_nodes.end(), Node{});
    m_heap.clear();

    auto index = [&](const glm::ivec2& p){ return p.y * m_width + p.x; };

    int startIdx = index(start);
    Node& sNode = m_nodes[startIdx];
    sNode.pos = start;
    sNode.g = 0;
    sNode.f = Heuristic(start, goal);
    HeapPush(m_heap, m_nodes, startIdx);
    sNode.inOpen = true;

    const glm::ivec2 dirs[4] = { {1,0}, {-1,0}, {0,1}, {0,-1} };

    while (!m_heap.empty()) {
        int currentIdx = HeapPop(m_heap, m_nodes);
        Node& current = m_nodes[currentIdx];
        current.inOpen = false;
        current.closed = true;

        if (current.pos == goal) {
            int idx = currentIdx;
            while (idx != -1) {
                res.cells.push_back(m_nodes[idx].pos);
                idx = m_nodes[idx].parent;
                if (res.cells.size() > 1024) break;
            }
            std::reverse(res.cells.begin(), res.cells.end());
            return res;
        }

        for (auto d : dirs) {
            glm::ivec2 np = current.pos + d;
            if (np.x < 0 || np.y < 0 || np.x >= m_width || np.y >= m_height)
                continue;
            if (!m_collision->IsWalkable(np))
                continue;
            int nidx = index(np);
            Node& n = m_nodes[nidx];
            if (n.closed) continue;
            int tentativeG = current.g + 1;
            if (!n.inOpen || tentativeG < n.g) {
                n.pos = np;
                n.g = tentativeG;
                n.f = tentativeG + Heuristic(np, goal);
                n.parent = currentIdx;
                if (!n.inOpen) {
                    HeapPush(m_heap, m_nodes, nidx);
                    n.inOpen = true;
                }
            }
        }
    }

    res.error = PathError::NoPath;
    return res;
}

} // namespace pe::ai
