#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/gtc/matrix_transform.hpp"

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error with glew init !" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    {

        float positions[] = {
            100.0f, 100.0f, 0.0f, 0.0f,
            150.0f, 100.0f, 1.0f, 0.0f,
            150.0f, 159.0f, 1.0f, 1.0f,
            100.0f, 159.0f, 0.0f, 1.0f
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));

        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 0.0f, 0.0f));
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(300.0f, 300.0f, 0.0f));

        glm::mat4 mvp = proj * view * model;

        Shader shader("Basic.shader");
        shader.Bind();
        shader.SetUniformMat4f("u_MVP", mvp);

        Texture texture("res/img.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0); // 0 Because the texture is bound to slot 0 (default)

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;

        while (!glfwWindowShouldClose(window))
        {
            renderer.clear();
            renderer.Draw(va, ib, shader);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}