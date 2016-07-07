#ifndef CLASS_NCINE_AUDIOSTREAM
#define CLASS_NCINE_AUDIOSTREAM

#include "IAudioLoader.h"
#include "StaticArray.h"

namespace ncine {

/// Audio stream class
class DLL_PUBLIC AudioStream
{
  public:
	virtual ~AudioStream();

	/// Enqueues new buffers and unqueues processed ones
	bool enqueue(unsigned int source, bool looping);
	/// Unqueues any left buffer and rewinds the loader
	void stop(unsigned int source);

	/// Returns samples frequency
	inline int frequency() const { return frequency_; }

  private:
	/// Number of buffers for streaming
	static const int NumBuffers = 3;
	/// OpenAL buffer queue for streaming
	StaticArray<unsigned int, NumBuffers> alBuffers_;
	/// Index of the next available OpenAL buffer
	int nextAvailALBuffer_;

	/// Size in bytes of each streaming buffer
	static const int BufferSize = 16 * 1024;
	/// Memory buffer to feed OpenAL ones
	char *memBuffer_;

	/// Samples frequency
	int frequency_;
	/// OpenAL channel format enumeration
	int format_;
	/// The associated loader to continuosly stream decoded data
	IAudioLoader *audioLoader_;

	/// Constructor creating an audio stream from an audio file
	explicit AudioStream(const char *filename);
	/// Private copy constructor
	AudioStream(const AudioStream&);
	/// Private assignment operator
	AudioStream& operator=(const AudioStream&);

	friend class AudioStreamPlayer;
};

}

#endif