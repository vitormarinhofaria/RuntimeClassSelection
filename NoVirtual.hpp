#include <iostream>
#include <entt/entt.hpp>

namespace novirt
{
    struct Behavior
    {
        entt::entity m_Entity;
        void (*OnTick)(Behavior *, float);
        void (*OnCreate)(Behavior *);
        void (*OnDestroy)(Behavior *);
        Behavior()
        {
            std::cout << "Calling NoVirtual Behavior constructor\n";
            OnTick = [](Behavior *, float tick) {};
            OnCreate = [](Behavior *) {};
            OnDestroy = [](Behavior *) {};
        }
    };

    struct NativeScriptComponent
    {
        Behavior *m_Instance;

        Behavior *(*CreateInstance)();
        void (*DestroyInstance)(NativeScriptComponent *);

        template <typename T>
        void Bind()
        {
            CreateInstance = []()
            {
                return static_cast<Behavior *>(new T());
            };
            DestroyInstance = [](NativeScriptComponent *self)
            {
                delete (T*)self->m_Instance;
                self->m_Instance = nullptr;
            };
        }
    };

    struct Camera : public Behavior
    {
        void Tick(float deltaTime)
        {
            std::cout << "Camera::Tick NoVirtual...\n";
        }
        void Create()
        {
            std::cout << "Camera::Create NoVirtual...\n";
        }
        void Destroy()
        {
            std::cout << "Camera::Destroy NoVirtual...\n";
        }
        Camera()
        {
            std::cout << "Calling Camera NoVirtual Constructor\n";
            OnTick = [](Behavior *selfBase, float deltaTime)
            {
                auto *self = (Camera *)selfBase;
                self->Tick(deltaTime);
            };
            OnCreate = [](Behavior *selfBase)
            {
                auto *self = (Camera *)selfBase;
                self->Create();
            };
            OnDestroy = [](Behavior *selfBase)
            {
                auto *self = (Camera *)selfBase;
                self->Destroy();
            };
        }
    };
}