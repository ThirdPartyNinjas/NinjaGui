#include <NinjaParty/SpriteBatch.hpp>
#include <NinjaParty/TouchEvents.hpp>

#include <NinjaGui/Button.hpp>

namespace NinjaGui
{
    enum class ButtonState
    {
        Up,
        Down,
    };
    
    struct Button::impl
    {
        int buttonId;
        ButtonState buttonState = ButtonState::Up;
        NinjaParty::TouchHandle activeTouchHandle;
        
        NinjaParty::Vector2 dimensions;
        
        bool isActive = true;
        bool isEnabled = true;
        
        NinjaParty::Texture *texture;
        NinjaParty::TextureRegion upRegion;
        NinjaParty::TextureRegion downRegion;
        NinjaParty::TextureRegion inactiveRegion;
    };
    
    Button::Button(int buttonId,
                   const NinjaParty::Vector2 &position,
                   NinjaParty::Texture *texture,
                   NinjaParty::TextureRegion upRegion,
                   NinjaParty::TextureRegion downRegion)
    : Button(buttonId, position, texture, upRegion, downRegion, upRegion)
    {
    }

    Button::Button(int buttonId,
                   const NinjaParty::Vector2 &position,
                   NinjaParty::Texture *texture,
                   NinjaParty::TextureRegion upRegion,
                   NinjaParty::TextureRegion downRegion,
                   NinjaParty::TextureRegion inactiveRegion)
    : pimpl(new impl())
    {
        Position() = position;

        pimpl->buttonId = buttonId;
        pimpl->dimensions = NinjaParty::Vector2(upRegion.bounds.width, upRegion.bounds.height);
        pimpl->texture = texture;
        pimpl->upRegion = upRegion;
        pimpl->downRegion = downRegion;
        pimpl->inactiveRegion = inactiveRegion;
    }
    
    Button::~Button()
    {
    }
    
    bool Button::IsActive() const
    {
        return pimpl->isActive;
    }
    
    void Button::SetActive(bool active)
    {
        pimpl->isActive = active;
        
        if(!active)
            pimpl->buttonState = ButtonState::Up;
    }

    bool Button::IsEnabled() const
    {
        return pimpl->isEnabled;
    }
    
    void Button::SetEnabled(bool enabled)
    {
        pimpl->isEnabled = enabled;

        if(!enabled)
            pimpl->buttonState = ButtonState::Up;
}

    bool Button::ProcessEvent(const std::shared_ptr<NinjaParty::IEvent> &event,
                              const bool hasFocusIn,
                              bool &hasFocusOut)
    {
        if(!pimpl->isEnabled || !pimpl->isActive)
            return false;
        
		NinjaParty::Vector2 absolutePosition = GetAbsolutePosition();
        
        switch(pimpl->buttonState)
        {
            case ButtonState::Up:
                if(event->GetId() == NinjaParty::TouchBeganEvent::GetEventId())
                {
                    auto e = dynamic_cast<NinjaParty::TouchBeganEvent*>(event.get());
                    bool touchInside = (e->X() >= absolutePosition.X() - pimpl->dimensions.X() / 2 &&
                                        e->X() <= absolutePosition.X() + pimpl->dimensions.X() / 2 &&
                                        e->Y() >= absolutePosition.Y() - pimpl->dimensions.Y() / 2 &&
                                        e->Y() <= absolutePosition.Y() + pimpl->dimensions.Y() / 2);
                    if(touchInside)
                    {
                        pimpl->buttonState = ButtonState::Down;
                        pimpl->activeTouchHandle = e->Handle();
                        hasFocusOut = true;
                        return true;
                    }
                }
                
                return false;
                
            case ButtonState::Down:
                if(event->GetId() == NinjaParty::TouchMovedEvent::GetEventId())
                {
                    auto e = dynamic_cast<NinjaParty::TouchMovedEvent*>(event.get());
                    if(pimpl->activeTouchHandle != e->Handle())
                        return false;
                    
                    bool touchInside = (e->X() >= absolutePosition.X() - pimpl->dimensions.X() / 2 &&
                                        e->X() <= absolutePosition.X() + pimpl->dimensions.X() / 2 &&
                                        e->Y() >= absolutePosition.Y() - pimpl->dimensions.Y() / 2 &&
                                        e->Y() <= absolutePosition.Y() + pimpl->dimensions.Y() / 2);
                    if(!touchInside)
                    {
                        pimpl->buttonState = ButtonState::Up;
                        hasFocusOut = false;
                    }
                    
                    return false;
                }
                if(event->GetId() == NinjaParty::TouchEndedEvent::GetEventId())
                {
                    auto e = dynamic_cast<NinjaParty::TouchEndedEvent*>(event.get());
                    if(pimpl->activeTouchHandle != e->Handle())
                        return false;
                    
                    bool touchInside = (e->X() >= absolutePosition.X() - pimpl->dimensions.X() / 2 &&
                                        e->X() <= absolutePosition.X() + pimpl->dimensions.X() / 2 &&
                                        e->Y() >= absolutePosition.Y() - pimpl->dimensions.Y() / 2 &&
                                        e->Y() <= absolutePosition.Y() + pimpl->dimensions.Y() / 2);
                    pimpl->buttonState = ButtonState::Up;
                    hasFocusOut = false;

                    if(touchInside)
                    {
                        NinjaParty::EventBroadcaster::Instance().Broadcast(std::make_shared<ButtonClickedEvent>(pimpl->buttonId));
                        return true;
                    }
                    
                    return false;
                }
                if(event->GetId() == NinjaParty::TouchCancelledEvent::GetEventId())
                {
                    auto e = dynamic_cast<NinjaParty::TouchCancelledEvent*>(event.get());
                    if(pimpl->activeTouchHandle == e->Handle())
                    {
                        pimpl->buttonState = ButtonState::Up;
                        hasFocusOut = false;
                    }
                    
                    return false;
                }
                
                return false;
        }
    }
    
    void Button::Update(float deltaSeconds,
                        const bool hasFocusIn,
                        bool &hasFocusOut)
    {
    }
    
    void Button::Draw(NinjaParty::SpriteBatch *spriteBatch)
    {
        NinjaParty::TextureRegion textureRegion;
        
        if(pimpl->isActive)
            textureRegion = pimpl->buttonState == ButtonState::Up ? pimpl->upRegion : pimpl->downRegion;
        else
            textureRegion = pimpl->inactiveRegion;
        
        spriteBatch->Draw(pimpl->texture,
                          textureRegion,
                          GetAbsolutePosition(),
                          NinjaParty::Vector2(0.5f));
    }
    
    void Button::SetUpRegion(const NinjaParty::TextureRegion &region)
    {
        pimpl->upRegion = region;
    }
    
    void Button::SetDownRegion(const NinjaParty::TextureRegion &region)
    {
        pimpl->downRegion = region;
    }
    
    void Button::SetInactiveRegion(const NinjaParty::TextureRegion &region)
    {
        pimpl->inactiveRegion = region;
    }
}