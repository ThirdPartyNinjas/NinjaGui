#ifndef NINJAGUI_TEXTBUTTON_HPP
#define NINJAGUI_TEXTBUTTON_HPP

#include <memory>

#include <NinjaParty/Color.hpp>
#include <NinjaParty/Event.hpp>
#include <NinjaParty/Font.hpp>
#include <NinjaParty/Math.hpp>
#include <NinjaParty/Texture.hpp>

#include "Control.hpp"
#include "Events.hpp"

namespace NinjaGui
{
    class TextButton : public Control<TextButton>
    {
    public:
        TextButton(int buttonId,
                   const std::string &text,
                   const NinjaParty::Vector2 &position,
                   NinjaParty::Font *font,
                   NinjaParty::Texture *fontTexture,
                   NinjaParty::Color upColor,
                   NinjaParty::Color downColor,
                   NinjaParty::Color inactiveColor = NinjaParty::Color::Gray);
        ~TextButton();
        
        bool IsActive() const;
        void SetActive(bool active);
        
        bool IsEnabled() const;
        void SetEnabled(bool enabled);
        
        virtual bool ProcessEvent(const std::shared_ptr<NinjaParty::IEvent> &event,
                                  const bool hasFocusIn,
                                  bool &hasFocusOut);
        
        virtual void Update(float deltaSeconds,
                            const bool hasFocusIn,
                            bool &hasFocusOut);
        
        virtual void Draw(NinjaParty::SpriteBatch *spriteBatch);
        
        void SetText(const std::string &text);
        void SetUpColor(const NinjaParty::Color &color);
        void SetDownColor(const NinjaParty::Color &color);
        void SetInactiveColor(const NinjaParty::Color &color);
        
    private:
        struct impl;
        std::unique_ptr<impl> pimpl;
    };
}

#endif//NINJAGUI_TEXTBUTTON_HPP
