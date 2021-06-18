#include"texture.h"
#include"vendors/stb_image/stb_image.h"

texture::texture(const std::string& path)
	:m_rendererID{0}, m_filepath{path}, m_localBuffer{ NULL }, m_width{ 0 }, m_height{ 0 }, m_byte{ 0 }
{

	stbi_set_flip_vertically_on_load(1); // the reason for this is openGL expects our texture pixels to start from bottom left and
										// the png format has pixels (0,0) at top left it depends on the format so read the documentation for that
	
	
	m_localBuffer = stbi_load(m_filepath.c_str(), &m_width, &m_height, &m_byte, 4);
	//the image data is now stored in the m_localBuffer
	
	GLCall(glGenTextures(1, &m_rendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));


	if (m_localBuffer)
		stbi_image_free(m_localBuffer);
}

texture::~texture()
{
	GLCall(glDeleteTextures(1, &m_rendererID));
}


void texture::bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0+slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererID));

}


void texture::unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

}




