#ifndef NINJAGUI_USERINTERFACEMANAGER_HPP
#define NINJAGUI_USERINTERFACEMANAGER_HPP

#include <memory>

#include <NinjaParty/Event.hpp>

namespace NinjaParty
{
    class SpriteBatch;
}

namespace NinjaGui
{
    class IControl;
    
    class UserInterfaceManager : public NinjaParty::IEventReceiver
    {
    public:
        UserInterfaceManager(int screenWidth, int screenHeight, int virtualScreenWidth, int virtualScreenHeight);
        ~UserInterfaceManager();
        
        void Add(std::shared_ptr<IControl> control, int layer = 0);
        void Remove(IControl *control);
        void Remove(std::shared_ptr<IControl> control);
        
        bool Contains(IControl *control);
        
        void Update(float deltaSeconds);
        void Draw();
        
        int GetScreenTop() const;
        int GetScreenBottom() const;
        int GetScreenLeft() const;
        int GetScreenRight() const;
        
        virtual void Receive(const std::shared_ptr<NinjaParty::IEvent> &event);
        
    private:
        struct impl;
        std::unique_ptr<impl> pimpl;
    };
}

#endif//NINJAGUI_USERINTERFACEMANAGER_HPP
