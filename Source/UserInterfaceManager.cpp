#include <list>

#include <NinjaParty/Event.hpp>
#include <NinjaParty/SpriteBatch.hpp>
#include <NinjaParty/TouchEvents.hpp>

#include <NinjaGui/Control.hpp>
#include <NinjaGui/UserInterfaceManager.hpp>

namespace NinjaGui
{
    typedef std::pair<int, std::shared_ptr<IControl>> ControlPair;
    struct UserInterfaceManager::impl
    {
        std::list<ControlPair> controls;
        std::list<ControlPair> controlsToAdd;
        std::weak_ptr<IControl> focusControl;
        NinjaParty::SpriteBatch *spriteBatch;
        
        int screenWidth, screenHeight;
        int virtualScreenWidth, virtualScreenHeight;
        float renderScale;
        NinjaParty::Matrix3 renderTransform;
        
        void CalculateRenderScale()
        {
            if(virtualScreenWidth > screenWidth || virtualScreenHeight > screenHeight)
            {
                renderScale = std::min(screenWidth / static_cast<float>(virtualScreenWidth),
                                       screenHeight / static_cast<float>(virtualScreenHeight));
            }
            else if(virtualScreenWidth < screenWidth && virtualScreenHeight < screenHeight)
            {
                renderScale = std::min(screenWidth / static_cast<float>(virtualScreenWidth),
                                       screenHeight / static_cast<float>(virtualScreenHeight));
            }
            else
            {
                renderScale = 1.0f;
            }
            
            renderTransform = NinjaParty::CreateTranslationMatrix(screenWidth / 2, screenHeight / 2) *
                NinjaParty::CreateScaleMatrix(renderScale, renderScale) *
                NinjaParty::CreateTranslationMatrix(-virtualScreenWidth / 2, -virtualScreenHeight / 2);
        }
        
        void ScaleTouchPoint(int &x, int &y)
        {
            float scaledVirtualWidth = virtualScreenWidth * renderScale;
            float scaledVirtualHeight = virtualScreenHeight * renderScale;
            
            int offsetX = (scaledVirtualWidth - screenWidth) / 2;
            int offsetY = (scaledVirtualHeight - screenHeight) / 2;
            
            x = (x + offsetX) / renderScale;
            y = (y + offsetY) / renderScale;
        }
        
    };
    
    UserInterfaceManager::UserInterfaceManager(int screenWidth, int screenHeight, int virtualScreenWidth, int virtualScreenHeight)
    : pimpl(new impl())
    {
        pimpl->spriteBatch = new NinjaParty::SpriteBatch(screenWidth, screenHeight);
        pimpl->screenWidth = screenWidth;
        pimpl->screenHeight = screenHeight;
        pimpl->virtualScreenWidth = virtualScreenWidth;
        pimpl->virtualScreenHeight = virtualScreenHeight;
        pimpl->CalculateRenderScale();

        auto &eb = NinjaParty::EventBroadcaster::Instance();

        eb.AddEventReceiver(NinjaParty::TouchBeganEvent::GetEventId(), this);
        eb.AddEventReceiver(NinjaParty::TouchEndedEvent::GetEventId(), this);
        eb.AddEventReceiver(NinjaParty::TouchMovedEvent::GetEventId(), this);
        eb.AddEventReceiver(NinjaParty::TouchCancelledEvent::GetEventId(), this);
        // todo: add resolution change event
    }
    
    UserInterfaceManager::~UserInterfaceManager()
    {
        NinjaParty::EventBroadcaster::Instance().RemoveEventReceiver(this);
    }
    
    void UserInterfaceManager::Add(std::shared_ptr<IControl> control, int layer)
    {
        pimpl->controlsToAdd.push_back(std::make_pair(layer, control));
    }
    
    void UserInterfaceManager::Remove(IControl *control)
    {
        pimpl->controls.remove_if([=](const ControlPair &pair){ return pair.second.get() == control; });
    }
    
    void UserInterfaceManager::Remove(std::shared_ptr<IControl> control)
    {
        Remove(control.get());
    }
    
    bool UserInterfaceManager::Contains(IControl *control)
    {
        auto p = std::find_if(pimpl->controls.begin(),
                              pimpl->controls.end(),
                              [=](const ControlPair &pair){ return pair.second.get() == control; });
        return p != pimpl->controls.end();
    }
    
