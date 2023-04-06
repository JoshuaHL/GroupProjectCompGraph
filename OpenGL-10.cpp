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


// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "shader.h"
#include "camera.h"
#include "model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Active window
GLFWwindow* window;

// Window Properties
GLuint sWidth = 800, sHeight = 800;



// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 200.0f));


GLfloat angle = 0.0f;
GLfloat angleInc = 0.001f;


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
    
    //Set the window's background colour (to GREY)
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    

    // Setup OpenGL options
    glEnable(GL_DEPTH_TEST);

}




// The MAIN function, from here we start our application and run our Game loop
int main()
{
     init_Resources();
    
     //-------- Earth increments ----------------------
    GLfloat earthX = 80.0;
    GLfloat earthY = 80.0;
    GLfloat earthXinc = 0.5;
    GLfloat earthYinc = 1;
    //------------------------------------------------
    
    //-------- Mars increments -----------------------
    GLfloat marsX = 40.0;
    GLfloat marsY = 40.0;
    GLfloat marsXinc = -0.1;
    GLfloat marsYinc = -0.1;
    //------------------------------------------------

    //-------- Venus increments ----------------------
    GLfloat venusX = 30.0;
    GLfloat venusY = 30.0;
    GLfloat venusXinc = -0.5;
    GLfloat venusYinc = -0.5;
    //------------------------------------------------


    GLdouble xdif, ydif, temp;

    
    // ==============================================
    // ====== Set up the stuff for our sphere =======
    // ==============================================
    
    // 1. Setup and compile our shaders (new approach)
    Shader earthShader("earthVertex.glsl", "earthFragment.glsl");
    Shader marsShader("marsVertex.glsl"  , "marsFragment.glsl");
    Shader venusShader("venusVertex.glsl", "venusFragment.glsl");
 
    
    // 2. Load the earth. mars and venus objects
    Model earth((GLchar*)"earth.obj");
    Model mars((GLchar*)"mars.obj");
    Model venus((GLchar*)"venus.obj");

    
 
    // 3. Set the projection matrix for the camera
    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)sWidth/(GLfloat)sHeight,
                                            1.0f, 10000.0f);
    
    
    
    // 4. Create the projection matrices by Activating the shader objects for each planet
    earthShader.Use();
    glUniformMatrix4fv(glGetUniformLocation(earthShader.Program, "projection"),
                       	1, GL_FALSE, glm::value_ptr(projection));
    
    marsShader.Use();
    glUniformMatrix4fv(glGetUniformLocation(marsShader.Program, "projection"),
                       1, GL_FALSE, glm::value_ptr(projection));

    venusShader.Use();
    glUniformMatrix4fv(glGetUniformLocation(venusShader.Program, "projection"),
        1, GL_FALSE, glm::value_ptr(projection));

    
    
    // ==================================================================
    // ====== Set up the changes we want while the window is open =======
    // ==================================================================

     while(!glfwWindowShouldClose(window))
    {
        
        // Clear buffers
        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        // Add transformation matrices ... by repeatedly modifying the model matrix
        
        // 1. The View matrix for each planet first...
        earthShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(earthShader.Program, "view"), 1,
                           GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
        
        marsShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(marsShader.Program, "view"), 1,
                           GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

        venusShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(venusShader.Program, "view"), 1,
            GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

        
        
        // 2. Create the model matrix for each planet
        earthShader.Use();
        glm::mat4 earthModel = glm::mat4(1);
        
        marsShader.Use();
        glm::mat4 marsModel = glm::mat4(1);

       venusShader.Use();
        glm::mat4 venusModel = glm::mat4(1);




        
        // Test for edge detection
    
        // ...Test for Earth touching the sides...
        if (earthX > 82 || earthX < -82)
           earthXinc *= -1;

        if (earthY > 82 || earthY < -82)
            earthYinc *= -1;

    
        // ...Test for Mars touching the sides...
        if (marsX > 82 || marsX < -82)
            marsXinc *= -1;
    
        if (marsY > 82 || marsY < -82)
            marsYinc *= -1;


        // ...Test for Venus touching the sides...
        if (venusX > 82 || venusX < -82)
            venusXinc *= -1;

        if (venusY > 82 || venusY < -82)
            venusYinc *= -1;



        
        // ...Test for earth touching Mars... using Pythagorus theorem
        
        xdif = earthX - marsX;
        ydif = earthY - marsY;
        if ( sqrt((xdif*xdif) + (ydif*ydif)) < 52)    //(earthRadius + marsRadius))
            {
                // Once collided, repel and swap speed of translation.
                temp = marsXinc;
                marsXinc = earthXinc;
                earthXinc = temp;
            
                temp = marsYinc;
                marsYinc = earthYinc;
                earthYinc = temp;
            }

        
        // ...Test for earth touching venus... Pythagorus theorem again...

        xdif = earthX - venusX;
        ydif = earthY - venusY;
        if (sqrt((xdif * xdif) + (ydif * ydif)) < 52)    //(earthRadius + venusRadius))
            {
                // Once collided, repel and swap speed of translation.
                temp = venusXinc;
                venusXinc = earthXinc;
                earthXinc = temp;

                temp = venusYinc;
                venusYinc = earthYinc;
                earthYinc = temp;
            }


        // ...Test for Mars touching venus... Pythagorus theorem yet again...

        xdif = marsX - venusX;
        ydif = marsY - venusY;
        if (sqrt((xdif * xdif) + (ydif * ydif)) < 52) //(marsRadius + venusRadius))
        {
            // Once collided, repel and swap speed of translation.
            temp = venusXinc;
            venusXinc = marsXinc;
            marsXinc = temp;

            temp = venusYinc;
            venusYinc = marsYinc;
            marsYinc = temp;
        }





        //Increment the planets' locations
        
        earthX += earthXinc;
        earthY += earthYinc;

        marsX += marsXinc;
        marsY += marsYinc;

        venusX += venusXinc;
        venusY += venusYinc;

        
        
        
        
        
        // 3. Apply the translation matrix to the planets' model matrix

        earthModel = glm::translate(earthModel, glm::vec3(earthX, earthY, 0.0f));
        marsModel = glm::translate(marsModel  , glm::vec3(marsX, marsY, 0.0f));
        venusModel = glm::translate(venusModel, glm::vec3(venusX, venusY, 0.0f));



        // 4. Apply the scaling matrix to the planets' model matrix
        earthModel = glm::scale(earthModel, glm::vec3(10.0f, 10.0f, 10.0f));
        marsModel = glm::scale(marsModel, glm::vec3(10.0f, 10.0f, 10.0f));
        venusModel = glm::scale(venusModel, glm::vec3(10.0f, 10.0f, 10.0f));




        // 5. Apply the rotation matrix to the planets' model matrix
        earthModel = glm::rotate(earthModel, -45.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        marsModel  = glm::rotate(marsModel, -45.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        venusModel = glm::rotate(venusModel, -45.0f, glm::vec3(1.0f, 0.0f, 0.0f));





        // Make yje planets rotate around the y-axis
        angle += angleInc;
        earthModel = glm::rotate(earthModel, angle, glm::vec3(0.0f, 0.0f, 1.0f));
        marsModel  = glm::rotate(marsModel,  angle, glm::vec3(0.0f, 0.0f, 1.0f));
        venusModel = glm::rotate(venusModel, angle, glm::vec3(0.0f, 0.0f, 1.0f));

        if (angle >= 360) angle = angleInc;
    



        // ...and 6. The final Model-View-Projection matrix for the object
        glUniformMatrix4fv(glGetUniformLocation(earthShader.Program, "model"), 1,
                           GL_FALSE, glm::value_ptr(earthModel));
        
        // Display the Earth
        earth.Draw(earthShader);

        
        
        glUniformMatrix4fv(glGetUniformLocation(marsShader.Program, "model"), 1,
                           GL_FALSE, glm::value_ptr(marsModel));
        
        // Display the Mars
        mars.Draw(marsShader);



        glUniformMatrix4fv(glGetUniformLocation(venusShader.Program, "model"), 1,
                           GL_FALSE, glm::value_ptr(venusModel));

        // Display the Venus
        venus.Draw(venusShader);


        
        // Swap the frame buffers
        glfwSwapBuffers(window);
        
        glfwPollEvents();
        
     

    }
    
    glfwTerminate();
    return 0;
}



