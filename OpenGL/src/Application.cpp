#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>

#include <Context.h>

#include <Window.h>
#include <Mesh.h>
#include <Shader.h>
#include <Camera.h>
#include <Texture.h>
#include <DirectionalLight.h>
#include <PointLight.h>
#include <SpotLight.h>
#include <Material.h>
#include <Model.h>


#define ASSERT(x) if(!(x)) __debugbreak();

void GLAPIENTRY GLErrorCallback(GLenum source,GLenum type,GLuint id,GLenum severity,
                                GLsizei length,const GLchar* message,const void* userParam)
{
    printf("OpenGL Error:\nSource: 0x%x\nType: 0x%x\n"
        "Id: 0x%x\nSeverity: 0x%x\n", source, type, id, severity);
    printf("%s\n", message);
    ASSERT(false);
}

//Apply a smooth shading effect to the model by averaging out the normals.
void CalculateAvgNormals(unsigned int* indices, unsigned int indiceCount, float* vertices,
    unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset) {
    for (int i = 0; i < indiceCount; i += 3) {
        unsigned int in0 = indices[i] * vLength;
        unsigned int in1 = indices[i + 1] * vLength;
        unsigned int in2 = indices[i + 2] * vLength;
        glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
        glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
        glm::vec3 normal = glm::cross(v1, v2);
        normal = glm::normalize(normal);

        in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;

        vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
        vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
        vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
    }

    for (int i = 0; i < verticeCount / vLength; i++) {
        unsigned int nOffset = i * vLength + normalOffset;
        glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
        vec = glm::normalize(vec);
        vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
    }
}

void GenerateObjects(std::vector<Mesh*> &list) {

    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    float vertices[] = {
    //	     x      y      z		 u	   v		  nx	ny    nz
           -1.0f, -1.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
            0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
            0.0f,  1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
    };

    float floorVertices[] = {
     //	  x      y      z		 u	   v		  nx	ny    nz
        -10.0f, 0.0f, -10.0f,    0.0f,  0.0f,   0.0f, -1.0f, 0.0f,
         10.0f, 0.0f, -10.0f,   10.0f,  0.0f,   0.0f, -1.0f, 0.0f,
        -10.0f, 0.0f,  10.0f,    0.0f, 10.0f,   0.0f, -1.0f, 0.0f,
         10.0f, 0.0f,  10.0f,   10.0f, 10.0f,   0.0f, -1.0f, 0.0f
    };

    unsigned int floorIndices[] = {
        0, 2, 1,
        1, 2, 3
    };

    CalculateAvgNormals(indices, 12, vertices, 32, 8, 5);

    Mesh* testMesh1 = new Mesh();
    testMesh1->CreateMesh(vertices, indices, 32, 12);
    list.push_back(testMesh1);

    Mesh* testMesh2 = new Mesh();
    testMesh2->CreateMesh(vertices, indices, 32, 12);
    list.push_back(testMesh2);

    Mesh* floor = new Mesh();
    floor->CreateMesh(floorVertices, floorIndices, 32, 6);
    list.push_back(floor);
}

void GenerateShaders(std::vector<Shader*>& list) {
    const char* vertexShader = "Shaders/vertex.shader";
    const char* fragmentShader = "Shaders/fragment.shader";
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vertexShader, fragmentShader);
    list.push_back(shader1);
}

