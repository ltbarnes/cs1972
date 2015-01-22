#include "graphics.h"
#include <QFile>
#include <QTextStream>

Graphics::Graphics()
{
}

Graphics::~Graphics()
{
}

void Graphics::init()
{
    m_default = Graphics::loadShaders(
                ":/shaders/default.vert",
                ":/shaders/default.frag");


    m_uniformLocs["projection"] = glGetUniformLocation(m_default, "projection");
    m_uniformLocs["view"] = glGetUniformLocation(m_default, "view");
    m_uniformLocs["model"] = glGetUniformLocation(m_default, "model");

    m_uniformLocs["tex"] = glGetUniformLocation(m_default, "tex");
    m_uniformLocs["useTexture"] = glGetUniformLocation(m_default, "useTexture");
}

void Graphics::setUniforms(Camera *camera)
{
//    assert(camera);

    glUseProgram(m_default);

//    // Set scene uniforms.
//    glUniformMatrix4fv(m_uniformLocs["p"], 1, GL_FALSE,
//            glm::value_ptr(camera->getProjectionMatrix()));
//    glUniformMatrix4fv(m_uniformLocs["v"], 1, GL_FALSE,
//            glm::value_ptr(camera->getViewMatrix));
}


GLuint Graphics::loadShaders(const char *vertex_file_path, const char *fragment_file_path){

    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    QString vertFilePath = QString(vertex_file_path);
    QFile vertFile(vertFilePath);
    if (vertFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream vertStream(&vertFile);
        VertexShaderCode = vertStream.readAll().toStdString();
    }


    // Read fragment shader code from file
    std::string FragmentShaderCode;
    QString fragFilePath = QString(fragment_file_path);
    QFile fragFile(fragFilePath);
    if (fragFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream fragStream(&fragFile);
        FragmentShaderCode = fragStream.readAll().toStdString();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> VertexShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    if (!Result)
        fprintf(stderr, "Error compiling shader: %s\n%s\n",
                vertex_file_path, &VertexShaderErrorMessage[0]);

    // Compile Fragment Shader
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    if (!Result)
        fprintf(stderr, "Error compiling shader: %s\n%s\n",
                fragment_file_path, &FragmentShaderErrorMessage[0]);

    // Link the program
    GLuint programId = glCreateProgram();
    glAttachShader(programId, VertexShaderID);
    glAttachShader(programId, FragmentShaderID);
    glLinkProgram(programId);

    // Check the program
    glGetProgramiv(programId, GL_LINK_STATUS, &Result);
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ProgramErrorMessage( std::max(InfoLogLength, int(1)) );
    glGetProgramInfoLog(programId, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    if (!Result)
        fprintf(stderr, "Error linking shader: %s\n", &ProgramErrorMessage[0]);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return programId;
}



