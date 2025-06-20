#include "ui/HorizontalLayout.h"

void HorizontalLayout::AddChild(Widget* child)
{
    m_children.push_back(child);
    Recompute();
}

void HorizontalLayout::SetPosition(const glm::vec2& pos)
{
    m_position = pos;
    Recompute();
}

void HorizontalLayout::SetSize(const glm::vec2& size)
{
    m_size = size;
    Recompute();
}

void HorizontalLayout::Draw(BatchRenderer& renderer)
{
    assert(!m_children.empty() && "Layout has no children");
    WidgetContainer::Draw(renderer);
}

void HorizontalLayout::Recompute()
{
    if(m_children.empty())
        return;

    float width = (m_size.x - m_spacing * (m_children.size() - 1)) / m_children.size();
    float x = m_position.x;
    for(auto* c : m_children)
    {
        c->SetPosition({x, m_position.y});
        c->SetSize({width, m_size.y});
        x += width + m_spacing;
    }
}
