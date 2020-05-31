// g++ Main.cpp -o run -lglfw3 -lopengl32 -lglu32  -lgdi32 -lglew32 -lfreeglut -lsoil2

#include "Main.h"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int windowWidth = 1280, windowHeight = 720;

// todo: add listeners and break apart file into multiple, preferably hpp files? not sure if good practice.

int main() {
    glfwInit(); // initialize glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // using glfw 3.x
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // using glfw x.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // using core glfw functions
    glfwWindowHint(GLFW_RESIZABLE, false);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // for iOS
#endif

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "What", nullptr, nullptr);
    if(window == nullptr) { // check if glfw is ok
        std::cout << "uh glfw broke" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // select the window

    glewInit();
    glewExperimental = true;

    // viewport and callbacks(input)
    glViewport(0, 0, windowWidth, windowHeight);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback); // when window resized

    glEnable(GL_DEPTH_TEST); // enable z-axis depth

    // shader config
    Shader rectShader("C:\\Users\\kaleb\\Desktop\\3d\\src\\shaders\\vs_default.glsl", "C:\\Users\\kaleb\\Desktop\\3d\\src\\shaders\\fs_default.glsl");
    
    // side note: expirement with vertice values as warped

    // float vertices[] = { // vertices of rectangle (x, y, z)
    //     // vertex coords     // colors          // texture coords
    //     0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 1.0f,  1.0f, 1.0f, // top right
    //     0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  2.0f, 0.0f,  // bottom right
    //    -0.5f, -0.5f, 1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // bottom left
    //    -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f, // top left
    // };
    float vertices[] = { // cube vertices
        // vertex coords     texture coords
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

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    // create and load buffer and array objects to shade
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO); // only generating 1 VAO
    glGenBuffers(1, &VBO); // generate 1 VBO
    // glGenBuffers(1, &EBO); // generate 1 EBO

    glBindVertexArray(VAO); // bind the vao first
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // then load to the just binded array(vbo -> vao)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // setup draw for whatever vbo has to vao

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // glBindBuffers() is stored in element array buffers as it is required; so vao should be unbinded first later
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // setup draw for whatever ebo has to vao

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0); // using 3's for (x, y, z); sets how the vertices should be handled
    glEnableVertexAttribArray(0); // disable vao
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float))); // map color coords from array
    // glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float))); // map texture coords from array
    glEnableVertexAttribArray(1);

    // glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind VBO, but KEEPING ebo binded, as vao is still using ebo
    // glBindVertexArray(0); // unbind vao

    // texture loading
    unsigned int texture1;// texture2;
    float borderColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glGenTextures(1, &texture1);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("C:/Users/kaleb/Desktop/3d/src/assets/new.jpg", &width, &height, &nrChannels, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // just in case if width and height return segmentation fault
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    if(data != nullptr) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
         std::cout << "uh" << std::endl;
    }

    stbi_image_free(data);

    rectShader.use();
    rectShader.setInt("texture1", 0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // todo: create camera system tomorrow

    while(!glfwWindowShouldClose(window)) { // render loop(each iteration is a frame)
        // input callbacks
        processInput(window);

        // refresh frame
        glClearColor(0.9f, 1.0f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render things in between
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        // glActiveTexture(GL_TEXTURE1);
        // glBindTexture(GL_TEXTURE_2D, texture2);
        // glm::mat4 transform(glm::mat4(1.0f)); // make sure to initialize matrix to identity matrix first
        // transform = glm::rotate(transform, (float)glm::sin(glfwGetTime()*4), glm::vec3(0.0f, 0.0f, 1.0f));
        // transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));

        // start drawing by enabling shader
        rectShader.use();

        // making 3d matrices
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        model = glm::rotate(model, 5*glm::sin((float)glfwGetTime()), glm::vec3(1.0f, 1.0f, 1.0f));
        view = glm::translate(view, glm::vec3(3*glm::cos((float)glfwGetTime()), -1.0f, -10.0f));
        projection = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);

        unsigned int modelLoc = glGetUniformLocation(rectShader.ID, "model");
        unsigned int viewLoc = glGetUniformLocation(rectShader.ID, "view");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

        rectShader.setMat4("projection", projection);

        // glUniformMatrix4fv(glGetUniformLocation(rectShader.ID, "transform"), 1, GL_FALSE, glm::value_ptr(transform));

        // draw
        glBindVertexArray(VAO); // repeatedly bind vao for triangle
        glDrawArrays(GL_TRIANGLES, 0, 36);
        view = glm::translate(view, glm::vec3(3*glm::sin((float)glfwGetTime()), -3.0f, -10.0f)); // another cube
        // glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        // rectShader.setMat4("projection", projection);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // end render
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // destroy vao, vbo, then glfw to clear memory
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) { // call when window resized
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}