int main(void) {
    const float M_PI = 3.14f;//3.14159265358979323846
    const float toRadians = M_PI / 180.0f;
    float deltaTime = 0.0f;
    float lastTime = 0.0f;

    std::vector<Mesh*> meshList;
    std::vector<Shader*> shaderList;

    Texture marbleTexture;
    Texture corkTexture;

    Material shinyMaterial;
    Material dullMaterial;

    Model knight;

    DirectionalLight sun;
    PointLight pointLights[MAX_POINT_LIGHTS];
    SpotLight spotLights[MAX_SPOT_LIGHTS];
    unsigned int pointLightCount = 0, spotLightCount = 0;

    Window* mainWindow = new Window(1920,1080,"Tureebluh's World");
    
    glm::mat4 projection;

    mainWindow->initialize();
    GenerateObjects(meshList);
    GenerateShaders(shaderList);

    Camera* camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 3.0f, 0.2f);

    marbleTexture = Texture("Textures/marble_1K_seamless.png");
    marbleTexture.LoadTexture(true);
    corkTexture = Texture("Textures/cork.png");
    corkTexture.LoadTexture(true);

    shinyMaterial = Material(1.0f, 32.0f);
    dullMaterial = Material(0.3f, 4.0f);

    knight = Model();
    knight.LoadModel("Models/knight.obj");

    sun = 
    DirectionalLight(   1.0f, 1.0f, 1.0f,               // R G B
                        0.05f,                          // Ambient Light Intensity
                        0.4f,                           // Diffuse Light Intensity
                        0.0f, 0.0f, -180.0f);           // X Y Z Direction  

    pointLights[pointLightCount] = 
    PointLight( 1.0f, 0.0f, 0.0f,               // R G B
                0.1f,                           // Ambient Light Intensity
                1.0f,                           // Diffuse Light Intensity
                -4.0f, 4.0f, 0.0f,              // X Y Z Position
                0.3f, 0.1f, 0.1f                // Constant = c, Linear = b, Exponent = a (ax^2 + bx + c) where X is our distance.
                ); pointLightCount++;

    spotLights[spotLightCount] = 
    SpotLight(  1.0f, 1.0f, 1.0f,               // R G B
                0.0f,                           // Ambient Light Intensity
                0.1f,                           // Diffuse Light Intensity
                0.0f, 0.0f, 0.0f,               // X Y Z Position
                0.0f, -5.0f, 0.0f,              // X Y Z Direction
                0.2f, 0.0f, 0.0f,               // Constant = c, Linear = b, Exponent = a (ax^2 + bx + c) where X is our distance.
                15.0f                           // Edge (degrees)
                ); spotLightCount++;
    
    unsigned int uniformModel = 0, uniformProjection = 0, uniformView = 0, uniformEyePosition = 0,
        uniformSpecularIntensity = 0, uniformShininess = 0;

    projection = glm::perspective(45.0f, mainWindow->GetBufferWidth() / mainWindow->GetBufferHeight(), 0.1f, 100.0f);

    /* Loop until the user closes the window */
    while (!mainWindow->getShouldClose()) {
        //Calculate deltatime
        float now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        /* Process events in input event queue (user input) */
        glfwPollEvents();
        camera->keyControl(mainWindow->getsKeys(), deltaTime);
        camera->mouseControl(mainWindow->getXChange(), mainWindow->getYChange());

        /* Render here */
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0]->UseShader();
        uniformModel = shaderList[0]->GetModelLocation();
        uniformProjection = shaderList[0]->GetProjectionLocation();
        uniformView = shaderList[0]->GetViewLocation();
        uniformEyePosition = shaderList[0]->GetEyePositionLocation();
        
        uniformSpecularIntensity = shaderList[0]->GetSpecularIntensityLocation();
        uniformShininess = shaderList[0]->GetShininessLocation();

        glm::vec3 lowerLight = camera->getCameraPosition();
        lowerLight.y -= 0.4f;
        spotLights[0].SetFlash(lowerLight, camera->getCameraDirection());

        shaderList[0]->SetDirectionalLight(&sun);
        shaderList[0]->SetPointLights(pointLights, pointLightCount);
        shaderList[0]->SetSpotLights(spotLights, spotLightCount);

        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
        glUniform3f(uniformEyePosition, camera->getCameraPosition().x, camera->getCameraPosition().y, camera->getCameraPosition().z);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(5.0f, 1.0f, 5.0f));
        
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        
        marbleTexture.UseTexture();
        shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[0]->RenderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-5.0f, 1.0f, -5.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        corkTexture.UseTexture();
        dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[1]->RenderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -0.1f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        marbleTexture.UseTexture();
        shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[2]->RenderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.0f, -0.1f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        knight.RenderModel();

        glUseProgram(0);

        /* Swap front and back buffers */
        mainWindow->swapBuffers();
    }

    shaderList[0]->ClearShader();
    return 0;
}