// All implementations of stb

#define _CRT_SECURE_NO_WARNINGS // to quiet msvc

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_SIMD // SIMD build crashes clang on windows
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
