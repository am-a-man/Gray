#pragma once


#include<GL/glew.h>

class vertexBuffer {
private:
	unsigned int m_renderer_id;
	/* id : we know that openGL needs some kind of numeric id to keep track for objects created in openGL  */

public:
	vertexBuffer(const void* data, unsigned int size);
	~vertexBuffer();

	void unbind() const; 
	void bind() const; // in an actual engine we would have more things like getData(), lock() , unlock() etc.

};