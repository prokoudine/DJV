//------------------------------------------------------------------------------
// Copyright (c) 2004-2019 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the names of the copyright holders nor the names of any
//   contributors may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

#include <djvCore/IEventSystem.h>

#include <djvCore/Context.h>
#include <djvCore/Event.h>
#include <djvCore/IObject.h>
#include <djvCore/TextSystem.h>
#include <djvCore/Timer.h>

#include <map>

using namespace djv::Core;

namespace djv
{
    namespace Core
    {
        namespace Event
        {
            namespace
            {
                class RootObject : public IObject
                {
                protected:
                    RootObject()
                    {}

                public:
                    ~RootObject() override
                    {}

                    static std::shared_ptr<RootObject> create(const std::shared_ptr<Context>& context)
                    {
                        auto out = std::shared_ptr<RootObject>(new RootObject);
                        out->_init(context);
                        out->setClassName("djv::Core::Context::RootObject");
                        return out;
                    }
                };

            } // namespace

            struct IEventSystem::Private
            {
                std::vector<std::shared_ptr<IObject> > objectsCreated;
                std::shared_ptr<RootObject> rootObject;
                std::weak_ptr<TextSystem> textSystem;
                std::chrono::steady_clock::time_point t;
                PointerInfo pointerInfo;
                std::shared_ptr<ValueSubject<PointerInfo> > pointerSubject;
                std::shared_ptr<ValueSubject<std::shared_ptr<IObject> > > hover;
                std::shared_ptr<ValueSubject<std::shared_ptr<IObject> > > grab;
                std::shared_ptr<ValueSubject<std::shared_ptr<IObject> > > keyGrab;
                std::weak_ptr<IObject> textFocus;
                bool init = false;
                std::shared_ptr<ValueObserver<std::string> > localeObserver;
                std::shared_ptr<ValueObserver<bool> > textChangedObserver;
                std::shared_ptr<Time::Timer> statsTimer;
            };

            void IEventSystem::_init(const std::string & systemName, const std::shared_ptr<Context>& context)
            {
                ISystem::_init(systemName, context);

                DJV_PRIVATE_PTR();

                p.rootObject = RootObject::create(context);
                p.pointerSubject = ValueSubject<PointerInfo>::create();
                p.hover = ValueSubject<std::shared_ptr<IObject> >::create();
                p.grab = ValueSubject<std::shared_ptr<IObject> >::create();
                p.keyGrab = ValueSubject<std::shared_ptr<IObject> >::create();

                auto weak = std::weak_ptr<IEventSystem>(std::dynamic_pointer_cast<IEventSystem>(shared_from_this()));
                p.textSystem = context->getSystemT<TextSystem>();
                if (auto textSystem = p.textSystem.lock())
                {
                    p.localeObserver = ValueObserver<std::string>::create(
                        textSystem->observeCurrentLocale(),
                        [weak](const std::string& value)
                        {
                            if (auto system = weak.lock())
                            {
                                system->_p->init = true;
                            }
                        });
                    p.textChangedObserver = ValueObserver<bool>::create(
                        textSystem->observeTextChanged(),
                        [weak](bool)
                        {
                            if (auto system = weak.lock())
                            {
                                system->_p->init = true;
                            }
                        });
                }

                p.statsTimer = Time::Timer::create(context);
                p.statsTimer->setRepeating(true);
                p.statsTimer->start(
                    Time::getTime(Time::TimerValue::VerySlow),
                    [weak](const std::chrono::steady_clock::time_point&, const Time::Unit&)
                {
                    if (auto system = weak.lock())
                    {
                        std::stringstream ss;
                        ss << "Global object count: " << IObject::getGlobalObjectCount();
                        system->_log(ss.str());
                    }
                });
            }

            IEventSystem::IEventSystem() :
                _p(new Private)
            {}

            IEventSystem::~IEventSystem()
            {}

            std::shared_ptr<IObject> IEventSystem::getRootObject() const
            {
                return _p->rootObject;
            }

            std::shared_ptr<Core::IValueSubject<PointerInfo> > IEventSystem::observePointer() const
            {
                return _p->pointerSubject;
            }

            std::shared_ptr<Core::IValueSubject<std::shared_ptr<IObject> > > IEventSystem::observeHover() const
            {
                return _p->hover;
            }

