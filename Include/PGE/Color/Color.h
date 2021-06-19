#ifndef PGE_COLOR_H_INCLUDED
#define PGE_COLOR_H_INCLUDED

#include <PGE/Types/Types.h>
#include <PGE/Exception/Exception.h>

#define PGE_ASSERT_COLOR(color, name) PGE_ASSERT((color).name >= 0.f && (color).name <= 1.f, "Value is outside of valid range (" #name ": " + String::fromFloat((color).name) + ")")

namespace PGE {

class Color {
    private:
        constexpr void validateColor(const Color& color) {
            PGE_ASSERT_COLOR(color, red);
            PGE_ASSERT_COLOR(color, green);
            PGE_ASSERT_COLOR(color, blue);
            PGE_ASSERT_COLOR(color, alpha);
        }

    public:
        Color() = default;
        
        constexpr Color(byte r, byte g, byte b, byte a = 255) : red(r / 255.f), green(g / 255.f), blue(b / 255.f), alpha(a / 255.f) {
            validateColor(*this);
        }

        constexpr Color(float r, float g, float b, float a = 1.f) : red(r), green(g), blue(b), alpha(a) {
            validateColor(*this);
        }

        static constexpr Color fromHSV(float h, float s, float v, float a = 1.f) {
            PGE_ASSERT(h >= 0 && h <= 360.f, "Hue is outside of valid range (hue: " + String::fromFloat(h) + ")");
            PGE_ASSERT(s >= 0 && s <= 1.f, "Saturation is outside of valid range (saturation: " + String::fromFloat(s) + ")");
            PGE_ASSERT(v >= 0 && v <= 1.f, "Value is outside of valid range (value: " + String::fromFloat(v) + ")");

            float hh = h / 60.f;
            int i = (int)hh;
            float dr = hh - i; // Decimal remainder.

            float x = v * (1.f - s);
            float y = v * (1.f - (s * dr));
            float z = v * (1.f - (s * (1.f - dr)));

            switch (i) {
                default:
                case 0: {
                    return Color(v, z, x);
                }
                case 1: {
                    return Color(y, v, x);
                }
                case 2: {
                    return Color(x, v, z);
                }
                case 3: {
                    return Color(x, y, v);
                }
                case 4: {
                    return Color(z, x, v);
                }
                case 5: {
                    return Color(v, x, y);
                }
            }
        }

        constexpr bool operator==(const Color& other) const {
            return red == other.red && blue == other.blue && green == other.green && alpha == other.alpha;
        }

        constexpr bool operator!=(const Color& other) const {
            return red != other.red || blue != other.blue || green != other.green || alpha != other.alpha;
        }

        constexpr byte getRed() const { return (byte)(red * 255.f); }
        constexpr byte getGreen() const { return (byte)(green * 255.f); }
        constexpr byte getBlue() const { return (byte)(blue * 255.f); }
        constexpr byte getAlpha() const { return (byte)(alpha * 255.f); }
        constexpr void setRed(byte r) { red = ((float)r) / 255.f; PGE_ASSERT_COLOR(*this, red); }
        constexpr void setGreen(byte g) { green = ((float)g) / 255.f; PGE_ASSERT_COLOR(*this, green); }
        constexpr void setBlue(byte b) { blue = ((float)b) / 255.f; PGE_ASSERT_COLOR(*this, blue); }
        constexpr void setAlpha(byte a) { alpha = ((float)a) / 255.f; PGE_ASSERT_COLOR(*this, alpha); }

        float red; float green; float blue; float alpha;

        static const Color RED;
        static const Color GREEN;
        static const Color BLUE;
        static const Color ORANGE;
        static const Color YELLOW;
        static const Color CYAN;
        static const Color MAGENTA;
        static const Color WHITE;
        static const Color GRAY;
        static const Color BLACK;
};

}

#endif // PGE_COLOR_H_INCLUDED
