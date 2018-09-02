//------------------------------------------------------------------------------
// Copyright (c) 2004-2015 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
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

#include <djvViewViewToolBar.h>

#include <djvViewViewActions.h>

#include <djvFloatEdit.h>
#include <djvFloatObject.h>
#include <djvToolButton.h>

//------------------------------------------------------------------------------
// djvViewViewToolBarPrivate
//------------------------------------------------------------------------------

struct djvViewViewToolBarPrivate
{
    djvViewViewToolBarPrivate() :
        zoomEdit(0)
    {}

    djvFloatEdit * zoomEdit;
};

//------------------------------------------------------------------------------
// djvViewViewToolBar
//------------------------------------------------------------------------------

djvViewViewToolBar::djvViewViewToolBar(
    djvViewAbstractActions * actions,
    djvViewContext *         context,
    QWidget *                parent) :
    djvViewAbstractToolBar(actions, context, parent),
    _p(new djvViewViewToolBarPrivate)
{
    djvToolButton * button = new djvToolButton;
    button->setDefaultAction(actions->action(djvViewViewActions::ZOOM_IN));
    addWidget(button);

    button = new djvToolButton;
    button->setDefaultAction(actions->action(djvViewViewActions::ZOOM_OUT));
    addWidget(button);

    button = new djvToolButton;
    button->setDefaultAction(actions->action(djvViewViewActions::ZOOM_RESET));
    addWidget(button);

    button = new djvToolButton;
    button->setDefaultAction(actions->action(djvViewViewActions::FIT));
    addWidget(button);

    _p->zoomEdit = new djvFloatEdit;
    _p->zoomEdit->setRange(0.1, 1000.0);
    _p->zoomEdit->object()->setInc(0.1, 0.1);
    addWidget(_p->zoomEdit);
}

djvViewViewToolBar::~djvViewViewToolBar()
{
    delete _p;
}

djvFloatEdit * djvViewViewToolBar::zoomEdit() const
{
    return _p->zoomEdit;
}
