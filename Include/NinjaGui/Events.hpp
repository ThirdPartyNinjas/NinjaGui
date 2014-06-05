#ifndef NINJAGUI_EVENTS_HPP
#define NINJAGUI_EVENTS_HPP

#include <NinjaParty/Event.hpp>

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
}

#endif//NINJAGUI_EVENTS_HPP