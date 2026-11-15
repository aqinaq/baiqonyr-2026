#pragma once
#include <glad/gl.h>
#include <string>
namespace bk {
class Texture {
    GLuint id = 0;

  public:
    Texture();
    explicit Texture(const std::string &path);
    ~Texture();
    Texture(const Texture &) = delete;
    Texture &operator=(const Texture &) = delete;
    void bind(unsigned unit = 0) const {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, id);
    }
};
} // namespace bk
