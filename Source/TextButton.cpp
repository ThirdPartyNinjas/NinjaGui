#include <NinjaParty/SpriteBatch.hpp>
#include <NinjaParty/TouchEvents.hpp>

#include <NinjaGui/TextButton.hpp>

namespace NinjaGui
{
    enum class ButtonState
    {
        Up,
        Down,
    };
    
    struct TextButton::impl
    {
        int buttonId;
        std::string text;
        
        ButtonState buttonState = ButtonState::Up;
        NinjaParty::TouchHandle activeTouchHandle;
        
        NinjaParty::Vector2 dimensions;
        
        bool isActive = true;
        bool isEnabled = true;
        
        NinjaParty::Font *font;
        NinjaParty::Texture *fontTexture;

        NinjaParty::Color upColor;
        NinjaParty::Color downColor;
        NinjaParty::Color inactiveColor;
    };
    
    TextButton::TextButton(int buttonId,
                           const std::string &text,
                           const NinjaParty::Vector2 &position,
                           NinjaParty::Font *font,
                           NinjaParty::Texture *fontTexture,
                           NinjaParty::Color upColor,
                           NinjaParty::Color downColor,
                           NinjaParty::Color inactiveColor)
    : pimpl(new impl())
    {
        Position() = position;
        
        pimpl->buttonId = buttonId;
        pimpl->text = text;
        pimpl->dimensions = NinjaParty::Vector2(NinjaParty::MeasureString(font, text));
        pimpl->font = font;
        pimpl->fontTexture = fontTexture;
        pimpl->upColor = upColor;
        pimpl->downColor = downColor;
        pimpl->inactiveColor = inactiveColor;
    }
    
    TextButton::~TextButton()
    {
    }
    
    bool TextButton::IsActive() const
    {
        return pimpl->isActive;
    }
    
    void TextButton::SetActive(bool active)
    {
        pimpl->isActive = active;
        
        if(!active)
            pimpl->buttonState = ButtonState::Up;
    }
    
    bool TextButton::IsEnabled() const
    {
        return pimpl->isEnabled;
    }
    
    void TextButton::SetEnabled(bool enabled)
    {
        pimpl->isEnabled = enabled;
        
        if(!enabled)
            pimpl->buttonState = ButtonState::Up;
    }
    
    bool TextButton::ProcessEvent(const std::shared_ptr<NinjaParty::IEvent> &event,
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
    
    void TextButton::Update(float deltaSeconds,
                        const bool hasFocusIn,
                        bool &hasFocusOut)
    {
    }
    
    void TextButton::Draw(NinjaParty::SpriteBatch *spriteBatch)
    {
        NinjaParty::Color color;
        
        if(pimpl->isActive)
            color = pimpl->buttonState == ButtonState::Up ? pimpl->upColor : pimpl->downColor;
        else
            color = pimpl->inactiveColor;
        
		spriteBatch->DrawString(pimpl->font,
                                pimpl->fontTexture,
                                pimpl->text,
                                Position(),
                                NinjaParty::Vector2(0.5f),
                                0.0f,
                                color,
                                NinjaParty::Vector2(1.0f));
    }
    
    void TextButton::SetUpColor(const NinjaParty::Color &color)
    {
        pimpl->upColor = color;
    }
    
    void TextButton::SetDownColor(const NinjaParty::Color &color)
    {
        pimpl->downColor = color;
    }
    
    void TextButton::SetInactiveColor(const NinjaParty::Color &color)
    {
        pimpl->inactiveColor = color;
    }
}