            std::shared_ptr<Core::IValueSubject<std::shared_ptr<IObject> > > IEventSystem::observeGrab() const
            {
                return _p->grab;
            }

            std::shared_ptr<Core::IValueSubject<std::shared_ptr<IObject> > > IEventSystem::observeKeyGrab() const
            {
                return _p->keyGrab;
            }

            const std::weak_ptr<IObject> & IEventSystem::getTextFocus() const
            {
                return _p->textFocus;
            }

            void IEventSystem::setTextFocus(const std::shared_ptr<IObject> & value)
            {
                DJV_PRIVATE_PTR();
                if (value == p.textFocus.lock())
                    return;
                auto prev = p.textFocus;
                p.textFocus = value;
                if (auto textFocus = prev.lock())
                {
                    TextFocusLost event;
                    textFocus->event(event);
                }
                if (auto textFocus = p.textFocus.lock())
                {
                    TextFocus event;
                    textFocus->event(event);
                }
            }

            void IEventSystem::setClipboard(const std::string&)
            {
                // Default implementation does nothing.
            }

            std::string IEventSystem::getClipboard() const
            {
                return std::string();
            }

            void IEventSystem::tick(const std::chrono::steady_clock::time_point&, const Time::Unit& dt)
            {
                DJV_PRIVATE_PTR();
                p.t += dt;

                auto objectsCreated = std::move(p.objectsCreated);
                for (const auto & i : objectsCreated)
                {
                    _initObject(i);
                }

                if (p.init)
                {
                    p.init = false;
                    Init event;
                    _initRecursive(p.rootObject, event);
                }

                Update updateEvent(p.t, dt);
                _updateRecursive(p.rootObject, updateEvent);

                PointerMove moveEvent(p.pointerInfo);
                if (auto grab = p.grab->get())
                {
                    /*{
                        std::stringstream ss;
                        ss << "Grab: " << p.grab->getClassName();
                        _log(ss.str());
                    }*/
                    grab->event(moveEvent);
                    if (!moveEvent.isAccepted())
                    {
                        // Release the grabbed object if it did not accept the move event.
                        auto object = grab;
                        p.grab->setIfChanged(nullptr);
                        moveEvent.reject();

                        // See if a parent wants the event.
                        for (auto parent = object->getParent().lock(); parent; parent = parent->getParent().lock())
                        {
                            parent->event(moveEvent);
                            if (moveEvent.isAccepted())
                            {
                                _setHover(parent);
                                auto hover = p.hover->get();
                                if (hover && hover->isEnabled(true))
                                {
                                    auto info = p.pointerInfo;
                                    info.buttons[info.id] = true;
                                    ButtonPress buttonPressEvent(info);
                                    hover->event(buttonPressEvent);
                                    if (buttonPressEvent.isAccepted())
                                    {
                                        p.grab->setIfChanged(hover);
                                    }
                                }
                                break;
                            }
                        }

                        // If none of the parents wanted the event, see if the original
                        // object wants it back.
                        if (!moveEvent.isAccepted())
                        {
                            object->event(moveEvent);
                            if (moveEvent.isAccepted())
                            {
                                _setHover(object);
                            }
                        }
                    }
                }
                else
                {
                    std::shared_ptr<IObject> hover;
                    _hover(moveEvent, hover);
                    /*if (hover)
                    {
                        size_t indent = 0;
                        auto widget = hover;
                        while (widget)
                        {
                            std::stringstream ss;
                            ss << "Hover: " << std::string(indent, ' ') << widget->getClassName();
                            _log(ss.str());
                            indent += 2;
                            widget = widget->getParent().lock();
                        }
                    }*/
                    _setHover(hover);
                }
            }

            void IEventSystem::_initRecursive(const std::shared_ptr<IObject> & object, Init & event)
            {
                object->event(event);
                for (const auto & child : object->_children)
                {
                    _initRecursive(child, event);
                }
            }

            void IEventSystem::_pointerMove(const PointerInfo & info)
            {
                DJV_PRIVATE_PTR();
                p.pointerInfo = info;
                p.pointerSubject->setIfChanged(info);
            }

