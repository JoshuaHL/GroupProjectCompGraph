//
//  OpenGL-10.cpp
//  OpenGL-10
//
//  Created by Dr. John Charlery on 03/20/2023.
//  Copyright (c) 2023 University of the West Indies. All rights reserved.
//
// ========================================================================
//  PROGRAM:
//          Three Animated, Textured Spheres WITH edge detection
//
//=========================================================================

/*=========================================================================
Install the following in Package Manager :
Install-Package glew_dynamic
Install-Package glfw
Install-Package GLMathematics
Install-Package freeimage -version 3.16.0
Install-Package nupengl.core
Install-Package Soil
Install-Package Assimp -version 3.0.0
============================================================================*/

//  Shifting the focus here a bit:
//      1.  Using a modified class-based shaders system (shader.h)
//      2.  Using a model.h header to import objects from programs such
//          as Blender
//      3.  Using a mesh.h file to handle the object(s)
//      4.  Using a camera.h file to handle the camers's movements (pitch,
//          yaw, speed, zoom, etc)
//      4.  Using ASSIMP library to handle the textures to objects
//      5.  Using libSOIL library to handle the textures of diffrent
//          formats (PNG, BMP, etc)
// ========================================================================
#define _USE_MATH_DEFINES
//final commit
// GL includes
#include "shader.h"
#include "camera.h"
#include "model.h"
#include <math.h>
// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Active window
GLFWwindow* window;

// Window Properties
GLuint sWidth = 1280, sHeight = 720;



// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 200.0f));


const float BALL_RADIUS = 6.7;
const float FRICTION = 0.01;
const float RESTITUTION = 0.0;
const float MASS = 0.0;
float ballPositions[16][2];
float ballVelocities[16][2] = { {0} };
float angle[16] = { 0 };

//================================Jonathan Drakes======================================

//===================== Protoype function for call back  ==============================
void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int modes);
void mouse_callback(GLFWwindow* window, int button, int  action, int mode);
void windowSize_callback(GLFWwindow* window, int width, int height);
//=====================================================================================

void init_Resources()
{
    // Initialize the resources - set window, etc.
    if (!glfwInit())
    {
        cout << "\nFailed to Initialize GLFW...";
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    // Create the display window
    window = glfwCreateWindow(sWidth, sHeight, "COMP3420 - Colliding Spheres with Edge-Detection", 0, 0); // Windowed


    //If window fails creation, then shut down the whole thing
    if (!window)
    {
        cout << "\nFailed to open display window...";
        glfwTerminate();
        exit(EXIT_FAILURE);
    }


    //If window succeeded then make it active on the display device
    glfwMakeContextCurrent(window);


    // ******  ABSOLUTELY required otherwise glGenVertexArrays will FAIL! *************
    glewExperimental = GL_TRUE;


    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        cout << "\nFailed to initialize GLEW...";
        exit(EXIT_FAILURE);
    }


    //================== Jonathan Drakes ==================
    //-----------------------------------------------------
    // Registering the call-back function foe the keyboard
    //-----------------------------------------------------
    glfwSetKeyCallback(window, keyboardCallback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetWindowSizeCallback(window, windowSize_callback);


    // Setup OpenGL options
    glEnable(GL_DEPTH_TEST);

}


// ================== Jonathan Drakes ===============================
// ==================================================================
// ============ Call back function for the keyboard =================
// ==================================================================

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int modes)
{
    //If ESC us pressed, close the window
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
        return;
    }
}

// ============ Call back function for window resize  ===============
// ==================================================================

void windowSize_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

bool dragging = false;
float cueBallPosX, cueBallPosY;
float mouseX, mouseY;
float ballxdif, ballydif, totaldiff;

void mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        mouseX = xpos;
        mouseY = ypos;
        ballxdif = (ballPositions[15][0] + 198) - (396 * (mouseX / 1280));
        ballydif = (ballPositions[15][1] + 105) - (210 * ((720 - mouseY) / 720));
        totaldiff = abs(ballxdif) + abs(ballydif);
        ballVelocities[15][0] = -(ballxdif / totaldiff * 2);
        ballVelocities[15][1] = -(ballydif / totaldiff * 2);
        //     cueBallPosX = getCueBallX(); // Function to get the current cue ball position
        //     cueBallPosY = getCueBallY();
        // }
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        dragging = false;
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (dragging) {
        float deltaX = xpos - mouseX;
        float deltaY = ypos - mouseY;
        //setCueBallPos(cueBallPosX + deltaX, cueBallPosY + deltaY); // Function to set the new cue ball position
    }
}

