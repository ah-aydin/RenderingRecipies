#define GLFW_INCLUDE_NONE

#include <Core/Event/Event.h>
#include <Core/Log/Log.h>
#include <OpenGLCore/OpenGLWindow.h>
#include <OpenGLCore/OpenGLProgram.h>
#include <OpenGLCore/OpenGLVertexArray.h>
#include <OpenGLCore/OpenGLVertexBuffer.h>

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
    LOG_INFO("I AM ALIVE AND WELL");
	
	WindowProps windowProps{};
    windowProps.title = "OpenGL Basic";
#ifdef _DEBUG
	windowProps.debug = true;
#endif

	Window window(windowProps);
	window.setEventCallback(std::bind(&EventListener::onEvent, &eventListener, std::placeholders::_1));

	OpenGLProgram program("res/shader.vert", "res/shader.frag");
	program.use();

	std::vector<float> vertices = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0, 1.0f
	};
	std::vector<uint32_t> indices = { 0, 1, 2, 2, 3, 0};
	VertexBufferLayout layout{ {
		{ VertexBufferType::Float3, "aPos" },
        { VertexBufferType::Float3, "aColor"},
        { VertexBufferType::Float2, "aTexCoord"}
	}};
	OpenGLVertexArray vertexArray(vertices, layout, indices);
	vertexArray.bind();

	while (running)
	{
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDrawElements(GL_TRIANGLES, vertexArray.getIndexCount(), GL_UNSIGNED_INT, 0);

		window.update();
	}
	return 0;
}
