#include "AudioManager.hpp"
#include <iostream>

namespace audio {

    AudioManager& AudioManager::Instance() {
        static AudioManager instance;
        return instance;
    }

    AudioManager::~AudioManager() {
        Shutdown();
    }

    // System initialization
    bool AudioManager::Initialize(int maxChannels) {
        if (m_initialized) return true;

        FMOD_RESULT result = FMOD::System_Create(&m_system);
        FMODCheckError(result);  // No message parameter

        result = m_system->init(maxChannels, FMOD_INIT_NORMAL, nullptr);
        FMODCheckError(result);  // No message parameter

        m_initialized = true;
        return true;
    }

    void AudioManager::Update() {
        if (m_initialized) m_system->update();
    }

    void AudioManager::Shutdown() {
        if (!m_initialized) return;

        // Release all channels
        for (auto& channelPair : m_channels) {
            if (channelPair.second.channel) {
                channelPair.second.channel->stop();
                channelPair.second.channel = nullptr;
            }
        }
        m_channels.clear();

        // Release all sounds
        for (auto& soundPair : m_sounds) {
            if (soundPair.second.sound) {
                soundPair.second.sound->release();
                soundPair.second.sound = nullptr;
            }
        }
        m_sounds.clear();

        // Cleanup system
        if (m_system) {
            m_system->close();
            m_system->release();
            m_system = nullptr;
        }

        m_initialized = false;
    }

    // Listener configuration
    void AudioManager::SetListenerAttributes(const glm::vec3& position,
        const glm::vec3& velocity,
        const glm::vec3& forward,
        const glm::vec3& up) {
        if (!m_initialized) return;

        FMOD_VECTOR fmodPos = glmToFMOD(position);
        FMOD_VECTOR fmodVel = glmToFMOD(velocity);
        FMOD_VECTOR fmodForward = glmToFMOD(forward);
        FMOD_VECTOR fmodUp = glmToFMOD(up);

        m_system->set3DListenerAttributes(0, &fmodPos, &fmodVel, &fmodForward, &fmodUp);
    }

    // 2D Audio implementation
    bool AudioManager::Load2DSound(const std::string& alias, const std::string& path, bool loop) {
        if (!m_initialized) return false;
        if (m_sounds.count(alias)) return true;

        FMOD_MODE mode = FMOD_DEFAULT;
        if (loop) mode |= FMOD_LOOP_NORMAL;

        SoundData newSound;
        FMOD_RESULT result = m_system->createSound(path.c_str(), mode, nullptr, &newSound.sound);
        FMODCheckError(result);  // No message parameter

        newSound.is3D = false;
        newSound.isLooping = loop;
        m_sounds[alias] = newSound;
        return true;
    }

    void AudioManager::Play2DSound(const std::string& alias, float volume) {
        if (!m_initialized || !m_sounds.count(alias)) return;

        FMOD::Channel* channel = nullptr;
        FMOD_RESULT result = m_system->playSound(m_sounds[alias].sound, nullptr, false, &channel);
        FMODCheckError(result);

        channel->setVolume(volume);
        m_channels[alias] = { channel, false };
    }

    // 3D Audio implementation
    bool AudioManager::Load3DSound(const std::string& alias, const std::string& path, bool loop) {
        if (!m_initialized) return false;
        if (m_sounds.count(alias)) return true;

        FMOD_MODE mode = FMOD_3D | FMOD_3D_LINEARROLLOFF;
        if (loop) mode |= FMOD_LOOP_NORMAL;

        SoundData newSound;
        FMOD_RESULT result = m_system->createSound(path.c_str(), mode, nullptr, &newSound.sound);
        FMODCheckError(result);

        newSound.is3D = true;
        newSound.isLooping = loop;
        m_sounds[alias] = newSound;
        return true;
    }

    void AudioManager::Play3DSound(const std::string& alias,
        const glm::vec3& position,
        const glm::vec3& velocity,
        float volume) {
        if (!m_initialized || !m_sounds.count(alias)) return;

        FMOD::Channel* channel = nullptr;
        FMOD_RESULT result = m_system->playSound(m_sounds[alias].sound, nullptr, true, &channel);
        FMODCheckError(result);

        FMOD_VECTOR pos = glmToFMOD(position);
        FMOD_VECTOR vel = glmToFMOD(velocity);
        channel->set3DAttributes(&pos, &vel);
        channel->setVolume(volume);
        channel->setPaused(false);

        m_channels[alias] = { channel, true };
    }

    // One-shot sounds implementation
    void AudioManager::PlayOneShot3D(const std::string& path,
        const glm::vec3& position,
        const glm::vec3& velocity,
        float volume) {
        if (!m_initialized) return;

        FMOD::Sound* sound = nullptr;
        FMOD_MODE mode = FMOD_3D | FMOD_3D_LINEARROLLOFF | FMOD_CREATECOMPRESSEDSAMPLE;

        FMODCheckError(m_system->createSound(path.c_str(), mode, nullptr, &sound),
            "One-shot sound creation failed");

        FMOD::Channel* channel = nullptr;
        FMODCheckError(m_system->playSound(sound, nullptr, true, &channel));
        sound->release();


        FMOD_VECTOR pos = glmToFMOD(position);
        FMOD_VECTOR vel = glmToFMOD(velocity);
        channel->set3DAttributes(&pos, &vel);
        channel->setVolume(volume);
        channel->setPaused(false);

        // Auto-release when finished
        channel->setCallback([](FMOD_CHANNELCONTROL* ctrl, FMOD_CHANNELCONTROL_TYPE type,
            FMOD_CHANNELCONTROL_CALLBACK_TYPE cbtype, void*, void*) {
                if (type == FMOD_CHANNELCONTROL_CHANNEL && cbtype == FMOD_CHANNELCONTROL_CALLBACK_END) {
                    FMOD::Sound* sound;
                    ((FMOD::Channel*)ctrl)->getCurrentSound(&sound);
                    if (sound) sound->release();
                }
                return FMOD_OK;
            });
    }

    // Helper functions
    FMOD_VECTOR AudioManager::glmToFMOD(const glm::vec3& vec) {
        return { vec.x, vec.y, vec.z };
    }

} // namespace audio