#include <NinjaParty/SpriteBatch.hpp>

#include <NinjaGui/Label.hpp>

namespace NinjaGui
{
    struct Label::impl
    {
        std::string text;
        NinjaParty::Font *font;
        NinjaParty::Texture *fontTexture;
        NinjaParty::Color color;
        NinjaParty::Vector2 origin;
    };
    
    Label::Label(const NinjaParty::Vector2 &position,
                 const std::string &text,
                 NinjaParty::Font *font,
                 NinjaParty::Texture *fontTexture,
                 const NinjaParty::Color &color,
                 const NinjaParty::Vector2 &origin)
    : pimpl(new impl())
    {
        Position() = position;
        
        pimpl->text = text;
        pimpl->font = font;
        pimpl->fontTexture = fontTexture;
        pimpl->color = color;
        pimpl->origin = origin;
    }
    
    Label::~Label()
    {
    }
    
    void Label::Draw(NinjaParty::SpriteBatch *spriteBatch)
    {
        spriteBatch->DrawString(pimpl->font,
                                pimpl->fontTexture,
                                pimpl->text,
                                GetAbsolutePosition(),
                                pimpl->origin,
                                0,
                                pimpl->color);
    }
    
    std::string& Label::Text()
    {
        return pimpl->text;
    }
}
