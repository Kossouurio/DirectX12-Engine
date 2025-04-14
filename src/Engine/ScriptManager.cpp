#include "pch.h"
#include "ScriptManager.h"

ScriptManager::~ScriptManager() {
    for (auto* script : scripts) {
        delete script;
    }
    scripts.clear();
}

void ScriptManager::AddScript(Script* script) {
    scripts.push_back(script);
    script->Start();
}

void ScriptManager::Update(float dt) {
    for (auto* script : scripts) {
        script->Update(dt);
    }
}