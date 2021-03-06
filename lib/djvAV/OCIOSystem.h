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

#include <djvAV/OCIO.h>

#include <djvCore/ISystem.h>
#include <djvCore/ListObserver.h>
#include <djvCore/MapObserver.h>
#include <djvCore/PicoJSON.h>
#include <djvCore/ValueObserver.h>

#include <OpenColorIO/OpenColorIO.h>

namespace djv
{
    namespace AV
    {
        namespace OCIO
        {
            //! This struct provides a color space configuration.
            class Config
            {
            public:
                Config();
                
                std::string fileName;
                std::string name;
                std::string display;
                std::string view;
                std::map<std::string, std::string> fileColorSpaces;

                static std::string getNameFromFileName(const std::string&);

                bool operator == (const Config&) const;
            };

            //! This class manages color space configurations.
            class System : public Core::ISystem
            {
                DJV_NON_COPYABLE(System);

            protected:
                void _init(const std::shared_ptr<Core::Context>&);
                System();

            public:
                ~System() override;
                static std::shared_ptr<System> create(const std::shared_ptr<Core::Context>&);

                //! \name Configurations
                ///@{

                std::shared_ptr<Core::IListSubject<Config> > observeConfigs() const;

                int addConfig(const std::string& fileName);
                int addConfig(const Config&);
                void removeConfig(int);

                ///@}

                //! \name Current Configuration
                ///@{

                std::shared_ptr<Core::IValueSubject<Config> > observeCurrentConfig() const;
                std::shared_ptr<Core::IValueSubject<int> > observeCurrentIndex() const;
                std::shared_ptr<Core::IListSubject<std::string> > observeColorSpaces() const;
                std::shared_ptr<Core::IListSubject<Display> > observeDisplays() const;
                std::shared_ptr<Core::IListSubject<std::string> > observeViews() const;
            
                void setCurrentConfig(const Config&);
                void setCurrentIndex(int);

                ///@}

                //! \name Utilities
                ///@{

                //! Get the color space for the given display and view.
                std::string getColorSpace(const std::string& display, const std::string& view) const;

                ///@}
                
            private:
                int _addConfig(const Config&, bool init);
                void _configUpdate();

                DJV_PRIVATE();
            };

        } // namespace OCIO
    } // namespace AV

    picojson::value toJSON(const AV::OCIO::Config&);

    //! Throws:
    //! - std::exception
    void fromJSON(const picojson::value&, AV::OCIO::Config&);

} // namespace djv
