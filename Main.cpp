#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{   
    if (width >= 400 && height >= 300) {
        glViewport(0, 0, width, height);
    }
}
void boundCheck(GLFWwindow* window);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
char readMazeMap();
bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Game", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, 800, 600);

    //map loading
 



    float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 0.0f
    };

    unsigned int EBO;
    glGenBuffers(1, &EBO);
 
    //bind VAO 1
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    //texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load("bricks.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    //VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //binding VAO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    //map gen
    int mapsize = 32;
    int mx, my;
    char map[32][32];
    unsigned char* maze = stbi_load("map.png", &mx, &my, NULL, 1);
    for (int i = 0; i < mapsize; i++) {
        for (int j = 0; j < mapsize; j++) {
            stbi_uc pixel = maze[mx * i + j];
            if (pixel > 200) {
                map[i][j] = '#';
            }
            else if (pixel < 200 && pixel > 190) {
                map[i][j] = '@';
            }
            else if (pixel > 0 && pixel < 185) {
                map[i][j] = '%';
            }
            else {
                map[i][j] = ' ';
            }
        }
    }
    for (int i = 0; i < my; i++) {
        for (int j = 0; j < mx; j++) {
            std::cout << map[i][j];
        }
        std::cout << std::endl;
    }
    stbi_image_free(maze);
    std::vector<glm::vec3> cubes;
    cubes.clear();
    cubes.reserve(mapsize*mapsize);
    for (int i = 0; i < mapsize; i++) {
        for (int j = 0; j < mapsize; j++) {
            if ((char)map[i][j] == '#') {
                cubes.emplace_back(
                    static_cast<float>(j - mapsize / 2),
                    0.0f,
                    static_cast<float>(i - mapsize / 2)
                );
            }
        }
    }
    //player set
    for (int i = 0; i < mapsize; i++) {
        for (int j = 0; j < mapsize; j++) {
            if ((char)map[i][j] == '%') {
                cameraPos = glm::vec3(static_cast<float>(j - mapsize / 2), 0.0f, static_cast<float>(i - mapsize / 2));
            }
        }
    }
    //cam

    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDirectionOpp = glm::normalize(cameraPos - cameraTarget);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirectionOpp));


    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    Shader ourShader("shader.vert", "shader.frag");
    ourShader.use();
    glm::vec3 prPos;
    prPos = cameraPos;
    glEnable(GL_DEPTH_TEST);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.7f, 0.7f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // activate shader
        ourShader.use();
        boundCheck(window);
        glfwSetCursorPosCallback(window, mouseCallback);
        for (int i = 0; i < cubes.size(); i++) {
            if (cameraPos[0] >= cubes[i][0]-0.6f && cameraPos[0] <= cubes[i][0] + 0.6f && cameraPos[2] >= cubes[i][2] - 0.6f && cameraPos[2] <= cubes[i][2] + 0.6f) {
                cameraPos = prPos;
            }
        }
        glm::mat4 projection = glm::mat4(1.0f);

        projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);

        ourShader.setMat4("projection", projection);

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        ourShader.setMat4("view", view);
        
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.

        // render box
        for (int i = 0; i < mapsize; i++) {
            for (int j = 0; j < mapsize; j++) {
                if ((char)map[i][j] == '@' && cameraPos[0] >= j - mapsize / 2 - 0.5f && cameraPos[0] <= j - mapsize / 2 + 0.5f && cameraPos[2] >= i - mapsize / 2 - 0.5f && cameraPos[2] <= i - mapsize / 2 + 0.5f) {
                    glClearColor(0.0f, 0.7f, 0.0f, 1.0f);
                    std::cout << "YOU'VE WON!!!";
                    glfwSetWindowShouldClose(window, true);
                    system("pause");
                }
            }
        }
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < cubes.size(); i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubes[i]);
            ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        prPos = cameraPos;

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
	return 0;
}
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
}
void mouseCallback(GLFWwindow* window, double xpos, double ypos){
float xoffset = xpos - lastX;
float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
lastX = xpos;
lastY = ypos;

const float sensitivity = 0.1f;
xoffset *= sensitivity;
yoffset *= sensitivity;

yaw += xoffset;
pitch += yoffset;
if (pitch > 0.0f) {
pitch = 0.0f;
}

if (pitch < 0.0f) {
pitch = 0.0f;
}
glm::vec3 direction;
direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
direction.y = sin(glm::radians(pitch));
direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
cameraFront = glm::normalize(direction);
}
void boundCheck(GLFWwindow* window){
    if (cameraPos[1] != 0.0f) {
        cameraPos[1] = 0.0f;
    }
}
