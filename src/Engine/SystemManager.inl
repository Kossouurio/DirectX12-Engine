template <typename T, typename... Args>
T* SystemManager::AddSystem(Args&&... args) {
    static_assert(std::is_base_of<System, T>::value, "T doit être un sous-type de System");
    T* system = new T(std::forward<Args>(args)...);
    systems.push_back(system);
    return system;
}

template <typename T>
T* SystemManager::GetSystem() {
    for (auto* system : systems) {
        if (T* castedSystem = dynamic_cast<T*>(system)) {
            return castedSystem;
        }
    }
    return nullptr;
}