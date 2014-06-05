#ifndef NINJAGUI_BUTTON_HPP
#define NINJAGUI_BUTTON_HPP

#include <memory>

#include <NinjaParty/Event.hpp>
#include <NinjaParty/Math.hpp>
#include <NinjaParty/Texture.hpp>
#include <NinjaParty/TextureRegion.hpp>

#include "Control.hpp"
#include "Events.hpp"

namespace NinjaGui
{
    class Button : public Control<Button>
    {
    public:
        Button(int buttonId,
               const NinjaParty::Vector2 &position,
               NinjaParty::Texture *texture,
               NinjaParty::TextureRegion upRegion,
               NinjaParty::TextureRegion downRegion);
        Button(int buttonId,
               const NinjaParty::Vector2 &position,
               NinjaParty::Texture *texture,
               NinjaParty::TextureRegion upRegion,
               NinjaParty::TextureRegion downRegion,
               NinjaParty::TextureRegion inactiveRegion);
        ~Button();
        
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
        
        void SetUpRegion(const NinjaParty::TextureRegion &region);
        void SetDownRegion(const NinjaParty::TextureRegion &region);
        void SetInactiveRegion(const NinjaParty::TextureRegion &region);
        
    private:
        struct impl;
        std::unique_ptr<impl> pimpl;
    };
}

#endif//NINJAGUI_BUTTON_HPP
