#include "texture-utils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>

our::Texture2D* our::texture_utils::empty(GLenum format, glm::ivec2 size){
    our::Texture2D* texture = new our::Texture2D();
    //TODO: (Req 11) Finish this function to create an empty texture with the given size and format

    // Bind the texture using the bind method of the Texture2D class
    texture->bind();

    // glTexImage2D: Specifies a two-dimensional texture image
    // glTexImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void * data);
    // target : Specifies the target texture. (Bind to GL_TEXTURE_2D)
    // level : Specifies the level-of-detail number. Level 0 is the base image level. Level n is the nth mipmap reduction image.
    // internalFormat : Specifies the number of color components in the texture. (4 as we have RGBA)
    // width : Specifies the width of the texture image. (size.x)
    // height : Specifies the height of the texture image. (size.y)
    // border : Specifies the width of the border. Must be 0.
    // format : Specifies the format of the pixel data. (GL_RGBA)
    // type : Specifies the data type of the pixel data. (GL_UNSIGNED_BYTE)
    // data : Specifies a pointer to the image data in memory.
    // data is nullptr as no data is passed to the function, we are creating an empty texture.
    glTexImage2D(GL_TEXTURE_2D, 0, format, size.x, size.y, 0, format, GL_UNSIGNED_BYTE, NULL);

    texture->unbind();
    return texture;
}

our::Texture2D* our::texture_utils::loadImage(const std::string& filename, bool generate_mipmap) {
    glm::ivec2 size;
    int channels;
    //Since OpenGL puts the texture origin at the bottom left while images typically has the origin at the top left,
    //We need to till stb to flip images vertically after loading them
    stbi_set_flip_vertically_on_load(true);
    //Load image data and retrieve width, height and number of channels in the image
    //The last argument is the number of channels we want and it can have the following values:
    //- 0: Keep number of channels the same as in the image file
    //- 1: Grayscale only
    //- 2: Grayscale and Alpha
    //- 3: RGB
    //- 4: RGB and Alpha (RGBA)
    //Note: channels (the 4th argument) always returns the original number of channels in the file
    unsigned char* pixels = stbi_load(filename.c_str(), &size.x, &size.y, &channels, 4);
    if(pixels == nullptr){
        std::cerr << "Failed to load image: " << filename << std::endl;
        return nullptr;
    }
    // Create a texture
    our::Texture2D* texture = new our::Texture2D();
    //Bind the texture such that we upload the image data to its storage
    //DONE (Req 5) Finish this function to fill the texture with the data found in "pixels"

    // Bind the texture using the bind method of the Texture2D class
    texture->bind();

    // Fill the texture with the data found in "pixels" 
    // glTexImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void * data);
    // target : Specifies the target texture. (Bind to GL_TEXTURE_2D)
    // level : Specifies the level-of-detail number. Level 0 is the base image level. Level n is the nth mipmap reduction image.
    // internalFormat : Specifies the number of color components in the texture. (4 as we have RGBA)
    // width : Specifies the width of the texture image. (size.x)
    // height : Specifies the height of the texture image. (size.y)
    // border : Specifies the width of the border. Must be 0.
    // format : Specifies the format of the pixel data. (GL_RGBA)
    // type : Specifies the data type of the pixel data. (GL_UNSIGNED_BYTE)
    // data : Specifies a pointer to the image data in memory.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    // Generate mipmaps for the texture
    if(generate_mipmap){
        // glGenerateMipmap(GLenum target);
        // target : Specifies the target texture. (Bind to GL_TEXTURE_2D)
        // The mipmaps are used for minification filtering (When the texture is smaller than the screen)
        // Nearest neighbor minification filtering or interpolation minification filtering
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    
    stbi_image_free(pixels); //Free image data after uploading to GPU
    return texture;
}