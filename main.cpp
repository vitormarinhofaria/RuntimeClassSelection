#include <iostream>
#include <iomanip>

#include <thread>
#include <map>
#include <functional>
#include <chrono>

#include <entt/entt.hpp>

#include "NoVirtual.hpp"
#include "WithVirtual.hpp"
#include "Templated.hpp"

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
#define REGCLASS_TEMPLATED(type_name)                                                \
    RegisteredClasses[#type_name] = [](entt::registry &reg, entt::entity entity) {   \
        templated::NativeScriptComponent<type_name> component;                       \
        component.Bind<type_name>();                                                 \
        reg.emplace<templated::NativeScriptComponent<type_name>>(entity, component); \
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
    //REGCLASS_TEMPLATED(templated::Camera);

    entt::registry registry;

    for (auto i = 1; i < argc; i++)
    {
        entt::entity createdEntity = registry.create();
        AddClassComponentFunction addComponentToEntity = RegisteredClasses.at(argv[i]);
        addComponentToEntity(registry, createdEntity);
    }

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
    // auto viewTemplated = registry.view<templated::NativeScriptComponent<templated::Behavior>>();
    // for (auto [entity, scriptComponent] : viewTemplated.each())
    // {
    //     scriptComponent.m_Instance = scriptComponent.InstantiateScript();
    //     // scriptComponent.m_Instance->m_Entity = entity;
    //     // scriptComponent.m_Instance->OnCreate();
    //     // scriptComponent.O
    // }

    auto begin = std::chrono::steady_clock::now();
    // OPTICK_START_CAPTURE();
    while (true)
    {
        auto currentTime = std::chrono::steady_clock::now();
        auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - begin);
        begin = currentTime;

        auto view = registry.view<wivirt::NativeScriptComponent>();
        auto preTick = std::chrono::steady_clock::now();
        for (auto [entity, scriptComponent] : view.each())
        {
            scriptComponent.m_Instance->OnTick(deltaTime.count());
        }
        auto postTick = std::chrono::steady_clock::now();
        auto delta = std::chrono::duration_cast<std::chrono::microseconds>(postTick - preTick);
        std::cout << "WithVirt:: " << delta.count() << '\n';

        auto viewNoVirt = registry.view<novirt::NativeScriptComponent>();
        auto preTickNoVirt = std::chrono::steady_clock::now();
        for (auto [entity, scriptComponent] : viewNoVirt.each())
        {
            scriptComponent.m_Instance->OnTick(scriptComponent.m_Instance, deltaTime.count());
        }
        auto postTickNoVirt = std::chrono::steady_clock::now();
        auto deltaNoVirt = std::chrono::duration_cast<std::chrono::microseconds>(postTickNoVirt - preTickNoVirt);
        std::cout << "NoVirt:: " << deltaNoVirt.count() << '\n';

        auto viewWivirt = registry.view<wivirt::NativeScriptComponent>();
        auto preTickWivirt = std::chrono::steady_clock::now();
        for (auto [entity, scriptComponent] : viewWivirt.each())
        {
            scriptComponent.m_Instance->OnTick(deltaTime.count());
        }
        auto postTickWivirt = std::chrono::steady_clock::now();
        auto deltaWivirt = std::chrono::duration_cast<std::chrono::microseconds>(postTickWivirt - preTickWivirt);
        std::cout << "Wivirt:: " << deltaNoVirt.count() << '\n';

        // std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}