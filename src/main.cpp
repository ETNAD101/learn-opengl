#include <iostream>
#include <vector>

#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <IMGUI/imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "logger.h"
#include "shader.h"
#include "camera.h"
#include "stb_image.h"
#include "settings.h"

/*
 * TODO
 * add comments
 * Cleanup code
 * Make light cube colour change with light colour
*/

// Initializing world pieces
struct Cube {
    glm::vec3 scale;
    glm::vec3 pos;
};

Camera camera = Camera();

glm::vec3 lightPos(2.0f, 1.0f, 1.5f);
std::vector<Cube> cubes;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool mouse_enabled = false;
bool mouse_toggle = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double p_xPos, double p_yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void processInput(GLFWwindow* window);
unsigned int generateTexture(const char* path, unsigned int format, bool flip);
void spawnCube(float s_x, float s_y, float s_z);


int main() {
    // Init
    Logger logger = Logger("MAIN");
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //line needed to run on OS X
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Creating window
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE, NULL, NULL);
    if (window == NULL) {
        logger.setErr();
        logger.post("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Load glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        logger.setErr();
        logger.post("Failed to initialize GLAD");
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Load ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Create shader programs
    Shader lightingShader("../shaders/lightingShader.vert", "../shaders/lightingShader.frag");
    Shader lightCubeShader("../shaders/lightingShader.vert", "../shaders/lightCube.frag");

    // Cube verticies and normals
    float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    // Load verticies to GPU
    unsigned int VBO, modelVAO;
    glGenVertexArrays(1, &modelVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(modelVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Create starting cube
    cubes.push_back(Cube{glm::vec3(1.0, 1.0, 1.0), glm::vec3(0.0, 0.0, 0.0)});

    // Init more variables
    glm::mat4 model = glm::mat4(1.0f);

    float scale_x = 1;
    float scale_y = 1;
    float scale_z = 1;

    // Main loop
    while(!glfwWindowShouldClose(window)) {
        // Delta time calculation
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Start ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Main GUI panel
        ImGui::Begin("Spawn Panel");
        ImGui::Text("Scale");
        ImGui::SliderFloat("X", &scale_x, 1, 10);
        ImGui::SliderFloat("Y", &scale_y, 1, 10);
        ImGui::SliderFloat("Z", &scale_z, 1, 10);
        if (ImGui::Button("Spawn"))
            spawnCube(scale_x, scale_y, scale_z);
        ImGui::Text("FPS: %f", 1000 / deltaTime);
        ImGui::End();

        // Main OpenGL loop
        processInput(window);

        // Background colour
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.update_view();

        lightPos = glm::vec3(cos(currentFrame) * 2, cos(currentFrame) / 2 * 1.5, sin(currentFrame) * 2);

        // Set cube material
        lightingShader.use();
        lightingShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
        lightingShader.setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
        lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        lightingShader.setFloat("material.shininess", 32.0f);

        // Set light 
        lightingShader.setVec3("light.position", lightPos);
        lightingShader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        lightingShader.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        lightingShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

        lightingShader.setVec3("viewPos", camera.pos);

        // Render all cubes 
        for (Cube cube : cubes) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cube.pos);
            model = glm::scale(model, cube.scale);
            lightingShader.setMat4("model", model);
            lightingShader.setMat4("view", camera.view);
            lightingShader.setMat4("projection", camera.projection);

            // Draw normal cubes
            glBindVertexArray(modelVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Render light
        lightCubeShader.use();

        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));

        lightCubeShader.setMat4("model", model);
        lightCubeShader.setMat4("view", camera.view);
        lightCubeShader.setMat4("projection", camera.projection);

        // Draw light cube
        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Draw Gui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Display buffer
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glDeleteVertexArrays(1,&modelVAO);
    glDeleteBuffers(1, &VBO);


    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double p_xPos, double p_yPos) {
    if (!mouse_enabled) {
        float xPos = static_cast<float>(p_xPos);
        float yPos = static_cast<float>(p_yPos);
        camera.handle_mouse(xPos, yPos);
    }
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
    camera.fov -= (float)yOffset;
    if (camera.fov < 1.0f)
        camera.fov = 1.0f;
    if (camera.fov > FOV)
        camera.fov = FOV;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
        mouse_toggle = true;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && mouse_toggle) {
        mouse_toggle = false;
        if (mouse_enabled) {
            mouse_enabled = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        } else {
            mouse_enabled = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    float cam_speed = CAMERA_SPEED * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.move(FORWARD, cam_speed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.move(BACKWARD, cam_speed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.move(LEFT, cam_speed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.move(RIGHT, cam_speed);
}

unsigned int generateTexture(const char* path, unsigned int format, bool flip) {
    unsigned int tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(flip);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        Logger logger = Logger("GEN_TEXTURE");
        logger.setAlert();
        logger.post("Failed to load texture");
    }
    stbi_image_free(data);
    return tex;
}

void spawnCube(float s_x, float s_y, float s_z) {
    cubes.push_back(Cube{glm::vec3{s_x, s_y, s_z}, camera.pos});
}
