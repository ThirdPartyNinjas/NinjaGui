#include <NinjaParty/SpriteBatch.hpp>

#include <NinjaGui/Image.hpp>

namespace NinjaGui
{
    struct Image::impl
    {
        NinjaParty::Texture *texture;
        NinjaParty::TextureRegion textureRegion;
        
        float rotation = 0.0f;
        float scale = 1.0f;
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
    
    void Image::Change(NinjaParty::Texture *texture,
                       NinjaParty::TextureRegion textureRegion)
    {
        pimpl->texture = texture;
        pimpl->textureRegion = textureRegion;
    }
    
    float& Image::Rotation()
    {
        return pimpl->rotation;
    }
    
    float& Image::Scale()
    {
        return pimpl->scale;
    }
    
    void Image::Draw(NinjaParty::SpriteBatch *spriteBatch)
    {
        spriteBatch->Draw(pimpl->texture,
                          pimpl->textureRegion,
                          GetAbsolutePosition(),
                          NinjaParty::Vector2(0.5f),
                          pimpl->rotation,
                          NinjaParty::Color::White,
                          NinjaParty::Vector2(pimpl->scale));
    }
}
