#include "OpenALAudioDevice/OpenALAudioStream.h"
#include "OpenALAudioDevice/OpenALAudioManager.h"

OpenALAudioStream::OpenALAudioStream()
{
    alGenSources(1, &m_source);
    alGenBuffers(AL_STREAM_BUFFER_COUNT, m_buffers);

    // Make stream ignore positioning
    alSource3i(m_source, AL_POSITION, 0, 0, 0);
    alSourcei(m_source, AL_SOURCE_RELATIVE, AL_TRUE);
    alSourcei(m_source, AL_ROLLOFF_FACTOR, 0);
}

OpenALAudioStream::~OpenALAudioStream()
{
    // Unbind the buffers first
    alSourceStop(m_source);
    alSourcei(m_source, AL_BUFFER, 0);
    DEBUG_ASSERTLOG(OpenALAudioManager::checkALError(), ("Failed to unbind buffers"));
    alDeleteSources(1, &m_source);
    DEBUG_ASSERTLOG(OpenALAudioManager::checkALError(), ("Failed to delete source"));
    // Now delete the buffers
    alDeleteBuffers(AL_STREAM_BUFFER_COUNT, m_buffers);
    DEBUG_ASSERTLOG(OpenALAudioManager::checkALError(), ("Failed to delete buffers"));
}

bool OpenALAudioStream::bufferData(uint8_t *data, size_t data_size, ALenum format, int samplerate)
{
    ALint num_queued;
    alGetSourcei(m_source, AL_BUFFERS_QUEUED, &num_queued);
    if (num_queued >= AL_STREAM_BUFFER_COUNT) {
        DEBUG_LOG(("Having too many buffers already queued: %i", num_queued));
        return false;
    }

    ALuint &current_buffer = m_buffers[m_current_buffer_idx];
    alBufferData(current_buffer, format, data, data_size, samplerate);
    alSourceQueueBuffers(m_source, 1, &current_buffer);
    m_current_buffer_idx++;

    if (m_current_buffer_idx >= AL_STREAM_BUFFER_COUNT)
        m_current_buffer_idx = 0;

    return true;
}

void OpenALAudioStream::update()
{
    ALint processed;
    alGetSourcei(m_source, AL_BUFFERS_PROCESSED, &processed);
    while (processed > 0) {
        ALuint buffer;
        alSourceUnqueueBuffers(m_source, 1, &buffer);
        processed--;
    }

    ALint num_queued;
    alGetSourcei(m_source, AL_BUFFERS_QUEUED, &num_queued);
    DEBUG_LOG(("Having %i buffers queued", num_queued));
    if (num_queued == 1) {
        // TODO: we could invoke a callback here, asking for more audio data
    }
}

void OpenALAudioStream::reset()
{
    alSourceRewind(m_source);
    alSourcei(m_source, AL_BUFFER, 0);
}

bool OpenALAudioStream::isPlaying()
{
    ALint state;
    alGetSourcei(m_source, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}