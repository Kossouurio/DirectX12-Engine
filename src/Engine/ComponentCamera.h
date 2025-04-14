#pragma once


class ComponentCamera : public Component {
private:
    Camera* camera;

public:
    ComponentCamera(Entity* entity);

	void Logs() override;

    Camera* GetCamera() const;

    void SyncWithTransform();
};