    void UserInterfaceManager::Update(float deltaSeconds)
    {
        if(!pimpl->controlsToAdd.empty())
        {
            pimpl->controls.merge(pimpl->controlsToAdd);
            pimpl->controls.sort([](const ControlPair &lhs, const ControlPair &rhs){ return lhs.first > rhs.first; });
        }
        
        for(auto &controlPair : pimpl->controls)
        {
            auto focusControl = pimpl->focusControl.lock();
            bool hasFocusOut;
            
            controlPair.second->Update(deltaSeconds,
                                focusControl == controlPair.second,
                                hasFocusOut);
            
            if(hasFocusOut)
                pimpl->focusControl = controlPair.second;
        }
    }
    
    void UserInterfaceManager::Draw()
    {
        pimpl->spriteBatch->Begin(NinjaParty::BlendMode::PremultipliedAlpha, pimpl->renderTransform);
        
        for(auto p = pimpl->controls.rbegin(); p != pimpl->controls.rend(); ++p)
        {
            p->second->Draw(pimpl->spriteBatch);
        }
        
        pimpl->spriteBatch->End();
    }
    
    void UserInterfaceManager::Receive(const std::shared_ptr<NinjaParty::IEvent> &event)
    {
        for(auto &controlPair : pimpl->controls)
        {
            using namespace NinjaParty;

            auto focusControl = pimpl->focusControl.lock();
            bool hasFocusOut;
            bool absorbedEvent;

            if(event->GetId() == TouchBeganEvent::GetEventId())
            {
                TouchBeganEvent *tbe = dynamic_cast<TouchBeganEvent*>(event.get());
                
                int scaledX = tbe->X();
                int scaledY = tbe->Y();
                pimpl->ScaleTouchPoint(scaledX, scaledY);
                
                auto scaledEvent = std::make_shared<TouchBeganEvent>(tbe->Handle(), scaledX, scaledY, tbe->TapCount());
                
                absorbedEvent = controlPair.second->ProcessEvent(scaledEvent,
                                                                 focusControl == controlPair.second,
                                                                 hasFocusOut);
            }
            else if(event->GetId() == TouchEndedEvent::GetEventId())
            {
                TouchEndedEvent *tee = dynamic_cast<TouchEndedEvent*>(event.get());
                
                int scaledX = tee->X();
                int scaledY = tee->Y();
                pimpl->ScaleTouchPoint(scaledX, scaledY);
                
                auto scaledEvent = std::make_shared<TouchEndedEvent>(tee->Handle(), scaledX, scaledY);
                
                absorbedEvent = controlPair.second->ProcessEvent(scaledEvent,
                                                                 focusControl == controlPair.second,
                                                                 hasFocusOut);
            }
            else if(event->GetId() == TouchMovedEvent::GetEventId())
            {
                TouchMovedEvent *tme = dynamic_cast<TouchMovedEvent*>(event.get());
                
                int scaledX = tme->X();
                int scaledY = tme->Y();
                pimpl->ScaleTouchPoint(scaledX, scaledY);
                
                auto scaledEvent = std::make_shared<TouchMovedEvent>(tme->Handle(), scaledX, scaledY);
                
                absorbedEvent = controlPair.second->ProcessEvent(scaledEvent,
                                                                 focusControl == controlPair.second,
                                                                 hasFocusOut);
            }
            else
            {
                absorbedEvent = controlPair.second->ProcessEvent(event,
                                                                 focusControl == controlPair.second,
                                                                 hasFocusOut);
            }
            
            if(hasFocusOut)
                pimpl->focusControl = controlPair.second;
            
            if(absorbedEvent)
                return;
        }
    }

    int UserInterfaceManager::GetScreenTop() const
    {
        return (pimpl->virtualScreenHeight - pimpl->screenHeight / pimpl->renderScale) / 2;
    }

    int UserInterfaceManager::GetScreenBottom() const
    {
        return (pimpl->virtualScreenHeight + pimpl->screenHeight / pimpl->renderScale) / 2;
    }
    
    int UserInterfaceManager::GetScreenLeft() const
    {
        return (pimpl->virtualScreenWidth - pimpl->screenWidth / pimpl->renderScale) / 2;
    }
    
    int UserInterfaceManager::GetScreenRight() const
    {
        return (pimpl->virtualScreenWidth + pimpl->screenWidth / pimpl->renderScale) / 2;
    }
}
