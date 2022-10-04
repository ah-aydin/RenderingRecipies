#include "pch.h"
#include "Bitmap.h"

#include "../Log/Log.h"

Bitmap::Bitmap(int w, int h, int comp, eBitmapFormat fmt)
    : m_w(w), m_h(h), m_comp(comp), m_fmt(fmt), m_data(w* h* comp* getBytesPerComponent(fmt))
{
    initGetSetFuncs();
}

Bitmap::Bitmap(int w, int h, int d, int comp, eBitmapFormat fmt)
    : m_w(w), m_h(h), m_d(d), m_comp(comp), m_fmt(fmt), m_data(w* h* d* comp* getBytesPerComponent(fmt))
{
    initGetSetFuncs();
}

Bitmap::Bitmap(int w, int h, int comp, eBitmapFormat fmt, const void* ptr)
    : m_w(w), m_h(h), m_comp(comp), m_fmt(fmt), m_data(w* h* comp* getBytesPerComponent(fmt))
{
    initGetSetFuncs();
    memcpy(m_data.data(), ptr, m_data.size());
}

int Bitmap::getBytesPerComponent(eBitmapFormat fmt)
{
    if (fmt == eBitmapFormat_UnsignedByte) return 1;
    if (fmt == eBitmapFormat_Float) return 4;
    return 0;
}

void Bitmap::setPixel(int x, int y, const glm::vec4& c)
{
    return (*this.*setPixelFunc)(x, y, c);
}

glm::vec4 Bitmap::getPixel(int x, int y) const
{
    return (*this.*getPixelFunc)(x, y);
}

void Bitmap::initGetSetFuncs()
{
    switch (m_fmt)
    {
    case eBitmapFormat_Float:
        setPixelFunc = &Bitmap::setPixelFloat;
        getPixelFunc = &Bitmap::getPixelFloat;
        break;
    case eBitmapFormat_UnsignedByte:
        setPixelFunc = &Bitmap::setPixelUnsignedByte;
        getPixelFunc = &Bitmap::getPixelUnsignedByte;
        break;
    }
}

void Bitmap::setPixelFloat(int x, int y, const glm::vec4& c)
{
    const int ofs = m_comp * (y * m_w + x);
    float* data = reinterpret_cast<float*>(m_data.data());
    if (m_comp > 0) data[ofs + 0] = c.x;
    if (m_comp > 1) data[ofs + 1] = c.y;
    if (m_comp > 2) data[ofs + 2] = c.z;
    if (m_comp > 3) data[ofs + 3] = c.w;
}

glm::vec4 Bitmap::getPixelFloat(int x, int y) const
{
    const int ofs = m_comp * (y * m_w + x);
    const float* data = reinterpret_cast<const float*>(m_data.data());
    return glm::vec4(
        m_comp > 0 ? data[ofs + 0] : 0.0f,
        m_comp > 1 ? data[ofs + 1] : 0.0f,
        m_comp > 2 ? data[ofs + 2] : 0.0f,
        m_comp > 3 ? data[ofs + 3] : 0.0f);
}

void Bitmap::setPixelUnsignedByte(int x, int y, const glm::vec4& c)
{
    const int ofs = m_comp * (y * m_w + x);
    if (m_comp > 0) m_data[ofs + 0] = uint8_t(c.x * 255.0f);;
    if (m_comp > 1) m_data[ofs + 1] = uint8_t(c.y * 255.0f);
    if (m_comp > 2) m_data[ofs + 2] = uint8_t(c.z * 255.0f);
    if (m_comp > 3) m_data[ofs + 3] = uint8_t(c.w * 255.0f);
}

glm::vec4 Bitmap::getPixelUnsignedByte(int x, int y) const
{
    const int ofs = m_comp * (y * m_w + x);
    return glm::vec4(
        m_comp > 0 ? float(m_data[ofs + 0]) / 255.0f : 0.0f,
        m_comp > 1 ? float(m_data[ofs + 1]) / 255.0f : 0.0f,
        m_comp > 2 ? float(m_data[ofs + 2]) / 255.0f : 0.0f,
        m_comp > 3 ? float(m_data[ofs + 3]) / 255.0f : 0.0f);
}