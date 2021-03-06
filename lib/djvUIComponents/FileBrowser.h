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

#include <set>

namespace djv
{
    namespace Core
    {
        namespace FileSystem
        {
            class FileInfo;
            class Path;

        } // namespace FileSystem
    } // namespace Core

    namespace UI
    {
        //! This namespace provides file browser functionality.
        namespace FileBrowser
        {
            //! This class provides a file browser widget.
            class FileBrowser : public Widget
            {
                DJV_NON_COPYABLE(FileBrowser);

            protected:
                void _init(const std::shared_ptr<Core::Context>&);
                FileBrowser();

            public:
                virtual ~FileBrowser();

                static std::shared_ptr<FileBrowser> create(const std::shared_ptr<Core::Context>&);

                void setFileExtensions(const std::set<std::string>&);

                const Core::FileSystem::Path& getPath() const;
                void setPath(const Core::FileSystem::Path &);
                void setCallback(const std::function<void(const Core::FileSystem::FileInfo &)> &);

                float getHeightForWidth(float) const override;

            protected:
                void _preLayoutEvent(Core::Event::PreLayout &) override;
                void _layoutEvent(Core::Event::Layout &) override;

                void _initEvent(Core::Event::Init &) override;

            private:
                std::string _getItemCountLabel(size_t) const;

                DJV_PRIVATE();
            };

        } // namespace FileBrowser
    } // namespace UI
} // namespace djv

