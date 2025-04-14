#include "pch.h"
#include "ComponentAudio.h"

ComponentAudio::ComponentAudio(Entity* entity) : Component(entity) {}

void ComponentAudio::AddSound(SoundType type, const std::string& file) {
    soundFiles[type] = file;
}

std::string ComponentAudio::GetSound(SoundType type) const {
    auto it = soundFiles.find(type);
    if (it != soundFiles.end()) {
        return it->second;
    }
    return "";
}

void ComponentAudio::PlaySound(SoundType type) {
    std::string soundFile = GetSound(type);
    if (!soundFile.empty()) {
        HEAD("AUDIO") << "Playing sound: " /*<< soundFile */<< ENDL;
    }
}
