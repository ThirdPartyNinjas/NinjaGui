#ifndef NINJAGUI_NULLCONTROL_HPP
#define NINJAGUI_NULLCONTROL_HPP

#include "Control.hpp"

// This class exists as a concrete empty Control that we can add children to
// but doesn't actually render anything itself
namespace NinjaGui
{
    class NullControl : public Control<NullControl>
    {
    public:
        NullControl()
        {
        }
        
        NullControl(const NinjaParty::Vector2 &position)
        {
            Position() = position;
        }
        
        ~NullControl()
        {
        }
    };
}

#endif//NINJAGUI_NULLCONTROL_HPP
