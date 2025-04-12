#pragma once

#include <fmod.hpp>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include "AudioUtils.hpp"



namespace audio {

    class AudioManager {
    public:
        // Singleton control
        static AudioManager& Instance();
        AudioManager(const AudioManager&) = delete;
        AudioManager& operator=(const AudioManager&) = delete;

        // Core system management
        bool Initialize(int maxChannels = 512);
        void Update();
        void Shutdown();

        // Listener configuration
        void SetListenerAttributes(const glm::vec3& position,
            const glm::vec3& velocity,
            const glm::vec3& forward,
            const glm::vec3& up);

        // 2D Audio operations
        bool Load2DSound(const std::string& alias, const std::string& path, bool loop = false);
        void Play2DSound(const std::string& alias, float volume = 1.0f);
        void Stop2DSound(const std::string& alias);

        // 3D Audio operations
        bool Load3DSound(const std::string& alias, const std::string& path, bool loop = false);
        void Play3DSound(const std::string& alias,
            const glm::vec3& position,
            const glm::vec3& velocity = glm::vec3(0.0f),
            float volume = 1.0f);
        void Update3DSoundPosition(const std::string& alias,
            const glm::vec3& newPosition,
            const glm::vec3& newVelocity = glm::vec3(0.0f));
        void Stop3DSound(const std::string& alias);

        // One-shot sounds
        void PlayOneShot2D(const std::string& path, float volume = 1.0f);
        void PlayOneShot3D(const std::string& path,
            const glm::vec3& position,
            const glm::vec3& velocity = glm::vec3(0.0f),
            float volume = 1.0f);

        // Global controls
        void SetMasterVolume(float volume);
        void PauseAll();
        void ResumeAll();

    private:
        AudioManager() = default;
        ~AudioManager();

        struct SoundData {
            FMOD::Sound* sound = nullptr;
            bool is3D = false;
            bool isLooping = false;
        };

        struct ChannelData {
            FMOD::Channel* channel = nullptr;
            bool is3D = false;
        };

        FMOD::System* m_system = nullptr;
        std::unordered_map<std::string, SoundData> m_sounds;
        std::unordered_map<std::string, ChannelData> m_channels;
        bool m_initialized = false;

        static FMOD_VECTOR glmToFMOD(const glm::vec3& vec);
    };

} // namespace audio