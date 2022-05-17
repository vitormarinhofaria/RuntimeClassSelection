#include <iostream>
#include <iomanip>

#include <thread>
#include <map>
#include <functional>
#include <chrono>

#include <entt/entt.hpp>

#define REGCLASS(type_name)                                            \
    RegisteredClasses[#type_name] = [](entt::registry &reg, entt::entity entity) { \
        NativeScriptComponent component;                               \
        component.Bind<type_name>();                                   \
        reg.emplace<NativeScriptComponent>(entity, component);         \
    };

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
        std::cout << "Camera::OnTick Delta: " << std::fixed << std::setprecision(2) << t << "ms\n";
    }
    void OnCreate()
    {
        std::cout << "Camera::OnCreate\n";
    }
    void OnDestroy()
    {
        std::cout << "Camera::OnDestroy\n";
    }
};

struct Entity : public Behavior
{
    void OnTick(float t)
    {
        std::cout << "Entity::OnTick Delta: " << std::fixed << std::setprecision(2) << t << "ms\n";
    }
    void OnCreate()
    {
        std::cout << "Entity::OnCreate\n";
    }
    void OnDestroy()
    {
        std::cout << "Entity::OnDestroy\n";
    }
};
typedef std::function<void(entt::registry &, entt::entity)> AddClassComponentFunction;
std::map<std::string, AddClassComponentFunction> RegisteredClasses;

int main(int argc, char **argv)
{
    REGCLASS(Camera);
    REGCLASS(Entity);

    entt::registry registry;

    entt::entity createdEntity = registry.create();
    AddClassComponentFunction addComponentToEntity = RegisteredClasses[argv[1]];
    addComponentToEntity(registry, createdEntity);

    auto view = registry.view<NativeScriptComponent>();
    for (auto [entity, scriptComponent] : view.each())
    {
        scriptComponent.m_Instance = scriptComponent.InstantiateScript();
        scriptComponent.m_Instance->m_Entity = entity;
        scriptComponent.m_Instance->OnCreate();
    }

    auto begin = std::chrono::steady_clock::now();
    while (true)
    {
        auto currentTime = std::chrono::steady_clock::now();
        auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - begin);
        begin = currentTime;

        auto view = registry.view<NativeScriptComponent>();
        for (auto [entity, scriptComponent] : view.each())
        {
            scriptComponent.m_Instance->OnTick(deltaTime.count());
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}