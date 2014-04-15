#include <list>

#include <NinjaParty/SpriteBatch.hpp>

#include <NinjaGui/Control.hpp>

namespace NinjaGui
{
    ControlId IControl::nextId = 0;
    
    struct IControl::impl
    {
        NinjaParty::Vector2 position;
        
        std::weak_ptr<IControl> parent;
        std::list<std::weak_ptr<IControl>> children;
    };
    
    IControl::IControl()
    : pimpl(new impl())
    {
    }
    
    IControl::~IControl()
    {
    }
    
    NinjaParty::Vector2& IControl::Position()
    {
        return pimpl->position;
    }
    
    const NinjaParty::Vector2 IControl::GetAbsolutePosition() const
    {
        auto p = pimpl->parent.lock();
        if(p != nullptr)
        {
            return p->GetAbsolutePosition() + pimpl->position;
        }
        
        return pimpl->position;
    }
    
    void IControl::SetParent(const std::weak_ptr<IControl> &control)
    {
        pimpl->parent = control;
    }

    void IControl::Unparent()
    {
        pimpl->position = GetAbsolutePosition();
        pimpl->parent.reset();
    }
    
    void IControl::AddChild(const std::weak_ptr<IControl> &control)
    {
        pimpl->children.push_back(control);
    }
    
    void IControl::RemoveChild(const std::weak_ptr<IControl> &control)
    {
        RemoveChild(control.lock().get());
    }
    
    void IControl::RemoveChild(IControl *control)
    {
        pimpl->children.remove_if([=](const std::weak_ptr<IControl> &c)
        {
            auto p = c.lock();
            return p.get() == control || p.get() == nullptr;
        });
    }
    
    void IControl::UnparentChildren()
    {
        for(auto &child : pimpl->children)
        {
            auto p = child.lock();
            if(p != nullptr)
                p->Unparent();
        }
    }
    
    void SetParentChild(const std::shared_ptr<IControl> &parent, const std::shared_ptr<IControl> &child)
    {
        parent->AddChild(child);
        child->SetParent(parent);
    }
}
