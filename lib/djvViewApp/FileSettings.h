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

#include <djvUI/ISettings.h>

#include <djvCore/BBox.h>
#include <djvCore/ListObserver.h>
#include <djvCore/ValueObserver.h>

namespace djv
{
    namespace Core
    {
        namespace FileSystem
        {
            class FileInfo;

        } // namespace FileSystem
    } // namespace Core

    namespace ViewApp
    {
        //! This class provides the file settings.
        class FileSettings : public UI::Settings::ISettings
        {
            DJV_NON_COPYABLE(FileSettings);

        protected:
            void _init(const std::shared_ptr<Core::Context>&);

            FileSettings();

        public:
            virtual ~FileSettings();

            static std::shared_ptr<FileSettings> create(const std::shared_ptr<Core::Context>&);
            
            std::shared_ptr<Core::IValueSubject<size_t> > observeOpenMax() const;
            void setOpenMax(size_t);

            std::shared_ptr<Core::IListSubject<Core::FileSystem::FileInfo> > observeRecentFiles() const;
            void setRecentFiles(const std::vector<Core::FileSystem::FileInfo>&);

            std::shared_ptr<Core::IValueSubject<bool> > observeAutoDetectSequences() const;
            void setAutoDetectSequences(bool);

            std::shared_ptr<Core::IValueSubject<bool> > observeCacheEnabled() const;
            std::shared_ptr<Core::IValueSubject<int> > observeCacheMaxGB() const;
            void setCacheEnabled(bool);
            void setCacheMaxGB(int);

            const std::map<std::string, Core::BBox2f>& getWidgetGeom() const;
            void setWidgetGeom(const std::map<std::string, Core::BBox2f>&);

            void load(const picojson::value &) override;
            picojson::value save() override;

        private:
            DJV_PRIVATE();
        };

    } // namespace ViewApp
} // namespace djv

