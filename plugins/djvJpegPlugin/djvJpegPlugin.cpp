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

#include <djvJpegPlugin.h>

#include <djvJpegLoad.h>
#include <djvJpegSave.h>

#include <djvAssert.h>
#include <djvError.h>

#include <QCoreApplication>

extern "C"
{

DJV_PLUGIN_EXPORT djvPlugin * djvImageIoEntry(djvCoreContext * context)
{
    return new djvJpegPlugin(context);
}

} // extern "C"

//------------------------------------------------------------------------------
// djvJpegPlugin
//------------------------------------------------------------------------------

djvJpegPlugin::djvJpegPlugin(djvCoreContext * context) :
    djvImageIo(context)
{}

QString djvJpegPlugin::pluginName() const
{
    return djvJpeg::staticName;
}

QStringList djvJpegPlugin::extensions() const
{
    return QStringList() <<
        ".jpeg" <<
        ".jpg" <<
        ".jfif";
}

QStringList djvJpegPlugin::option(const QString & in) const
{
    QStringList out;
    if (0 == in.compare(options()[djvJpeg::QUALITY_OPTION], Qt::CaseInsensitive))
    {
        out << _options.quality;
    }
    return out;
}

bool djvJpegPlugin::setOption(const QString & in, QStringList & data)
{
    try
    {
        if (0 == in.compare(options()[djvJpeg::QUALITY_OPTION], Qt::CaseInsensitive))
        {
            int quality = 0;
            data >> quality;
            if (quality != _options.quality)
            {
                _options.quality = quality;
                Q_EMIT optionChanged(in);
            }
        }
    }
    catch (const QString &)
    {
        return false;
    }
    return true;
}

QStringList djvJpegPlugin::options() const
{
    return djvJpeg::optionsLabels();
}

void djvJpegPlugin::commandLine(QStringList & in) throw (QString)
{
    QStringList tmp;
    QString     arg;
    try
    {
        while (! in.isEmpty())
        {
            in >> arg;
            if (qApp->translate("djvJpegPlugin", "-jpeg_quality") == arg)
            {
                in >> _options.quality;
            }
            else
            {
                tmp << arg;
            }
        }
    }
    catch (const QString &)
    {
        throw arg;
    }
    in = tmp;
}

QString djvJpegPlugin::commandLineHelp() const
{
    return qApp->translate("djvJpegPlugin",
"\n"
"JPEG Options\n"
"\n"
"    -jpeg_quality (value)\n"
"        Set the quality used when saving JPEG images. Default = %1.\n").
    arg(djvStringUtil::label(_options.quality).join(", "));
}

djvImageLoad * djvJpegPlugin::createLoad() const
{
    return new djvJpegLoad(imageContext());
}

djvImageSave * djvJpegPlugin::createSave() const
{
    return new djvJpegSave(_options, imageContext());
}
