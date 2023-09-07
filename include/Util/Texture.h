#pragma once
#include <glad/glad.h>
#include <string>


class Texture {
    public:
        GLuint ID;
        GLenum target;
        Texture();
        virtual ~Texture();
        void Bind();
};

class Texture2D :public Texture {
    public:
        GLuint Width, Height;
        GLuint Internal_Format;
        GLuint Image_Format;

        GLuint Wrap_S;
        GLuint Wrap_T;
        GLuint Filter_Min;
        GLuint Filter_Max;

        Texture2D();
    };

class TextureCubeMap :public Texture {
    public:
        GLuint Width[6], Height[6];
        GLuint Internal_Format[6];
        GLuint Image_Format[6];

        GLuint Wrap_S;
        GLuint Wrap_T;
        GLuint Wrap_R;
        GLuint Filter_Min;
        GLuint Filter_Max;

        TextureCubeMap();
    };


