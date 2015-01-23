#include "graphics.h"
#include <QFile>
#include <QTextStream>
#include <QImage>
#include <QDir>
#include <QGLWidget>

#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
using namespace std;

Graphics::Graphics()
{
    m_quad = new Shape();
    m_cube = new Cube();
}

Graphics::~Graphics()
{
    delete m_quad;
    delete m_cube;
}

void Graphics::init()
{
    m_defaultShader = Graphics::loadShaders(
                ":/shaders/default.vert",
                ":/shaders/default.frag");

    m_uniformLocs["projection"] = glGetUniformLocation(m_defaultShader, "projection");
    m_uniformLocs["view"] = glGetUniformLocation(m_defaultShader, "view");
    m_uniformLocs["model"] = glGetUniformLocation(m_defaultShader, "model");

    m_uniformLocs["color"] = glGetUniformLocation(m_defaultShader, "color");
    m_uniformLocs["tex"] = glGetUniformLocation(m_defaultShader, "tex");
    m_uniformLocs["useTexture"] = glGetUniformLocation(m_defaultShader, "useTexture");

    m_uniformLocs["repeatU"] = glGetUniformLocation(m_defaultShader, "repeatU");
    m_uniformLocs["repeatV"] = glGetUniformLocation(m_defaultShader, "repeatV");

    m_quad->init(m_defaultShader);
    m_cube->init(m_defaultShader);
    loadTexture(":/images/grass.png", "grass");
}

void Graphics::setUniforms(Camera *camera)
{
    assert(camera);

    glUseProgram(m_defaultShader);

    // Set scene uniforms.
    glUniformMatrix4fv(m_uniformLocs["projection"], 1, GL_FALSE,
            glm::value_ptr(camera->getProjectionMatrix()));
    glUniformMatrix4fv(m_uniformLocs["view"], 1, GL_FALSE,
            glm::value_ptr(camera->getViewMatrix()));
}


void Graphics::setColor(float r, float g, float b, float a)
{
    glUniform4f(m_uniformLocs["color"], r, g, b, a);
}


void Graphics::setTexture(const QString &key, float repeatU = 1.f, float repeatV = 1.f)
{
    GLint tex = m_textures.value(key);

    if (tex)
    {
        glUniform1i(m_uniformLocs["useTexture"], 1);
        glUniform1i(m_uniformLocs["tex"], 1);
        glUniform1f(m_uniformLocs["repeatU"], repeatU);
        glUniform1f(m_uniformLocs["repeatV"], repeatV);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_textures.value(key));
        glActiveTexture(GL_TEXTURE0);
    }
    else
    {
        glUniform1i(m_uniformLocs["useTexture"], 0);
    }
}


void Graphics::drawQuad(glm::mat4 trans)
{
    m_quad->transformAndRender(m_defaultShader, trans);
}


void Graphics::drawCube(glm::mat4 trans)
{
    m_cube->transformAndRender(m_defaultShader, trans);
}


void Graphics::loadTexturesFromDirectory(const QString &dirname)
{
    QDir dir(dirname);
}


void Graphics::loadTexture(const QString &filename, const QString &key)
{
    // make sure file exists
    QFile file(filename);
    if (!file.exists() || m_textures.contains(key))
    {
        cout << "no tex" << endl;
        return;
    }

    // load file into memory
    QImage image;
    image.load(file.fileName());
    image = image.mirrored(false, true);
    QImage texture = QGLWidget::convertToGLFormat(image);

    // generate texture ID
    GLuint id = 0;
    glGenTextures(1, &id);

    // make the texture
    glBindTexture(GL_TEXTURE_2D, id);

    // copy image data into texture
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, texture.width(), texture.height(), GL_RGBA, GL_UNSIGNED_BYTE, texture.bits());

    // filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // coordinate wrapping options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    m_textures.insert(key, id);
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



