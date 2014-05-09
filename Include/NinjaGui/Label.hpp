#ifndef NINJAGUI_LABEL_HPP
#define NINJAGUI_LABEL_HPP

#include <memory>
#include <string>

#include <NinjaParty/Color.hpp>
#include <NinjaParty/Font.hpp>
#include <NinjaParty/Texture.hpp>

#include "Control.hpp"

namespace NinjaGui
{
    class Label : public Control<Label>
    {
    public:
        Label(const NinjaParty::Vector2 &position,
              const std::string &text,
              NinjaParty::Font *font,
              NinjaParty::Texture *fontTexture,
              const NinjaParty::Color &color,
              const NinjaParty::Vector2 &origin = NinjaParty::Vector2::ZERO);
        ~Label();
        
        std::string& Text();
        
        NinjaParty::Color& Color();
        
        virtual void Draw(NinjaParty::SpriteBatch *spriteBatch);

    private:
        struct impl;
        std::unique_ptr<impl> pimpl;
    };
}

#endif//NINJAGUI_LABEL_HPP