// The MAIN function, from here we start our application and run our Game loop
int main()
{

    init_Resources();

    //-------- PoolBall initial values -------------------

    ballPositions[0][0] = 132.0;
    ballPositions[0][1] = 14.0;
    ballPositions[1][0] = 93.0;
    ballPositions[1][1] = -7.0;
    ballPositions[2][0] = 132.0;
    ballPositions[2][1] = -14.0;
    ballPositions[3][0] = 106.0;
    ballPositions[3][1] = 14.0;
    ballPositions[4][0] = 132.0;
    ballPositions[4][1] = 28.0;
    ballPositions[5][0] = 119.0;
    ballPositions[5][1] = 7.0;
    ballPositions[6][0] = 119.0;
    ballPositions[6][1] = -21.0;
    ballPositions[7][0] = 106.0;
    ballPositions[7][1] = 0.0;
    ballPositions[8][0] = 80.0;
    ballPositions[8][1] = 0.0;
    ballPositions[9][0] = 106.0;
    ballPositions[9][1] = -14.0;
    ballPositions[10][0] = 119.0;
    ballPositions[10][1] = 21.0;
    ballPositions[11][0] = 119.0;
    ballPositions[11][1] = -7.0;
    ballPositions[12][0] = 132.0;
    ballPositions[12][1] = -28.0;
    ballPositions[13][0] = 132.0;
    ballPositions[13][1] = 0.0;
    ballPositions[14][0] = 93.0;
    ballPositions[14][1] = 7.0;
    ballPositions[15][0] = 10.0;
    ballPositions[15][1] = 10.0;



    ballVelocities[15][0] = 0.0;


    //------------------------------------------------


    GLdouble xdif, ydif, temp;


    // ==============================================
// ====== Set up the stuff for our sphere =======
// ==============================================

// 1. Setup and compile the shader
    Shader poolBallShader("poolBallVertex.glsl", "poolBallFragment.glsl");


    // 2. Load the earth. mars and venus objects
    Model Ball1((GLchar*)"1Ball.obj");
    Model Ball2((GLchar*)"2Ball.obj");
    Model Ball3((GLchar*)"3Ball.obj");
    Model Ball4((GLchar*)"4Ball.obj");
    Model Ball5((GLchar*)"5Ball.obj");
    Model Ball6((GLchar*)"6Ball.obj");
    Model Ball7((GLchar*)"7Ball.obj");
    Model Ball8((GLchar*)"8Ball.obj");
    Model Ball9((GLchar*)"9Ball.obj");
    Model Ball10((GLchar*)"10Ball.obj");
    Model Ball11((GLchar*)"11Ball.obj");
    Model Ball12((GLchar*)"12Ball.obj");
    Model Ball13((GLchar*)"13Ball.obj");
    Model Ball14((GLchar*)"14Ball.obj");
    Model Ball15((GLchar*)"15Ball.obj");
    Model CueBall((GLchar*)"CueBall.obj");




    // 3. Set the projection matrix for the camera
    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)sWidth / (GLfloat)sHeight,
        1.0f, 10000.0f);



    // 4. Create the projection matrices by Activating the shader objects

    poolBallShader.Use();
    glUniformMatrix4fv(glGetUniformLocation(poolBallShader.Program, "projection"),
        1, GL_FALSE, glm::value_ptr(projection));




    /*--////////////////Section below - Done by Zachary Farrell/////////////--*/
    /////////////////////////////////////////////////////////////////////////////


    Model poolStick((GLchar*)"10522_Pool_Cue_v1_L3.obj");


    /*--////////////////Section above - Done by Zachary Farrell/////////////--*/
    /////////////////////////////////////////////////////////////////////////////




     // ==================================================================
     // ====== Set up the changes we want while the window is open =======
     // ==================================================================

    while (!glfwWindowShouldClose(window))
    {

        // Clear buffers
        glClearColor(0.0f, 0.345f, 0.141f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Add transformation matrices ... by repeatedly modifying the model matrix

        // 1. The View matrix for each planet first...

        poolBallShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(poolBallShader.Program, "view"), 1,
            GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));


        // 2. Create the model matrix for each pool ball

        poolBallShader.Use();
        glm::mat4 Ball1Model = glm::mat4(1);
        glm::mat4 Ball2Model = glm::mat4(1);
        glm::mat4 Ball3Model = glm::mat4(1);
        glm::mat4 Ball4Model = glm::mat4(1);
        glm::mat4 Ball5Model = glm::mat4(1);
        glm::mat4 Ball6Model = glm::mat4(1);
        glm::mat4 Ball7Model = glm::mat4(1);
        glm::mat4 Ball8Model = glm::mat4(1);
        glm::mat4 Ball9Model = glm::mat4(1);
        glm::mat4 Ball10Model = glm::mat4(1);
        glm::mat4 Ball11Model = glm::mat4(1);
        glm::mat4 Ball12Model = glm::mat4(1);
        glm::mat4 Ball13Model = glm::mat4(1);
        glm::mat4 Ball14Model = glm::mat4(1);
        glm::mat4 Ball15Model = glm::mat4(1);
        glm::mat4 CueBallModel = glm::mat4(1);


        for (int i = 0; i < 16; i++) {
            if (ballPositions[i][0] > 190) {
                ballVelocities[i][0] *= -1;
                ballPositions[i][0] = 190;
            }

            if (ballPositions[i][0] < -190) {
                ballVelocities[i][0] *= -1;
                ballPositions[i][0] = -190;
            }

            if (ballPositions[i][1] > 105) {
                ballVelocities[i][1] *= -1;
                ballPositions[i][1] = 105;
            }

            if (ballPositions[i][1] < -105) {
                ballVelocities[i][1] *= -1;
                ballPositions[i][1] = -105;
            }
        }

        for (int i = 0; i < 16; i++) {
            for (int j = i + 1; j < 16; j++) {
                float dx = ballPositions[j][0] - ballPositions[i][0];
                float dy = ballPositions[j][1] - ballPositions[i][1];
                //Distance between the center of each ball
                float distance = sqrt(dx * dx + dy * dy);
                if (distance < 2 * BALL_RADIUS) {
                    float v1[2];
                    float v2[2];
                    float u1[2];
                    float u2[2];
                    float nx = dx / distance;
                    float ny = dy / distance;
                    float overlap = (BALL_RADIUS * 2 - distance);


                    //Calculate the angle of rotation
                    float theta = -atan2(ballPositions[i][1] - ballPositions[j][1], ballPositions[i][0] - ballPositions[j][0]);
                    //rotate angle
                    u1[0] = ballVelocities[i][0] * cos(theta) - ballVelocities[i][1] * sin(theta);
                    u1[1] = ballVelocities[i][0] * sin(theta) + ballVelocities[i][1] * cos(theta);
                    u2[0] = ballVelocities[j][0] * cos(theta) - ballVelocities[j][1] * sin(theta);
                    u2[1] = ballVelocities[j][0] * sin(theta) + ballVelocities[j][1] * cos(theta);
                    v1[0] = u2[0] * cos(-theta) - u1[1] * sin(-theta);
                    v1[1] = u2[0] * sin(-theta) + u1[1] * cos(-theta);
                    v2[0] = u1[0] * cos(-theta) - u2[1] * sin(-theta);
                    v2[1] = u1[0] * sin(-theta) + u2[1] * cos(-theta);

                    float overlapDirectionX = dx / distance;
                    float overlapDirectionY = dy / distance;
                    float totalOverlap = overlapDirectionX + overlapDirectionY;

                    ballVelocities[i][0] = ballVelocities[i][0] * 0.2 + v1[0];
                    ballVelocities[i][1] = ballVelocities[i][1] * 0.2 + v1[1];
                    ballVelocities[j][0] = ballVelocities[j][0] * 0.2 + v2[0];
                    ballVelocities[j][1] = ballVelocities[j][1] * 0.2 + v2[1];



                    // Move the balls away from each other along the direction of overlap

                    ballPositions[i][0] -= overlapDirectionX * overlap / 2.0f;
                    ballPositions[i][1] -= overlapDirectionY * overlap / 2.0f;
                    ballPositions[j][0] += overlapDirectionX * overlap / 2.0f;
                    ballPositions[j][1] += overlapDirectionY * overlap / 2.0f;

                }
            }
        }

        //Opdate all ball positions based on their velocities with a slight friction
        for (int i = 0; i < 16; i++) {
            ballPositions[i][0] += ballVelocities[i][0];
            ballPositions[i][1] += ballVelocities[i][1];
            if (ballVelocities[i][0] > 0) {
                ballVelocities[i][0] *= 0.996;
            }
            if (ballVelocities[i][0] < 0) {
                ballVelocities[i][0] *= 0.996;
            }
            if (ballVelocities[i][1] > 0) {
                ballVelocities[i][1] *= 0.996;
            }
            if (ballVelocities[i][1] < 0) {
                ballVelocities[i][1] *= 0.996;
            }
        }





        // 3. Apply the translation matrix to the planets' model matrix
        Ball1Model = glm::translate(Ball1Model, glm::vec3(ballPositions[0][0], ballPositions[0][1], 0.0f));
        Ball2Model = glm::translate(Ball2Model, glm::vec3(ballPositions[1][0], ballPositions[1][1], 0.0f));
        Ball3Model = glm::translate(Ball3Model, glm::vec3(ballPositions[2][0], ballPositions[2][1], 0.0f));
        Ball4Model = glm::translate(Ball4Model, glm::vec3(ballPositions[3][0], ballPositions[3][1], 0.0f));
        Ball5Model = glm::translate(Ball5Model, glm::vec3(ballPositions[4][0], ballPositions[4][1], 0.0f));
        Ball6Model = glm::translate(Ball6Model, glm::vec3(ballPositions[5][0], ballPositions[5][1], 0.0f));
        Ball7Model = glm::translate(Ball7Model, glm::vec3(ballPositions[6][0], ballPositions[6][1], 0.0f));
        Ball8Model = glm::translate(Ball8Model, glm::vec3(ballPositions[7][0], ballPositions[7][1], 0.0f));
        Ball9Model = glm::translate(Ball9Model, glm::vec3(ballPositions[8][0], ballPositions[8][1], 0.0f));
        Ball10Model = glm::translate(Ball10Model, glm::vec3(ballPositions[9][0], ballPositions[9][1], 0.0f));
        Ball11Model = glm::translate(Ball11Model, glm::vec3(ballPositions[10][0], ballPositions[10][1], 0.0f));
        Ball12Model = glm::translate(Ball12Model, glm::vec3(ballPositions[11][0], ballPositions[11][1], 0.0f));
        Ball13Model = glm::translate(Ball13Model, glm::vec3(ballPositions[12][0], ballPositions[12][1], 0.0f));
        Ball14Model = glm::translate(Ball14Model, glm::vec3(ballPositions[13][0], ballPositions[13][1], 0.0f));
        Ball15Model = glm::translate(Ball15Model, glm::vec3(ballPositions[14][0], ballPositions[14][1], 0.0f));
        CueBallModel = glm::translate(CueBallModel, glm::vec3(ballPositions[15][0], ballPositions[15][1], 0.0f));



        // 4. Apply the scaling matrix to the planets' model matrix
        Ball1Model = glm::scale(Ball1Model, glm::vec3(6.0f, 6.0f, 6.0f));
        Ball2Model = glm::scale(Ball2Model, glm::vec3(6.0f, 6.0f, 6.0f));
        Ball3Model = glm::scale(Ball3Model, glm::vec3(6.0f, 6.0f, 6.0f));
        Ball4Model = glm::scale(Ball4Model, glm::vec3(6.0f, 6.0f, 6.0f));
        Ball5Model = glm::scale(Ball5Model, glm::vec3(6.0f, 6.0f, 6.0f));
        Ball6Model = glm::scale(Ball6Model, glm::vec3(6.0f, 6.0f, 6.0f));
        Ball7Model = glm::scale(Ball7Model, glm::vec3(6.0f, 6.0f, 6.0f));
        Ball8Model = glm::scale(Ball8Model, glm::vec3(6.0f, 6.0f, 6.0f));
        Ball9Model = glm::scale(Ball9Model, glm::vec3(6.0f, 6.0f, 6.0f));
        Ball10Model = glm::scale(Ball10Model, glm::vec3(6.0f, 6.0f, 6.0f));
        Ball11Model = glm::scale(Ball11Model, glm::vec3(6.0f, 6.0f, 6.0f));
        Ball12Model = glm::scale(Ball12Model, glm::vec3(6.0f, 6.0f, 6.0f));
        Ball13Model = glm::scale(Ball13Model, glm::vec3(6.0f, 6.0f, 6.0f));
        Ball14Model = glm::scale(Ball14Model, glm::vec3(6.0f, 6.0f, 6.0f));
        Ball15Model = glm::scale(Ball15Model, glm::vec3(6.0f, 6.0f, 6.0f));
        CueBallModel = glm::scale(CueBallModel, glm::vec3(6.0f, 6.0f, 6.0f));




        // 5. Apply the rotation matrix to the planets' model matrix
        Ball1Model = glm::rotate(Ball1Model, -45.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        Ball2Model = glm::rotate(Ball2Model, -45.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        Ball3Model = glm::rotate(Ball3Model, -45.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        Ball4Model = glm::rotate(Ball4Model, -45.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        Ball5Model = glm::rotate(Ball5Model, -45.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        Ball6Model = glm::rotate(Ball6Model, -45.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        Ball7Model = glm::rotate(Ball7Model, -45.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        Ball8Model = glm::rotate(Ball8Model, -45.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        Ball9Model = glm::rotate(Ball9Model, -45.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        Ball10Model = glm::rotate(Ball10Model, -45.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        Ball11Model = glm::rotate(Ball11Model, -45.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        Ball12Model = glm::rotate(Ball12Model, -45.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        Ball13Model = glm::rotate(Ball13Model, -45.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        Ball14Model = glm::rotate(Ball14Model, -45.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        Ball15Model = glm::rotate(Ball15Model, -45.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        CueBallModel = glm::rotate(CueBallModel, -45.0f, glm::vec3(1.0f, 0.0f, 0.0f));

        GLfloat BallAngles[16] = { -45.0f };

        glm::mat4* BallModels[16] = { &Ball1Model, &Ball2Model, &Ball3Model, &Ball4Model, &Ball5Model, &Ball6Model, &Ball7Model, &Ball8Model, &Ball9Model, &Ball10Model, &Ball11Model, &Ball12Model, &Ball13Model, &Ball14Model, &Ball15Model, &CueBallModel };


        //* JOSHUA LASHLEY *//
        // Make the pool balls rotate in the direction they are spinning
        /*for (int i = 0; i < 16; i++) {
            GLfloat totalVelocity = sqrt(ballVelocities[i][0] * ballVelocities[i][0] + ballVelocities[i][1] * ballVelocities[i][1]);
            GLfloat angle = totalVelocity / 12;
            glm::vec3 axis = glm::cross(glm::vec3(ballVelocities[i][0], 0.0f, ballVelocities[i][1]), glm::vec3(0.0f, 1.0f, 0.0f));
            *BallModels[i] = glm::rotate(*BallModels[i], angle, axis);
        }*/
        for (int i = 0; i < 16; i++) {
            GLfloat poolBallvelocity = sqrt(ballVelocities[i][0] * ballVelocities[i][0] + ballVelocities[i][1] * ballVelocities[i][1]);
            if (poolBallvelocity != 0) {
                angle[i] += poolBallvelocity / 12;
                //Calculate the 
                glm::vec3 axis = glm::cross(glm::vec3(ballVelocities[i][0], 0.0f, ballVelocities[i][1]), glm::vec3(0.0f, 1.0f, 0.0f));
                *BallModels[i] = glm::rotate(*BallModels[i], angle[i], axis);
            }
        }





        glUniformMatrix4fv(glGetUniformLocation(poolBallShader.Program, "model"), 1,
            GL_FALSE, glm::value_ptr(Ball1Model));

        Ball1.Draw(poolBallShader);

        glUniformMatrix4fv(glGetUniformLocation(poolBallShader.Program, "model"), 1,
            GL_FALSE, glm::value_ptr(Ball2Model));

        Ball2.Draw(poolBallShader);

        glUniformMatrix4fv(glGetUniformLocation(poolBallShader.Program, "model"), 1,
            GL_FALSE, glm::value_ptr(Ball3Model));

        Ball3.Draw(poolBallShader);

        glUniformMatrix4fv(glGetUniformLocation(poolBallShader.Program, "model"), 1,
            GL_FALSE, glm::value_ptr(Ball4Model));

        Ball4.Draw(poolBallShader);

        glUniformMatrix4fv(glGetUniformLocation(poolBallShader.Program, "model"), 1,
            GL_FALSE, glm::value_ptr(Ball5Model));

        Ball5.Draw(poolBallShader);

        glUniformMatrix4fv(glGetUniformLocation(poolBallShader.Program, "model"), 1,
            GL_FALSE, glm::value_ptr(Ball6Model));

        Ball6.Draw(poolBallShader);

        glUniformMatrix4fv(glGetUniformLocation(poolBallShader.Program, "model"), 1,
            GL_FALSE, glm::value_ptr(Ball7Model));

        Ball7.Draw(poolBallShader);

        glUniformMatrix4fv(glGetUniformLocation(poolBallShader.Program, "model"), 1,
            GL_FALSE, glm::value_ptr(Ball8Model));

        Ball8.Draw(poolBallShader);

        glUniformMatrix4fv(glGetUniformLocation(poolBallShader.Program, "model"), 1,
            GL_FALSE, glm::value_ptr(Ball9Model));

        Ball9.Draw(poolBallShader);

        glUniformMatrix4fv(glGetUniformLocation(poolBallShader.Program, "model"), 1,
            GL_FALSE, glm::value_ptr(Ball10Model));

        Ball10.Draw(poolBallShader);

        glUniformMatrix4fv(glGetUniformLocation(poolBallShader.Program, "model"), 1,
            GL_FALSE, glm::value_ptr(Ball11Model));

        Ball11.Draw(poolBallShader);

        glUniformMatrix4fv(glGetUniformLocation(poolBallShader.Program, "model"), 1,
            GL_FALSE, glm::value_ptr(Ball12Model));

        Ball12.Draw(poolBallShader);

        glUniformMatrix4fv(glGetUniformLocation(poolBallShader.Program, "model"), 1,
            GL_FALSE, glm::value_ptr(Ball13Model));

        Ball13.Draw(poolBallShader);

        glUniformMatrix4fv(glGetUniformLocation(poolBallShader.Program, "model"), 1,
            GL_FALSE, glm::value_ptr(Ball14Model));

        Ball14.Draw(poolBallShader);

        glUniformMatrix4fv(glGetUniformLocation(poolBallShader.Program, "model"), 1,
            GL_FALSE, glm::value_ptr(Ball15Model));

        Ball15.Draw(poolBallShader);

        glUniformMatrix4fv(glGetUniformLocation(poolBallShader.Program, "model"), 1,
            GL_FALSE, glm::value_ptr(CueBallModel));

        CueBall.Draw(poolBallShader);



        /*--/////////////////Section below - Done by Zachary Farrell///////////--*/
        //////////////////////////////////////////////////////////////////////////





        // =======================================================================
        // Creating the model matrix 
        // =======================================================================
        glm::mat4 poolStickModel = glm::mat4(1);

        //Modify the model matrix with scaling, translation, rotation, etc
        poolStickModel = glm::scale(poolStickModel, glm::vec3(2.0f, 2.0f, 2.0f));
        poolStickModel = glm::translate(poolStickModel, glm::vec3(10.0f, -25.0f, 20.0f));
        poolStickModel = glm::rotate(poolStickModel, -45.0f, glm::vec3(1.0f, 0.0f, 0.0f));


        // =======================================================================
        // Passing the Model matrix, "poolStickModel", to the shader as "model"
        // =======================================================================
        glUniformMatrix4fv(glGetUniformLocation(poolBallShader.Program, "model"), 1,
            GL_FALSE, glm::value_ptr(poolStickModel));

        // =======================================================================
        // Drawing the Pool Stick object.
        // =======================================================================
        poolStick.Draw(poolBallShader);


        /*--////////////////Section above - Done by Zachary Farrell////////////--*/
        //////////////////////////////////////////////////////////////////////////





        // Swap the frame buffers
        glfwSwapBuffers(window);

        glfwPollEvents();



    }

    glfwTerminate();
    return 0;
}