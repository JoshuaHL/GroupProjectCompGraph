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

// GL includes
#include "shader.h"
#include "camera.h"
#include "model.h"

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


GLfloat poolBallAngle = 0.0f;
GLfloat poolBall2Angle = 0.0f;

//=============Jonathan Drakes========================
//===================== Protoype function for call back  ==============================
void keyboardCallback(GLFWwindow* window int key, int scancode, int action, int modes);
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
    

    // Setup OpenGL options
    glEnable(GL_DEPTH_TEST);

}




// The MAIN function, from here we start our application and run our Game loop
int main()
{
     init_Resources();

    //-------- PoolBall increments -------------------
    GLfloat poolBallX = 50.0;
    GLfloat poolBallY = 10.0;
    GLfloat poolBallXinc = -0.2;
    GLfloat poolBallYinc = 0.8;
    //------------------------------------------------

       //-------- PoolBall2 increments -------------------
    GLfloat poolBall2X = 10.0;
    GLfloat poolBall2Y = 50.0;
    GLfloat poolBall2Xinc = 0.4;
    GLfloat poolBall2Yinc = -0.7;
    //------------------------------------------------

    GLdouble xdif, ydif, temp;

    
    // ==============================================
    // ====== Set up the stuff for our sphere =======
    // ==============================================
    
    // 1. Setup and compile our shaders (new approach)
    Shader poolBallShader("poolBallVertex.glsl", "poolBallFragment.glsl");
 
    
    // 2. Load the earth. mars and venus objects
    Model poolBall((GLchar*)"10Ball.obj");
    Model poolBall2((GLchar*)"10Ball.obj");
    

    
 
    // 3. Set the projection matrix for the camera
    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)sWidth/(GLfloat)sHeight,
                                            1.0f, 10000.0f);
    
    
    
    // 4. Create the projection matrices by Activating the shader objects for each planet

    poolBallShader.Use();
    glUniformMatrix4fv(glGetUniformLocation(poolBallShader.Program, "projection"),
        1, GL_FALSE, glm::value_ptr(projection));
    
    
    // ================== Jonathan Drakes ===============================
    // ==================================================================
    // ============ Call back function for the keyboard =================
    // ==================================================================
    
    void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int modes)
    {
        //If ESC us pressed, close the window
        if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
            return;
        }
    }
    
    
    // ==================================================================
    // ====== Set up the changes we want while the window is open =======
    // ==================================================================

     while(!glfwWindowShouldClose(window))
    {
        
        // Clear buffers
        glClearColor(0.0f, 0.345f, 0.141f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        // Add transformation matrices ... by repeatedly modifying the model matrix
        
        // 1. The View matrix for each planet first...

        poolBallShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(poolBallShader.Program, "view"), 1,
            GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
        
        
        // 2. Create the model matrix for each planet

       poolBallShader.Use();
        glm::mat4 poolBallModel = glm::mat4(1);
        glm::mat4 poolBall2Model = glm::mat4(1);
       
        

        // ...Test for PoolBall touching the sides...
        if (poolBallX > 190 || poolBallX < -190)
            poolBallXinc *= -1;

        if (poolBallY > 105 || poolBallY < -105)
            poolBallYinc *= -1;

        // ...Test for PoolBall touching the sides...
        if (poolBall2X > 190 || poolBall2X < -190)
            poolBall2Xinc *= -1;

        if (poolBall2Y > 105 || poolBall2Y < -105)
            poolBall2Yinc *= -1;





        // ...Test for balls touching...
       
        xdif = poolBallX - poolBall2X;
        ydif = poolBallY - poolBall2Y;
        if (sqrt((xdif * xdif) + (ydif * ydif)) < 13) 
        {
            // Once collided, repel and swap speed of translation.
            temp = poolBallXinc;
            poolBallXinc = poolBall2Xinc;
            poolBall2Xinc = temp;

            temp = poolBallYinc;
            poolBallYinc = poolBall2Yinc;
            poolBall2Yinc = temp;
        }





        //Increment the planets' locationn

        poolBallX += poolBallXinc;
        poolBallY += poolBallYinc;

        poolBall2X += poolBall2Xinc;
        poolBall2Y += poolBall2Yinc;
        
        
        
        
        // 3. Apply the translation matrix to the planets' model matrix
        poolBallModel = glm::translate(poolBallModel, glm::vec3(poolBallX, poolBallY, 0.0f));
        poolBall2Model = glm::translate(poolBall2Model, glm::vec3(poolBall2X, poolBall2Y, 0.0f));



        // 4. Apply the scaling matrix to the planets' model matrix
        poolBallModel = glm::scale(poolBallModel, glm::vec3(6.0f, 6.0f, 6.0f));
        poolBall2Model = glm::scale(poolBall2Model, glm::vec3(6.0f, 6.0f, 6.0f));




        // 5. Apply the rotation matrix to the planets' model matrix
        poolBallModel = glm::rotate(poolBallModel, -45.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        poolBall2Model = glm::rotate(poolBall2Model, -45.0f, glm::vec3(1.0f, 0.0f, 0.0f));




        // Make yje planets rotate around the y-axis
       
        GLfloat poolBallvelocity = sqrt(poolBallXinc * poolBallXinc + poolBallYinc * poolBallYinc);
        poolBallAngle += poolBallvelocity/12;
        GLfloat poolBall2velocity = sqrt(poolBall2Xinc * poolBall2Xinc + poolBall2Yinc * poolBall2Yinc);
        poolBall2Angle += poolBall2velocity / 12;
        glm::vec3 poolBall1axis = glm::cross(glm::vec3(poolBallXinc, 0.0f, poolBallYinc), glm::vec3(0.0f, 1.0f, 0.0f));
        poolBallModel = glm::rotate(poolBallModel, poolBallAngle, poolBall1axis);
        glm::vec3 poolBall2axis = glm::cross(glm::vec3(poolBall2Xinc, 0.0f, poolBall2Yinc), glm::vec3(0.0f, 1.0f, 0.0f));
        poolBall2Model = glm::rotate(poolBall2Model, poolBall2Angle, poolBall2axis);


        glUniformMatrix4fv(glGetUniformLocation(poolBallShader.Program, "model"), 1,
            GL_FALSE, glm::value_ptr(poolBallModel));

        // Display the poolBall
        poolBall.Draw(poolBallShader);

        glUniformMatrix4fv(glGetUniformLocation(poolBallShader.Program, "model"), 1,
            GL_FALSE, glm::value_ptr(poolBall2Model));

        poolBall.Draw(poolBallShader);

        
        // Swap the frame buffers
        glfwSwapBuffers(window);
        
        glfwPollEvents();
        
     

    }
    
    glfwTerminate();
    return 0;
}