            void IEventSystem::_buttonPress(int button)
            {
                DJV_PRIVATE_PTR();
                auto info = p.pointerInfo;
                info.buttons[button] = true;
                ButtonPress event(info);
                if (auto grab = p.grab->get())
                {
                    grab->event(event);
                }
                else
                {
                    auto object = p.hover->get();
                    while (object)
                    {
                        if (object->isEnabled(true))
                        {
                            object->event(event);
                            if (event.isAccepted())
                            {
                                p.grab->setIfChanged(object);
                                break;
                            }
                        }
                        object = object->getParent().lock();
                    }
                }
            }

            void IEventSystem::_buttonRelease(int button)
            {
                DJV_PRIVATE_PTR();
                auto info = p.pointerInfo;
                info.buttons[button] = false;
                ButtonRelease event(info);
                if (auto grab = p.grab->get())
                {
                    grab->event(event);
                    bool pressed = false;
                    for (const auto& i : info.buttons)
                    {
                        pressed |= i.second;
                    }
                    if (!pressed)
                    {
                        p.grab->setIfChanged(nullptr);
                    }
                }
            }

            void IEventSystem::_drop(const std::vector<std::string> & list)
            {
                DJV_PRIVATE_PTR();
                if (p.hover)
                {
                    Drop event(list, p.pointerInfo);
                    auto object = p.hover->get();
                    while (object)
                    {
                        if (object->isEnabled(true))
                        {
                            object->event(event);
                            if (event.isAccepted())
                            {
                                break;
                            }
                        }
                        object = object->getParent().lock();
                    }
                }
            }

            void IEventSystem::_keyPress(int key, int modifiers)
            {
                DJV_PRIVATE_PTR();
                auto textFocus = p.textFocus.lock();
                auto hover = p.hover->get();
                if (textFocus || hover)
                {
                    KeyPress event(key, modifiers, p.pointerInfo);
                    auto object = textFocus ? textFocus : hover;
                    while (object)
                    {
                        if (object->isEnabled(true))
                        {
                            object->event(event);
                            if (event.isAccepted())
                            {
                                p.keyGrab->setIfChanged(object);
                                break;
                            }
                        }
                        object = object->getParent().lock();
                    }
                }
            }

            void IEventSystem::_keyRelease(int key, int modifiers)
            {
                DJV_PRIVATE_PTR();
                KeyRelease event(key, modifiers, p.pointerInfo);
                if (auto keyGrab = p.keyGrab->get())
                {
                    keyGrab->event(event);
                    p.keyGrab->setIfChanged(nullptr);
                }
            }

            void IEventSystem::_text(const std::basic_string<djv_char_t>& utf32, int modifiers)
            {
                DJV_PRIVATE_PTR();
                if (auto textFocus = p.textFocus.lock())
                {
                    TextInput event(utf32, modifiers);
                    textFocus->event(event);
                }
            }
            
            void IEventSystem::_scroll(float x, float y)
            {
                DJV_PRIVATE_PTR();
                auto textFocus = p.textFocus.lock();
                if (textFocus || p.hover)
                {
                    Scroll event(glm::vec2(x, y), p.pointerInfo);
                    auto object = textFocus ? textFocus : p.hover->get();
                    while (object)
                    {
                        if (object->isEnabled(true))
                        {
                            object->event(event);
                            if (event.isAccepted())
                            {
                                break;
                            }
                        }
                        object = object->getParent().lock();
                    }
                }
            }

            void IEventSystem::_objectCreated(const std::shared_ptr<IObject> & object)
            {
                _p->objectsCreated.push_back(object);
            }

            void IEventSystem::_updateRecursive(const std::shared_ptr<IObject> & object, Update & event)
            {
                object->event(event);
                const auto children = object->_children;
                for (const auto & child : children)
                {
                    child->_parentsEnabled = object->_enabled && object->_parentsEnabled;
                    _updateRecursive(child, event);
                }
            }

            void IEventSystem::_setHover(const std::shared_ptr<IObject> & value)
            {
                DJV_PRIVATE_PTR();
                auto hoverPrev = p.hover->get();
                if (p.hover->setIfChanged(value))
                {
                    if (hoverPrev)
                    {
                        PointerLeave leaveEvent(p.pointerInfo);
                        hoverPrev->event(leaveEvent);
                    }
                    if (value)
                    {
                        PointerEnter enterEvent(p.pointerInfo);
                        value->event(enterEvent);
                    }
                }
            }

        } // namespace Event
    } // namespace Core
} // namespace djv

