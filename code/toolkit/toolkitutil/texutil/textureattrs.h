#pragma once
//------------------------------------------------------------------------------
/**
    @class ToolkitUtil::TextureAttrs
    
    Defines texture batch conversion attributes.
    
    (C) 2008 Radon Labs GmbH
*/
#include "util/string.h"

//------------------------------------------------------------------------------
namespace ToolkitUtil
{
class TextureAttrs
{
public:
    /// pixel formats
    enum PixelFormat
    {
        DXT1C,
        DXT1A,
        DXT3,
        DXT5,
        DXT5NM,
        U1555,
        U4444,
        U565,
        U8888,
        U888,
        V8U8,
    };

    /// filter types
    enum Filter
    {
        Point,
        Box,
        Triangle,
        Quadrat,
        Cubic,
        Kaiser,
    };

    /// quality tyoes
    enum Quality
    {
        Low,
        Normal,
        High,
    };
      
    /// constructor
    TextureAttrs();
    /// set max width
    void SetMaxWidth(SizeT w);
    /// get max width
    SizeT GetMaxWidth() const;
    /// set max height
    void SetMaxHeight(SizeT h);
    /// get max height
    SizeT GetMaxHeight() const;
    /// set mipmap generation flag
    void SetGenMipMaps(bool b);
    /// get mipmap generation flag
    bool GetGenMipMaps() const;
    /// set RGB pixel format
    void SetRGBPixelFormat(PixelFormat p);
    /// get RGB pixel form
    PixelFormat GetRGBPixelFormat() const;
    /// set RGBA pixel format
    void SetRGBAPixelFormat(PixelFormat p);
    /// get RGBA pixel format
    PixelFormat GetRGBAPixelFormat() const;
    /// set mipmap filter
    void SetMipMapFilter(Filter f);
    /// get mipmap filter
    Filter GetMipMapFilter() const;
    /// set scale filter
    void SetScaleFilter(Filter f);
    /// get scale filter
    Filter GetScaleFilter() const;
    /// set conversion quality
    void SetQuality(Quality q);
    /// get conversion quality
    Quality GetQuality() const;

    /// convert pixel format to string
    static Util::String PixelFormatToString(PixelFormat p);
    /// convert string to pixel format
    static PixelFormat StringToPixelFormat(const Util::String& str);
    /// convert filter type to string
    static Util::String FilterToString(Filter f);
    /// convert string to filter type
    static Filter StringToFilter(const Util::String& str);
    /// convert quality to string
    static Util::String QualityToString(Quality q);
    /// convert string to quality
    static Quality StringToQuality(const Util::String& str);

private:
    SizeT maxWidth;
    SizeT maxHeight;
    bool genMipMaps;
    PixelFormat rgbPixelFormat;
    PixelFormat rgbaPixelFormat;
    Filter mipMapFilter;
    Filter scaleFilter;
    Quality quality;
};

//------------------------------------------------------------------------------
/**
*/
inline void
TextureAttrs::SetMaxWidth(SizeT w)
{
    this->maxWidth = w;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
TextureAttrs::GetMaxWidth() const
{
    return this->maxWidth;
}

//------------------------------------------------------------------------------
/**
*/
inline void
TextureAttrs::SetMaxHeight(SizeT h)
{
    this->maxHeight = h;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
TextureAttrs::GetMaxHeight() const
{
    return this->maxHeight;
}

//------------------------------------------------------------------------------
/**
*/
inline void
TextureAttrs::SetGenMipMaps(bool b)
{
    this->genMipMaps = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
TextureAttrs::GetGenMipMaps() const
{
    return this->genMipMaps;
}

//------------------------------------------------------------------------------
/**
*/
inline void
TextureAttrs::SetRGBPixelFormat(PixelFormat p)
{
    this->rgbPixelFormat = p;
}

//------------------------------------------------------------------------------
/**
*/
inline TextureAttrs::PixelFormat
TextureAttrs::GetRGBPixelFormat() const
{
    return this->rgbPixelFormat;
}

//------------------------------------------------------------------------------
/**
*/
inline void
TextureAttrs::SetRGBAPixelFormat(PixelFormat p)
{
    this->rgbaPixelFormat = p;
}

//------------------------------------------------------------------------------
/**
*/
inline TextureAttrs::PixelFormat
TextureAttrs::GetRGBAPixelFormat() const
{
    return this->rgbaPixelFormat;
}

//------------------------------------------------------------------------------
/**
*/
inline void
TextureAttrs::SetMipMapFilter(Filter f)
{
    this->mipMapFilter = f;
}

//------------------------------------------------------------------------------
/**
*/
inline TextureAttrs::Filter
TextureAttrs::GetMipMapFilter() const
{
    return this->mipMapFilter;
}

//------------------------------------------------------------------------------
/**
*/
inline void
TextureAttrs::SetScaleFilter(Filter f)
{
    this->scaleFilter = f;
}

//------------------------------------------------------------------------------
/**
*/
inline TextureAttrs::Filter
TextureAttrs::GetScaleFilter() const
{
    return this->scaleFilter;
}

//------------------------------------------------------------------------------
/**
*/
inline void
TextureAttrs::SetQuality(Quality q)
{
    this->quality = q;
}

//------------------------------------------------------------------------------
/**
*/
inline TextureAttrs::Quality
TextureAttrs::GetQuality() const
{
    return this->quality;
}

} // namespace ToolkitUtil
//------------------------------------------------------------------------------
    