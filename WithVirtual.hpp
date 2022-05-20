#include <iostream>
#include <entt/entt.hpp>

namespace wivirt
{
    struct Behavior
    {
        entt::entity m_Entity;
        virtual void OnTick(float){};
        virtual void OnCreate(){};
        virtual void OnDestroy(){};
    };

    struct NativeScriptComponent
    {
        Behavior *m_Instance = nullptr;

        Behavior *(*InstantiateScript)();
        void (*DestroyScript)(NativeScriptComponent *);

        template <typename T>
        void Bind()
        {
            InstantiateScript = []()
            { return static_cast<Behavior *>(new T()); };
            DestroyScript = [](NativeScriptComponent *nsc)
            { delete nsc->m_Instance; nsc->m_Instance = nullptr; };
        }
    };

    struct Camera : public Behavior
    {
        void OnTick(float t)
        {
            std::cout << "Camera::OnTick WithVirt \n";
        }
        void OnCreate()
        {
            std::cout << "Camera::OnCreate WithVirt\n";
        }
        void OnDestroy()
        {
            std::cout << "Camera::OnDestroy WithVirt\n";
        }
    };

    struct Entity : public Behavior
    {
        void OnTick(float t)
        {
            std::cout << "Entity::OnTick WithVirt\n";
        }
        void OnCreate()
        {
            std::cout << "Entity::OnCreate WithVirt\n";
        }
        void OnDestroy()
        {
            std::cout << "Entity::OnDestroy WithVirt\n";
        }
    };
}