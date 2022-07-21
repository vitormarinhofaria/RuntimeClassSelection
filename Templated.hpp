#include <iostream>
#include <entt/entt.hpp>

namespace templated
{
    template<typename T>
    concept Behavior = requires(T t, float deltaTime)
    {
        t.OnTick(deltaTime);
        t.OnCreate();
        t.OnDestroy();
    };

    // struct Behavior
    // {
    //     entt::entity m_Entity;
    //     // virtual void OnTick(float){};
    //     // virtual void OnCreate(){};
    //     // virtual void OnDestroy(){};
    // };

    template<typename T> requires Behavior<T>
    struct NativeScriptComponent
    {
        T * m_Instance = nullptr;

        void *(*InstantiateScript)();
        void (*DestroyScript)(NativeScriptComponent *);

        void Tick(float deltaTime)
        {
            m_Instance->OnTick(deltaTime);
            m_Instance
        }
        void Create()
        {
            m_Instance->OnCreate();
        }
        void Destroy()
        {
            m_Instance->OnDestroy();
        }

        //template <Behavior T>
        void Bind()
        {
            InstantiateScript = []()
            { return (void*)new T(); };
            DestroyScript = [](NativeScriptComponent *nsc)
            { delete (T*)nsc->m_Instance; nsc->m_Instance = nullptr; };
        }
    };

    struct Camera
    {
        uint64_t counter = 0;
        void OnTick(float deltaTime)
        {
            counter++;
        }
        void OnCreate()
        {
            std::cout << "Camera::OnCreate WithTemplate\n";
        }
        void OnDestroy()
        {
            std::cout << "Camera::OnDestroy WithTemplate\n";
        }
    };

    struct Entity
    {
        void OnTick(float t)
        {
            std::cout << "Entity::OnTick WithTemplate\n";
        }
        void OnCreate()
        {
            std::cout << "Entity::OnCreate WithTemplate\n";
        }
        void OnDestroy()
        {
            std::cout << "Entity::OnDestroy WithTemplate\n";
        }
    };
}