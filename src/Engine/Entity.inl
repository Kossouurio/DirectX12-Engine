template <typename T, typename... Args>
T* Entity::AddComponent(Args&&... args)
{
    T* component = new T(this, std::forward<Args>(args)...);
    m_components.push_back(component);

    if (debugMode && logComponent) {
        HEAD("COMPONENT") << typeid(T).name() << " ADDED to entity " << m_id << ENDL;
    }

    return component;
}


template <typename T>
T* Entity::GetComponent() {
    for (auto* component : m_components) {
        if (T* casted = dynamic_cast<T*>(component)) {
            return casted;
        }
    }
    return nullptr;
}

template <typename T>
void Entity::RemoveComponent()
{
    for (auto it = m_components.begin(); it != m_components.end();)
    {
        if (T* casted = dynamic_cast<T*>(*it))
        {
            if (debugMode && logComponent) {
                HEAD("COMPONENT") << "Component" << typeid(T).name() << " REMOVED to entity " << m_id << ENDL;
            }

            delete* it;  
            it = m_components.erase(it);
        }
        else {
            ++it;
        }
    }
}

