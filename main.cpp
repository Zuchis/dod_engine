#include "engine.h"

void engine_init(void) {
    //SHADERS Init

    //GLEW Init
    glewExperimental = GL_TRUE;
    GLenum result = glewInit();

    if (result != GLEW_OK) 
    {
        std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
        exit(EXIT_FAILURE);
    }

    GLenum err_code = glGetError();
}

int main()
{
    unsigned int width = 1024;
    unsigned int height = 768;

    math::Quaternion rotationQuaternion(1.0f,0.0f,0.0f,0.0f);

    sf::RenderWindow window(sf::VideoMode(width, height), "Sick Engine M8");

    engine_init();

    ShaderProgram program = ShaderProgram();
    program.compileShaderFromFile("./data/shaders/vertexShaderSceneManagement.glsl", ShaderType::VERTEX);
    program.compileShaderFromFile("./data/shaders/fragmentShaderSceneManagement.glsl", ShaderType::FRAGMENT);
    program.bindAttribLocation(engine::VCOORDS, "in_Position");
    program.bindAttribLocation(engine::TCOORDS, "in_TexCoords");
    program.bindAttribLocation(engine::NCOORDS, "in_Normal");
    program.addUniformBlock("Camera", engine::UBO_BP);
    program.link();

    ArcballCamera camera = ArcballCamera(engine::UBO_BP);
    camera.setProjectionMatrix(math::Perspective(30.0f, width / height, 0.1f, 100.0f));

    //TODO move this one to the main loop later
    Matrix4 translation = math::translate(Vector3(0.0f,0.0f,engine::cameraDistance * -1));
    Matrix4 rotation    = rotationQuaternion.toMatrix();
    camera.setViewMatrix(translation * rotation);

    Matrix4 modelMatrix = math::translate(Vector3(0.0f,0.0f,0.0f)) *
                            math::Quaternion(0.0f,Vector3(-1.0f,0.0f,0.0f)).toMatrix() *
                            math::scale(Vector3(4.0f,3.0f,0.4f));
                            
    //Matrix4 modelMatrix = math::Create4DIdentity();
    
    

    std::string tamSquare("objects/tamSquare.obj");
    engine::Mesh cube(tamSquare);
    cube.create();

    while (window.isOpen())
    {
        // Treat the events 

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::Resized) {
                glViewport(0,0, event.size.width, event.size.height);
            }
        }

        // Drawing goes here
        

        program.use();
        Matrix4 translation = math::translate(Vector3(0.0f,0.0f,engine::cameraDistance * -1));
        Matrix4 rotation    = rotationQuaternion.toMatrix();
        camera.setViewMatrix(translation * rotation);
        program.setUniform("Matrix", modelMatrix.getData());

        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cube.draw();

        window.display();
    }

    return 0;
}
