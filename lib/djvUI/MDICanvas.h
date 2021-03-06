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

#pragma once

#include <djvUI/Widget.h>

namespace djv
{
    namespace UI
    {
        //! This namespace provides multiple document interface (MDI) functionality.
        namespace MDI
        {
            class IWidget;

            //! This class provides a MDI canvas widget.
            class Canvas : public Widget
            {
                DJV_NON_COPYABLE(Canvas);

            protected:
                void _init(const std::shared_ptr<Core::Context>&);
                Canvas();

            public:
                virtual ~Canvas();

                static std::shared_ptr<Canvas> create(const std::shared_ptr<Core::Context>&);

                const glm::vec2 & getCanvasSize() const;
                void setCanvasSize(const glm::vec2 &);

                const std::shared_ptr<IWidget> & getActiveWidget() const;
                void nextWidget();
                void prevWidget();
                void setActiveCallback(const std::function<void(const std::shared_ptr<IWidget> &)> &);

                glm::vec2 getWidgetPos(const std::shared_ptr<IWidget> &) const;
                //! \todo We should convert widgets to use local coordinates and then
                //! we could remove this and use Widget::move() instead.
                void setWidgetPos(const std::shared_ptr<IWidget>&, const glm::vec2&);
                void setWidgetGeometry(const std::shared_ptr<IWidget>&, const Core::BBox2f&);

                bool isMaximized() const;
                void setMaximize(bool);
                void setMaximizeCallback(const std::function<void(bool)>&);

            protected:
                void _preLayoutEvent(Core::Event::PreLayout &) override;
                void _layoutEvent(Core::Event::Layout &) override;
                void _paintOverlayEvent(Core::Event::PaintOverlay&) override;

                void _childAddedEvent(Core::Event::ChildAdded &) override;
                void _childRemovedEvent(Core::Event::ChildRemoved &) override;
                void _childOrderEvent(Core::Event::ChildOrder &) override;
                bool _eventFilter(const std::shared_ptr<Core::IObject> &, Core::Event::Event &) override;

            private:
                void _doActiveCallback();

                DJV_PRIVATE();
            };

        } // namespace MDI
    } // namespace UI
} // namespace djv
