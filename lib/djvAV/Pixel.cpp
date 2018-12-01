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

#include <djvAV/Pixel.h>

#include <algorithm>
#include <sstream>

namespace djv
{
    namespace AV
    {
        namespace Image
        {
        } // namespace Image
    } // namespace AV

    DJV_ENUM_SERIALIZE_HELPERS_IMPLEMENTATION(
        AV::Image,
        Pixel,
        DJV_TEXT("None"),
        DJV_TEXT("L_U8"),
        DJV_TEXT("L_U16"),
        DJV_TEXT("L_U32"),
        DJV_TEXT("L_F16"),
        DJV_TEXT("L_F32"),
        DJV_TEXT("LA_U8"),
        DJV_TEXT("LA_U16"),
        DJV_TEXT("LA_U32"),
        DJV_TEXT("LA_F16"),
        DJV_TEXT("LA_F32"),
        DJV_TEXT("RGB_U8"),
        DJV_TEXT("RGB_U10"),
        DJV_TEXT("RGB_U16"),
        DJV_TEXT("RGB_U32"),
        DJV_TEXT("RGB_F16"),
        DJV_TEXT("RGB_F32"),
        DJV_TEXT("RGBA_U8"),
        DJV_TEXT("RGBA_U16"),
        DJV_TEXT("RGBA_U32"),
        DJV_TEXT("RGBA_F16"),
        DJV_TEXT("RGBA_F32"));

} // namespace djv

