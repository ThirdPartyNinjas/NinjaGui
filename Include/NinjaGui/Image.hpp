#ifndef NINJAGUI_IMAGE_HPP
#define NINJAGUI_IMAGE_HPP

#include <memory>

#include "Control.hpp"

namespace NinjaGui
{
    class Image : public Control<Image>
    {
    public:
        Image(const NinjaParty::Vector2 &position,
              NinjaParty::Texture *texture,
              NinjaParty::TextureRegion textureRegion);
        ~Image();
        
        void Change(NinjaParty::Texture *texture,
                    NinjaParty::TextureRegion textureRegion);
        
        float& Rotation();
        float& Scale();
        
        virtual void Draw(NinjaParty::SpriteBatch *spriteBatch);
        
    private:
        struct impl;
        std::unique_ptr<impl> pimpl;
    };
}

#endif//NINJAGUI_IMAGE_HPP
