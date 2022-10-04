#pragma once

#include <glm/glm.hpp>
#include <vector>

enum eBitmapType
{
    eBitmapType_2D,
    eBitmapType_Cube
};

enum eBitmapFormat
{
    eBitmapFormat_UnsignedByte,
    eBitmapFormat_Float
};

struct Bitmap
{
public:
    Bitmap() = default;
    Bitmap(int w, int h, int cmp, eBitmapFormat fmt);
    Bitmap(int w, int h, int d, int comp, eBitmapFormat fmt);
    Bitmap(int w, int h, int comp, eBitmapFormat fmt, const void* ptr);

    static int getBytesPerComponent(eBitmapFormat fmt);
    void setPixel(int x, int y, const glm::vec4& c);
    glm::vec4 getPixel(int x, int y) const;

    int getWidth() const { return m_w; }
    int getHeight() const { return m_h; }
    int getComp() const { return m_comp; }
    eBitmapFormat getFormat() const { return m_fmt; }
    eBitmapType getType() const { return m_type; }

    void setType(eBitmapType newType) { m_type = newType; }
    std::vector<uint8_t> m_data;

private:
    int m_w = 0;
    int m_h = 0;
    int m_d = 1;
    int m_comp = 3;
    eBitmapFormat m_fmt = eBitmapFormat::eBitmapFormat_UnsignedByte;
    eBitmapType m_type = eBitmapType::eBitmapType_2D;


private:
    using setPixel_t = void(Bitmap::*)(int, int, const glm::vec4&);
    using getPixel_t = glm::vec4(Bitmap::*)(int, int) const;
    setPixel_t setPixelFunc = &Bitmap::setPixelUnsignedByte;
    getPixel_t getPixelFunc = &Bitmap::getPixelUnsignedByte;

    void initGetSetFuncs();
    void setPixelFloat(int x, int y, const glm::vec4& c);
    glm::vec4 getPixelFloat(int x, int y) const;
    void setPixelUnsignedByte(int x, int y, const glm::vec4& c);
    glm::vec4 getPixelUnsignedByte(int x, int y) const;
};