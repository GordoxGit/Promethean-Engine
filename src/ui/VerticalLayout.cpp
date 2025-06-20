#include "ui/VerticalLayout.h"

void VerticalLayout::AddChild(Widget* child)
{
    m_children.push_back(child);
    Recompute();
}

void VerticalLayout::SetPosition(const glm::vec2& pos)
{
    m_position = pos;
    Recompute();
}

void VerticalLayout::SetSize(const glm::vec2& size)
{
    m_size = size;
    Recompute();
}

void VerticalLayout::Draw(BatchRenderer& renderer)
{
    assert(!m_children.empty() && "Layout has no children");
    WidgetContainer::Draw(renderer);
}

void VerticalLayout::Recompute()
{
    if(m_children.empty())
        return;

    float height = (m_size.y - m_spacing * (m_children.size() - 1)) / m_children.size();
    float y = m_position.y;
    for(auto* c : m_children)
    {
        c->SetPosition({m_position.x, y});
        c->SetSize({m_size.x, height});
        y += height + m_spacing;
    }
}
