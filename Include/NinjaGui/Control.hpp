#ifndef NINJAGUI_CONTROL_HPP
#define NINJAGUI_CONTROL_HPP

#include <memory>
#include <stdint.h>

#include <NinjaParty/Math.hpp>

namespace NinjaParty
{
    struct IEvent;
    class SpriteBatch;
}

namespace NinjaGui
{
    typedef uint32_t ControlId;
    
    class IControl
    {
    public:
        IControl();
        virtual ~IControl();
        
        virtual ControlId GetId() = 0;

        virtual bool ProcessEvent(const std::shared_ptr<NinjaParty::IEvent> &event,
                                  const bool hasFocusIn,
                                  bool &hasFocusOut) { return false; }
        
        virtual void Update(float deltaSeconds,
                            const bool hasFocusIn,
                            bool &hasFocusOut) { }
        
        virtual void Draw(NinjaParty::SpriteBatch *spriteBatch) { }
        
        NinjaParty::Vector2& Position();
        
        const NinjaParty::Vector2 GetAbsolutePosition() const;
        
        void SetParent(const std::weak_ptr<IControl> &control);
        void Unparent();
        
        void AddChild(const std::weak_ptr<IControl> &control);
        void RemoveChild(const std::weak_ptr<IControl> &control);
        void RemoveChild(IControl *control);
        void UnparentChildren();
                
    protected:
        struct impl;
        std::unique_ptr<impl> pimpl;
        
        static ControlId nextId;
    };
    
    void SetParentChild(const std::shared_ptr<IControl> &parent, const std::shared_ptr<IControl> &child);
    
    template <typename T>
    class Control : public IControl
    {
    public:
        Control() { }
        virtual ~Control() { }
        
        static ControlId GetControlId()
        {
            static ControlId controldId = ++nextId;
            return controldId;
        }
        
        ControlId GetId()
        {
            return GetControlId();
        }
    };
}

#endif//NINJAGUI_CONTROL_HPP
