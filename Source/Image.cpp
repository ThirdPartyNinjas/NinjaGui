#include <NinjaParty/SpriteBatch.hpp>

#include <NinjaGui/Image.hpp>

namespace NinjaGui
{
    struct Image::impl
    {
        NinjaParty::Texture *texture;
        NinjaParty::TextureRegion textureRegion;
    };
    
    Image::Image(const NinjaParty::Vector2 &position,
                 NinjaParty::Texture *texture,
                 NinjaParty::TextureRegion textureRegion)
    : pimpl(new impl())
    {
        Position() = position;
        
        pimpl->texture = texture;
        pimpl->textureRegion = textureRegion;
    }
    
    Image::~Image()
    {
    }
    
    void Image::Draw(NinjaParty::SpriteBatch *spriteBatch)
    {
        spriteBatch->Draw(pimpl->texture,
                          pimpl->textureRegion,
                          GetAbsolutePosition(),
                          NinjaParty::Vector2(0.5f));
    }
}
