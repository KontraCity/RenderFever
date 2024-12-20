#include "rf/graphics/window.hpp"

// Custom engine modules
#include "rf/engine/input.hpp"
#include "rf/engine/utility.hpp"

// Custom graphics modules
#include "rf/graphics/cube.hpp"
#include "rf/graphics/lighting.hpp"
#include "rf/graphics/skybox.hpp"

namespace rf {
     
void Graphics::Window::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    Window* root = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    root->m_width = width;
    root->m_height = height;
    glViewport(0, 0, width, height);
}

Graphics::Window::Window(unsigned int width, unsigned int height)
    : m_logger(Engine::Utility::CreateLogger("window"))
    , m_width(width)
    , m_height(height)
{
    // Initialize GLFW
    if (glfwInit() != GLFW_TRUE)
        throw std::runtime_error("rf::Graphics::Window::Window(): Couldn't initialize GLFW");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create GLFW window
    m_window = glfwCreateWindow(m_width, m_height, "RenderFever Engine", NULL, NULL);
    if (!m_window)
    {
        glfwTerminate();
        throw std::runtime_error("rf::Graphics::Window::Window(): Couldn't create GLFW window");
    }
    glfwMakeContextCurrent(m_window);

    // Configure GLFW
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, &Window::FrameBufferSizeCallback);
    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) { Engine::Input::Key.broadcast(key, action, mods); });
    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double x, double y) { Engine::Input::CursorPos.broadcast(x, y); });
    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset) { Engine::Input::Scroll.broadcast(xOffset, yOffset); });
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    Engine::Input::Key.subscribe(std::bind(&Graphics::Window::onKey, this, _1, _2, _3));

    // Initializee GLEW
    GLenum result = glewInit();
    if (result != GLEW_OK)
    {
        glfwTerminate();
        throw std::runtime_error(fmt::format(
            "rf::Graphics::Window::Window(): Couldn't initialize GLEW: \"{}\"",
            reinterpret_cast<const char*>(glewGetErrorString(result)
        )));
    }

    // Configure OpenGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);
    glViewport(0, 0, m_width, m_height);

    try
    {
        // Make shaders
        Engine::Stopwatch stopwatch;
        m_shader.make("shaders/vertex/main.vert", "shaders/fragment/main.frag");
        m_lightingShader.make("shaders/vertex/lighting.vert", "shaders/fragment/lighting.frag");
        m_skyboxShader.make("shaders/vertex/skybox.vert", "shaders/fragment/skybox.frag");
        m_logger.info("[{:>5.3f} s] Shaders built", stopwatch.seconds());

        // Load textures
        stopwatch.reset();
        m_containerMaterial.diffuse() = std::make_shared<Texture>("textures/container/texture.png", Texture::Type::Diffuse);
        m_containerMaterial.specular() = std::make_shared<Texture>("textures/container/specular.png", Texture::Type::Specular);
        m_containerMaterial.shininess() = 32.0f;
        m_skyboxCubemap = std::make_shared<Cubemap>("textures/skybox");
        m_logger.info("[{:>5.3f} s] Textures loaded", stopwatch.seconds());

        // Load models
        stopwatch.reset();
        m_backpack.load("models/backpack.glb");
        m_logger.info("[{:>5.3f} s] Models loaded", stopwatch.seconds());
    }
    catch (...)
    {
        glfwTerminate();
        throw;
    }
}

Graphics::Window::~Window()
{
    glfwTerminate();
}

void Graphics::Window::onKey(int key, int action, int mods)
{
    switch (key)
    {
        case GLFW_KEY_ESCAPE:
        {
            if (action == GLFW_PRESS)
                glfwSetWindowShouldClose(m_window, true);
            break;
        }
        case GLFW_KEY_TAB:
        {
            if (action == GLFW_PRESS)
                toggleWireframe();
            break;
        }
        case GLFW_KEY_Q:
        {
            if (action == GLFW_PRESS)
                toggleVSync();
            break;
        }
        case GLFW_KEY_T:
        {
            if (action == GLFW_PRESS)
                toggleAntiAliasing();
            break;
        }
        case GLFW_KEY_F:
        {
            if (action == GLFW_PRESS)
                m_flashlight = !m_flashlight;
            break;
        }
    }
}

void Graphics::Window::toggleWireframe()
{
    static bool wireframe = false;
    wireframe = !wireframe;
    glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
}

void Graphics::Window::toggleVSync()
{
    static bool enabled = true;
    enabled = !enabled;
    glfwSwapInterval(static_cast<int>(enabled));
}

void Graphics::Window::toggleAntiAliasing()
{
    static bool enabled = true;
    enabled = !enabled;
    enabled ? glEnable(GL_MULTISAMPLE) : glDisable(GL_MULTISAMPLE);
}

void Graphics::Window::showFps() const
{
    float fps = 1.0f / m_deltaTime;
    static float min = fps;
    static float max = fps;
    static float resetTime = -1.0f;

    if (resetTime == -1 || m_currentFrameTime - resetTime > 3.0f)
    {
        min = max = fps;
        resetTime = m_currentFrameTime;
    }

    if (fps < min)
        min = fps;
    if (fps > max)
        max = fps;
    fmt::print("FPS: {:>6.1f} (min/max for 3s: {:>6.1f}, {:6.1f})\r", fps, min, max);
}

void Graphics::Window::run()
{
    // Sun
    Lighting::DirectionalLight sun;
    sun.properties() = { 0.2f, 0.9f, 1.0f };
    sun.direction() = { 1.0f, -1.0f, -1.0f };

    // Flashlight
    Lighting::SpotLight flashlight;
    flashlight.cutoff() = { 20, 25 };

    // Backpack
    m_backpack.transform().position().y = 3.0f;

    // Container
    Cube container;
    container.material() = m_containerMaterial;

    // Skybox
    Skybox skybox;
    skybox.cubemap() = m_skyboxCubemap;

    m_stopwatch.reset();
    while (!glfwWindowShouldClose(m_window))
    {
        // Calculate times
        m_currentFrameTime = m_stopwatch.seconds();
        m_deltaTime = m_currentFrameTime - m_lastFrameTime;
        m_lastFrameTime = m_currentFrameTime;
        showFps();

        // Prepare
        Engine::Input::Input.broadcast(m_window, m_deltaTime);
        glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Transform
        flashlight.position() = m_camera.position();
        flashlight.direction() = m_camera.direction();
        flashlight.color() = m_flashlight ? Color{ 255, 255, 255 } : Color{ 0, 0, 0 };

        // Illuminate
        sun.illuminate(m_shader);
        flashlight.illuminate(m_shader);

        // Draw
        container.draw(m_shader);
        m_backpack.draw(m_shader);

        // Skybox
        skybox.draw(m_skyboxShader);

        // Done
        m_camera.capture(m_shader, m_lightingShader, m_skyboxShader, m_width, m_height);
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
    m_logger.warn("{:<20}", "Stopped");
}

} // namespace rf
