#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>

using namespace std;

GLuint program;
GLint attribute_coord2d;

int init(void);
void free_resources(void);
void display(void);

int main(int argc, char **argv){

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Triangulos");

    if(glewInit()){
        cout << "Error glewInit()" << endl;
        return -1;
    }
    if(init()){
        glutDisplayFunc(display);
        glutMainLoop();
    }
    free_resources();
    return EXIT_SUCCESS;
}


int init(){
    GLint compile_ok = GL_FALSE, link_ok = GL_FALSE;

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    const char *vs_source = 
    #ifdef GL_ES_VERSION_4_0
        "#version 400\n"  // OpenGL ES 2.0
    #else
        "#version 120\n"  // OpenGL 2.1
    #endif
    "attribute vec2 coord2d;                  "
    "void main(void) {                        "
    "  gl_Position = vec4(coord2d, 1.0, 1.0); "
    "}";

    glShaderSource(vs, 1, &vs_source, NULL);
    glCompileShader(vs);
    glGetShaderiv(vs,GL_COMPILE_STATUS, &compile_ok);
    if(compile_ok == 0){
        cout << "Error vertex shader" << endl;
        return 0;
    }

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fs_source = 
    "#version 120           \n"
    "void main(void) {        "
    "  gl_FragColor[0] = 0.0; "
    "  gl_FragColor[1] = 0.0; "
    "  gl_FragColor[2] = 1.0; "
    "}";
    glShaderSource(fs, 1, &fs_source, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &compile_ok);
    if(compile_ok == 0){
        cout << "Error Fragmente Shader" << endl;
        return 0;
    }

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
    if(link_ok == 0){
        cout << "Error link program" << endl;
        return 0;
    }

    const char *atribute_name = "coord2d";
    attribute_coord2d = glGetAttribLocation(program, atribute_name);
    if(attribute_coord2d == -1){
        cout << "Could not Bind atribute " << atribute_name << endl;
        return 0;
    }

    return GL_TRUE;
}

void free_resources(){
    glDeleteProgram(program);
}

void display(){
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glEnableVertexAttribArray(attribute_coord2d);
    
    GLfloat triangle_vertices[][2] = {
        {0.0,  0.8},
        {-0.8, -0.8},
        {0.8, -0.8}
    };

    glVertexAttribPointer(
        attribute_coord2d,
        2,
        GL_FLOAT,
        GL_FALSE,
        0,
        triangle_vertices
    );

    /* Puxando cada elemento do nosso buffer_vertices para o vertex shader */
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(attribute_coord2d);

    /* Mostrando os resultados. */
    glutSwapBuffers();
    //glFlush();
}