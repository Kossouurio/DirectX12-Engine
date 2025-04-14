#pragma once

class ScriptManager {
private:
    std::vector<Script*> scripts;

public:
    ~ScriptManager();

    void AddScript
    
    (Script* script);

    void Update(float dt);
};
