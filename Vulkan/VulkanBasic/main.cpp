#include <Core/Log/Log.h>
#include <Core/Event/Event.h>

#include <VulkanCore/Core.h>
#include <VulkanCore/VulkanWindow.h>
#include <VulkanCore/VulkanRenderer.h>

bool running = true;

class EventListener
{
public:
	void onEvent(Event& e)
	{
		if (e.isInCategory(EventCategoryWindow))
		{
			if (e.getType() == EventType::WindowClose)
			{
				running = false;
			}
		}
	}
};

int main()
{
    Log::Init();
	EventListener eventListener;
    LOG_INFO("Vulkan Basic");
    
	WindowProps windowProps{};
    windowProps.title = "Vuklan Basic";
#ifdef _DEBUG
	windowProps.debug = true;
#endif

	VulkanWindow window(windowProps);
	window.setEventCallback(std::bind(&EventListener::onEvent, &eventListener, std::placeholders::_1));
    VulkanRenderer* renderer = new VulkanRenderer(window);

	while (running)
	{
		window.update();
	}

    delete renderer;

    return 0;
}
