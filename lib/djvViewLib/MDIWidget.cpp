//------------------------------------------------------------------------------
// Copyright (c) 2018 Darby Johnston
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

#include <djvViewLib/MDIWidget.h>

#include <djvViewLib/ImageView.h>
#include <djvViewLib/MediaWidget.h>
#include <djvViewLib/PlaybackWidget.h>
#include <djvViewLib/TimelineSlider.h>

#include <djvUI/Border.h>
#include <djvUI/Label.h>
#include <djvUI/RowLayout.h>
#include <djvUI/StackLayout.h>
#include <djvUI/ToolButton.h>

#include <djvCore/Memory.h>
#include <djvCore/UID.h>

using namespace djv::Core;

namespace djv
{
    namespace ViewLib
    {
        struct MDIWidget::Private
        {
            std::shared_ptr<MediaWidget> mediaWidget;
            std::shared_ptr<UI::Label> titleLabel;
            std::shared_ptr<UI::Layout::Horizontal> titleBar;
            std::shared_ptr<UI::Layout::Stack> layout;
            std::shared_ptr<UI::Layout::Border> border;
            std::function<void(void)> maximizeCallback;
            std::function<void(void)> closedCallback;
        };
        
        void MDIWidget::_init(Context * context)
        {
            IWidget::_init(context);

            DJV_PRIVATE_PTR();
            p.mediaWidget = MediaWidget::create(context);

            p.titleLabel = UI::Label::create(context);
            p.titleLabel->setTextHAlign(UI::TextHAlign::Left);
            p.titleLabel->setMargin(UI::Style::MetricsRole::MarginSmall);

            auto maximizeButton = UI::Button::Tool::create(context);
            maximizeButton->setIcon("djvIconMaximizeSmall");

            auto closeButton = UI::Button::Tool::create(context);
            closeButton->setIcon("djvIconCloseSmall");

            p.titleBar = UI::Layout::Horizontal::create(context);
            p.titleBar->setBackgroundRole(UI::Style::ColorRole::Overlay);
            p.titleBar->addWidget(p.titleLabel, UI::Layout::RowStretch::Expand);
            auto hLayout = UI::Layout::Horizontal::create(context);
            hLayout->setSpacing(UI::Style::MetricsRole::None);
            hLayout->addWidget(maximizeButton);
            hLayout->addWidget(closeButton);
            p.titleBar->addWidget(hLayout);

            p.layout = UI::Layout::Stack::create(context);
            p.layout->addWidget(p.mediaWidget);
            auto vLayout = UI::Layout::Vertical::create(context);
            vLayout->setSpacing(UI::Style::MetricsRole::None);
            vLayout->addWidget(p.titleBar);
            vLayout->addExpander();
            p.layout->addWidget(vLayout);

            p.border = UI::Layout::Border::create(context);
            p.border->setMargin(UI::Style::MetricsRole::Handle);
            p.border->addWidget(p.layout);
            IContainer::addWidget(p.border);

            auto weak = std::weak_ptr<MDIWidget>(std::dynamic_pointer_cast<MDIWidget>(shared_from_this()));
            maximizeButton->setClickedCallback(
                [weak]
            {
                if (auto widget = weak.lock())
                {
                    if (widget->_p->maximizeCallback)
                    {
                        widget->_p->maximizeCallback();
                    }
                }
            });
            closeButton->setClickedCallback(
                [weak]
            {
                if (auto widget = weak.lock())
                {
                    if (widget->_p->closedCallback)
                    {
                        widget->_p->closedCallback();
                    }
                }
            });
        }

        MDIWidget::MDIWidget() :
            _p(new Private)
        {}

        MDIWidget::~MDIWidget()
        {}

        std::shared_ptr<MDIWidget> MDIWidget::create(Context * context)
        {
            auto out = std::shared_ptr<MDIWidget>(new MDIWidget);
            out->_init(context);
            return out;
        }

        const std::string & MDIWidget::getTitle() const
        {
            return _p->titleLabel->getText();
        }

        void MDIWidget::setTitle(const std::string & text)
        {
            _p->titleLabel->setText(text);
        }

        const std::shared_ptr<Media> & MDIWidget::getMedia() const
        {
            return _p->mediaWidget->getMedia();
        }

        void MDIWidget::setMedia(const std::shared_ptr<Media> & value)
        {
            _p->mediaWidget->setMedia(value);
        }

        void MDIWidget::setClosedCallback(const std::function<void(void)> & value)
        {
            _p->closedCallback = value;
        }

        void MDIWidget::setMaximizeCallback(const std::function<void(void)> & value)
        {
            _p->maximizeCallback = value;
        }

        float MDIWidget::getHeightForWidth(float value) const
        {
            return _p->border->getHeightForWidth(value);
        }

        void MDIWidget::_preLayoutEvent(Event::PreLayout& event)
        {
            _setMinimumSize(_p->border->getMinimumSize());
        }

        void MDIWidget::_layoutEvent(Event::Layout&)
        {
            _p->border->setGeometry(getGeometry());
        }
        
    } // namespace ViewLib
} // namespace djv
