#include "layer.h"
#include <algorithm>

namespace core {

Layer::Layer(const String& name)
    : m_name(name)
    , m_visible(true)
    , m_locked(false)
    , m_opacity(1.0f)
    , m_blendMode(BlendMode::Normal)
    , m_position(0, 0)
    , m_size(0, 0)
    , m_parent(nullptr)
{
}

Layer::~Layer()
{
}

void Layer::addChild(std::shared_ptr<Layer> child)
{
    if (child && child.get() != this) {
        child->setParent(this);
        m_children.push_back(child);
    }
}

void Layer::removeChild(std::shared_ptr<Layer> child)
{
    auto it = std::find(m_children.begin(), m_children.end(), child);
    if (it != m_children.end()) {
        (*it)->setParent(nullptr);
        m_children.erase(it);
    }
}

} // namespace core
