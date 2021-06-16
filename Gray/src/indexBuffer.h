#pragma once


#include<GL/glew.h>

class indexBuffer {
private:
	unsigned int m_renderer_id;
	/* id : we know that openGL needs some kind of numeric id to keep track for objects created in openGL  */
	unsigned int m_count;

public:
	indexBuffer(const unsigned int* data, unsigned int count);
	~indexBuffer();

	void unbind() const;
	void bind() const; // in an actual engine we would have more things like getData(), lock() , unlock() etc.

	inline unsigned int getcount() const { return m_count; }

};