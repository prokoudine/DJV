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

#include <djvCore/RecentFilesModel.h>

#include <djvCore/FileInfo.h>

namespace djv
{
    namespace Core
    {
        namespace FileSystem
        {
            namespace
            {
                const size_t filesMaxDefault = 100;

            } // namespace

            struct RecentFilesModel::Private
            {
                std::shared_ptr<ListSubject<FileInfo> > files;
                std::shared_ptr<ValueSubject<size_t> > filesMax;
            };

            void RecentFilesModel::_init(const std::shared_ptr<Context>& context)
            {
                DJV_PRIVATE_PTR();
                p.files = ListSubject<FileInfo>::create();
                p.filesMax = ValueSubject<size_t>::create(filesMaxDefault);
            }

            RecentFilesModel::RecentFilesModel() :
                _p(new Private)
            {}

            std::shared_ptr<RecentFilesModel> RecentFilesModel::create(const std::shared_ptr<Context>& context)
            {
                auto out = std::shared_ptr<RecentFilesModel>(new RecentFilesModel);
                out->_init(context);
                return out;
            }

            std::shared_ptr<IListSubject<FileInfo> > RecentFilesModel::observeFiles() const
            {
                return _p->files;
            }

            void RecentFilesModel::setFiles(std::vector<FileInfo> value)
            {
                DJV_PRIVATE_PTR();
                while (value.size() > p.filesMax->get())
                {
                    value.pop_back();
                }
                p.files->setIfChanged(value);
            }

            void RecentFilesModel::addFile(const FileInfo & value)
            {
                DJV_PRIVATE_PTR();
                std::vector<FileInfo> list = p.files->get();
                auto i = std::find(list.begin(), list.end(), value);
                if (i != list.end())
                {
                    list.erase(i);
                }
                list.insert(list.begin(), value);
                while (list.size() > p.filesMax->get())
                {
                    list.pop_back();
                }
                p.files->setIfChanged(list);
            }

            std::shared_ptr<IValueSubject<size_t> > RecentFilesModel::observeFilesMax() const
            {
                return _p->filesMax;
            }

            void RecentFilesModel::setFilesMax(size_t value)
            {
                DJV_PRIVATE_PTR();
                if (value == p.filesMax->get())
                    return;
                p.filesMax->setIfChanged(value);
                if (p.files->getSize() > value)
                {
                    std::vector<FileInfo> list = p.files->get();
                    while (list.size() > value)
                    {
                        list.pop_back();
                    }
                    p.files->setIfChanged(list);
                }
            }

        } // namespace FileSystem
    } // namespace Core
} // namespace djv
