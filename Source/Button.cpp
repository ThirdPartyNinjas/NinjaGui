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
        
        NinjaParty::Texture *texture;
        NinjaParty::TextureRegion upRegion;
        NinjaParty::TextureRegion downRegion;
    };
    
    Button::Button(int buttonId,
                   const NinjaParty::Vector2 &position,
                   NinjaParty::Texture *texture,
                   NinjaParty::TextureRegion upRegion,
                   NinjaParty::TextureRegion downRegion)
    : pimpl(new impl())
    {
        Position() = position;

        pimpl->buttonId = buttonId;
        pimpl->dimensions = NinjaParty::Vector2(upRegion.bounds.width, upRegion.bounds.height);
        pimpl->texture = texture;
        pimpl->upRegion = upRegion;
        pimpl->downRegion = downRegion;
    }
    
    Button::~Button()
    {
    }
    
    bool Button::ProcessEvent(const std::shared_ptr<NinjaParty::IEvent> &event,
                              const bool hasFocusIn,
                              bool &hasFocusOut)
    {
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
        spriteBatch->Draw(pimpl->texture,
                          pimpl->buttonState == ButtonState::Up ? pimpl->upRegion : pimpl->downRegion,
                          GetAbsolutePosition(),
                          NinjaParty::Vector2(0.5f));
    }
}