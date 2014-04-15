#ifndef NINJAGUI_BUTTON_HPP
#define NINJAGUI_BUTTON_HPP

#include <memory>

#include <NinjaParty/Event.hpp>
#include <NinjaParty/Math.hpp>
#include <NinjaParty/Texture.hpp>
#include <NinjaParty/TextureRegion.hpp>

#include "Control.hpp"

namespace NinjaGui
{
    class ButtonClickedEvent : public NinjaParty::Event<ButtonClickedEvent>
    {
    public:
        ButtonClickedEvent(int buttonId)
        : buttonId(buttonId)
        {
        }
        
        int ButtonId() const { return buttonId; }
        
    private:
        int buttonId;
    };
    
    class Button : public Control<Button>
    {
    public:
        Button(int buttonId,
               const NinjaParty::Vector2 &position,
               NinjaParty::Texture *texture,
               NinjaParty::TextureRegion upRegion,
               NinjaParty::TextureRegion downRegion);
        ~Button();

        virtual bool ProcessEvent(const std::shared_ptr<NinjaParty::IEvent> &event,
                                  const bool hasFocusIn,
                                  bool &hasFocusOut);
        
        virtual void Update(float deltaSeconds,
                            const bool hasFocusIn,
                            bool &hasFocusOut);
        
        virtual void Draw(NinjaParty::SpriteBatch *spriteBatch);
        
    private:
        struct impl;
        std::unique_ptr<impl> pimpl;
    };
}

#endif//NINJAGUI_BUTTON_HPP
