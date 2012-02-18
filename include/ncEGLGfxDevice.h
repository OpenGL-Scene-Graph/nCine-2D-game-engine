#ifndef CLASS_NCEGLGFXDEVICE
#define CLASS_NCEGLGFXDEVICE

#include <EGL/egl.h>
#include <GLES/gl.h>

#include "ncIGfxDevice.h"
#include "ncPoint.h"
#include "ncDisplayMode.h"

struct android_app;

/// It represents a graphics device where everything is rendered
/*! At the moment it uses SDL to create an OpenGL context */
class ncEGLGfxDevice : public ncIGfxDevice
{
private:
	/// The EGL display connection
	EGLDisplay m_display;
	/// The EGL drawing surface
	EGLSurface m_surface;
	/// The EGL context
	EGLContext m_context;

	// Initializes the OpenGL graphic context
	void InitDevice(struct android_app* state);
	// Initializes starting OpenGL state
	void InitGL();

public:
	// Constructor taking a DisplayMode
	ncEGLGfxDevice(struct android_app* state, ncDisplayMode mode);
	// Destructor
	virtual ~ncEGLGfxDevice();

	/// Sets screen resolution with two integers
	virtual void SetResolution(int iWidth, int iHeight) { }
	/// Sets screen resolution with the Size class
	virtual void SetResolution(ncPoint size) { }

	/// Toggles between fullscreen and windowed mode
	virtual void ToggleFullScreen() { }

	/// Updates the screen swapping back and front buffers
	inline virtual void Update() { eglSwapBuffers(m_display, m_surface); }
	/// Clears the screen
	inline virtual void Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
};

#endif