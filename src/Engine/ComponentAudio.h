#pragma once
//#include "Component.h"

enum class SoundType {
    SHOOT,
    FOOTSTEP,
    DAMAGE
};

class ComponentAudio : public Component {
private:
    std::map<SoundType, std::string> soundFiles; 

public:
    ComponentAudio(Entity* entity);

    void AddSound(SoundType type, const std::string& file);
    std::string GetSound(SoundType type) const;

    void PlaySound(SoundType type);
};
