#include "pch.h"
#include "Cubemap.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

template<typename T>
T clamp(T v, T a, T b)
{
    if (v < a) return a;
    if (v > b) return b;
    return v;
}

glm::vec3 faceCoordsToXYZ(int i, int j, int faceID, int faceSize)
{
    const float A = 2.0f * float(i) / faceSize;
    const float B = 2.0f * float(j) / faceSize;

    if (faceID == 0) return glm::vec3(-1.0f, A - 1.0f, B - 1.0f);	// arka
    if (faceID == 1) return glm::vec3(A - 1.0f, -1.0f, 1.0f - B);	// sol
    if (faceID == 2) return glm::vec3(1.0f, A - 1.0f, 1.0f - B);	// on
    if (faceID == 3) return glm::vec3(1.0f - A, 1.0f, 1.0f - B);	// saga
    if (faceID == 4) return glm::vec3(B - 1.0f, A - 1.0f, 1.0f);	// yukari
    if (faceID == 5) return glm::vec3(1.0f - B, A - 1.0f, -1.0f);	// alt
    return glm::vec3();
}

Bitmap convertEquirectangularMapToVerticalCross(const Bitmap& b)
{
    if (b.getType() != eBitmapType_2D) return Bitmap();
    const int faceSize = b.getWidth() / 4;
    const int w = faceSize * 3;
    const int h = faceSize * 4;

    Bitmap result(w, h, b.getComp(), b.getFormat());

    const glm::ivec2 kFaceOffsets[] =
    {
        glm::ivec2(faceSize, faceSize * 3), // akra
        glm::ivec2(0, faceSize),			// sol
        glm::ivec2(faceSize, faceSize),		// on
        glm::ivec2(faceSize * 2, faceSize),	// saga
        glm::ivec2(faceSize, 0),			// yukari
        glm::ivec2(faceSize, faceSize * 2)	// alt
    };

    const int clampW = b.getWidth() - 1;
    const int clampH = b.getHeight() - 1;

    for (int face = 0; face != 6; ++face)
    {
        for (int i = 0; i != faceSize; ++i)
        {
            for (int j = 0; j != faceSize; ++j)
            {
                const glm::vec3 P = faceCoordsToXYZ(i, j, face, faceSize);
                const float R = hypot(P.x, P.y);
                const float theta = atan2(P.y, P.x);
                const float phi = atan2(P.z, R);

                // float point source coordinates
                const float Uf = float(2.0f * faceSize * (theta + M_PI) / M_PI);
                const float Vf = float(2.0f * faceSize * (M_PI / 2.0f - phi) / M_PI);

                // 4-samples for bilinear interpolation
                const int U1 = clamp(int(floor(Uf)), 0, clampW);
                const int V1 = clamp(int(floor(Vf)), 0, clampH);
                const int U2 = clamp(U1, 0, clampW);
                const int V2 = clamp(V1, 0, clampH);

                // fractional part
                const float s = Uf - U1;
                const float t = Vf - V1;

                // fetch 4-samples
                const glm::vec4 A = b.getPixel(U1, V1);
                const glm::vec4 B = b.getPixel(U2, V1);
                const glm::vec4 C = b.getPixel(U1, V2);
                const glm::vec4 D = b.getPixel(U2, V2);

                // bilinear interpolation
                const glm::vec4 color = A * (1 - s) * (1 - t) + B * (s) * (1 - t) + C * (1 - s) * t + D * (s) * (t);
                result.setPixel(i + kFaceOffsets[face].x, j + kFaceOffsets[face].y, color);
            }
        }
    }

    return result;
}

Bitmap convertVerticalCrossToCubeMapFaces(const Bitmap& b)
{
    const int faceWidth = b.getWidth() / 3;
    const int faceHeight = b.getHeight() / 4;

    Bitmap cubemap(faceWidth, faceHeight, 6, b.getComp(), b.getFormat());
    cubemap.setType(eBitmapType_Cube);

    const uint8_t* src = b.m_data.data();
    uint8_t* dst = cubemap.m_data.data();

    /*
            ------
            | +Y |
        ----------------
        | -X | -Z | +X |
        ----------------
            | -Y |
            ------
            | +Z |
            ------
    */

    const int pixelSize = cubemap.getComp() * Bitmap::getBytesPerComponent(cubemap.getFormat());

    for (int face = 0; face != 6; ++face)
    {
        for (int j = 0; j != faceHeight; ++j)
        {
            for (int i = 0; i != faceWidth; ++i)
            {
                int x = 0;
                int y = 0;

                switch (face)
                {
                    // GL_TEXTURE_CUBE_MAP_POSITIVE_X
                case 0:
                    x = i;
                    y = faceHeight + j;
                    break;

                    // GL_TEXTURE_CUBE_MAP_NEGATIVE_X
                case 1:
                    x = 2 * faceWidth + i;
                    y = 1 * faceHeight + j;
                    break;

                    // GL_TEXTURE_CUBE_MAP_POSITIVE_Y
                case 2:
                    x = 2 * faceWidth - (i + 1);
                    y = 1 * faceHeight - (j + 1);
                    break;

                    // GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
                case 3:
                    x = 2 * faceWidth - (i + 1);
                    y = 3 * faceHeight - (j + 1);
                    break;

                    // GL_TEXTURE_CUBE_MAP_POSITIVE_Z
                case 4:
                    x = 2 * faceWidth - (i + 1);
                    y = b.getHeight() - (j + 1);
                    break;

                    // GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
                case 5:
                    x = faceWidth + i;
                    y = faceHeight + j;
                    break;
                }

                memcpy(dst, src + (y * b.getWidth() + x) * pixelSize, pixelSize);

                dst += pixelSize;
            }
        }
    }

    return cubemap;
}

Bitmap convertEquirectangularMapToCubeMapFaces(const Bitmap& b)
{
    Bitmap cross = convertEquirectangularMapToVerticalCross(b);
    return convertVerticalCrossToCubeMapFaces(cross);
}
