#pragma once
#include "System.h"

class System;

class SystemManager {
private:
    std::vector<System*> systems;

public:
    ~SystemManager();

    template <typename T, typename... Args>
    T* AddSystem(Args&&... args);

    template <typename T>
    T* GetSystem();

    void Update(float dt);
};

#include "SystemManager.inl"