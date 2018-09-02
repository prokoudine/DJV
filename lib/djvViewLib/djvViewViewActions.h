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

#pragma once

#include <djvViewAbstractActions.h>

struct djvViewViewActionsPrivate;

//! \addtogroup djvViewView
//@{

//------------------------------------------------------------------------------
//! \class djvViewViewActions
//!
//! This class provides the view group actions.
//------------------------------------------------------------------------------

class DJV_VIEW_LIB_EXPORT djvViewViewActions : public djvViewAbstractActions
{
    Q_OBJECT
    Q_ENUMS(ACTION)
    Q_ENUMS(GROUP)
    
public:
    //! This enumeration provides the actions.
    enum ACTION
    {
        LEFT,
        RIGHT,
        UP,
        DOWN,
        CENTER,
        ZOOM_IN,
        ZOOM_OUT,
        ZOOM_RESET,
        RESET,
        FIT,
        HUD,

        ACTION_COUNT
    };

    //! This enumeration provides the action groups.
    enum GROUP
    {
        GRID_GROUP,

        GROUP_COUNT
    };

    explicit djvViewViewActions(djvViewContext *, QObject * parent = 0);

    virtual ~djvViewViewActions();

private Q_SLOTS:
    void update();

private:
    DJV_PRIVATE_COPY(djvViewViewActions);
    
    djvViewViewActionsPrivate * _p;
};

//@} // djvViewView

