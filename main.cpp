#include <iostream>
#include <iomanip>

#include <thread>
#include <map>
#include <functional>
#include <chrono>

#include <entt/entt.hpp>

#include "NoVirtual.hpp"
#include "WithVirtual.hpp"

#define REGCLASS_VIRT(type_name)                                                   \
    RegisteredClasses[#type_name] = [](entt::registry &reg, entt::entity entity) { \
        wivirt::NativeScriptComponent component;                                   \
        component.Bind<type_name>();                                               \
        reg.emplace<wivirt::NativeScriptComponent>(entity, component);             \
    };
#define REGCLASS_NOVIRT(type_name)                                                 \
    RegisteredClasses[#type_name] = [](entt::registry &reg, entt::entity entity) { \
        novirt::NativeScriptComponent component;                                   \
        component.Bind<type_name>();                                               \
        reg.emplace<novirt::NativeScriptComponent>(entity, component);             \
    };

typedef std::function<void(entt::registry &, entt::entity)> AddClassComponentFunction;
std::map<std::string, AddClassComponentFunction> RegisteredClasses;

int main(int argc, char **argv)
{
    std::cout << "Press Enter to begin\n";
    std::cin.get();

    REGCLASS_VIRT(wivirt::Camera);
    REGCLASS_VIRT(wivirt::Entity);
    REGCLASS_NOVIRT(novirt::Camera);

    entt::registry registry;

    entt::entity createdEntity = registry.create();
    AddClassComponentFunction addComponentToEntity = RegisteredClasses[argv[1]];
    addComponentToEntity(registry, createdEntity);

    auto view = registry.view<wivirt::NativeScriptComponent>();
    for (auto [entity, scriptComponent] : view.each())
    {
        scriptComponent.m_Instance = scriptComponent.InstantiateScript();
        scriptComponent.m_Instance->m_Entity = entity;
        scriptComponent.m_Instance->OnCreate();
    }
    auto viewNoVirt = registry.view<novirt::NativeScriptComponent>();
    for (auto [entity, scriptComponent] : viewNoVirt.each())
    {
        scriptComponent.m_Instance = scriptComponent.CreateInstance();
        scriptComponent.m_Instance->m_Entity = entity;
        scriptComponent.m_Instance->OnCreate(scriptComponent.m_Instance);
    }

    auto begin = std::chrono::steady_clock::now();
    // OPTICK_START_CAPTURE();
    while (true)
    {
        auto currentTime = std::chrono::steady_clock::now();
        auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - begin);
        begin = currentTime;

        auto view = registry.view<wivirt::NativeScriptComponent>();
        for (auto [entity, scriptComponent] : view.each())
        {
            auto preTick = std::chrono::steady_clock::now();
            scriptComponent.m_Instance->OnTick(deltaTime.count());
            auto postTick = std::chrono::steady_clock::now();
            auto delta = std::chrono::duration_cast<std::chrono::microseconds>(postTick - preTick);
            std::cout << delta.count() << '\n';
        }
        auto viewNoVirt = registry.view<novirt::NativeScriptComponent>();
        for (auto [entity, scriptComponent] : viewNoVirt.each())
        {
            auto preTick = std::chrono::steady_clock::now();
            scriptComponent.m_Instance->OnTick(scriptComponent.m_Instance, deltaTime.count());
            auto postTick = std::chrono::steady_clock::now();
            auto delta = std::chrono::duration_cast<std::chrono::microseconds>(postTick - preTick);
            std::cout << delta.count() << '\n';
        }

        // std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}