#pragma once

#include"renderer.h"

class texture {
private:
	unsigned int m_rendererID;
	std::string m_filepath;
	unsigned char* m_localBuffer;
	int m_width, m_height, m_byte;

public:
	texture(const std::string& path);
	~texture();

	void bind(unsigned int slot = 0) const;	//in modern GPUs we have about 32 texture slots and in mobile we have
											//about 8 so if we have to bind a single slot we can just pass its index 
											//as 1-based parameter
	void unbind() const;

	inline int getWidth() const { return m_width; }
	inline int getHeight() const { return m_height; }
};