

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
#include <ctime>

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
float DefaultPositions[16][2] = { {0} };
float ballVelocities[16][2] = { {0} };
float angle[16] = { 0 };
bool inHole[16] = { false };
bool inHole1 = false;
bool inHole2 = false;
bool inHole3 = false;
bool inHole4 = false;
bool inHole5 = false;
bool inHole6 = false;
bool inHole7 = false;
bool inHole8 = false;
bool inHole9 = false;
bool inHole10 = false;
bool inHole11 = false;
bool inHole12 = false;
bool inHole13 = false;
bool inHole14 = false;
bool inHole15 = false;
bool inHole16 = false;

GLfloat poolStickX = -20.0;
GLfloat poolStickY = 5.0;
GLfloat poolStickInc = 0.01;

GLfloat poolStickAngle = -45.0;
GLfloat poolStickAngleInc = 0.05;


//Lighting
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 199.0f);      // Light location
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);      // White light
glm::vec3 lightMode(4.0f);


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
    window = glfwCreateWindow(sWidth, sHeight, "Group Project Pool Game", 0, 0); // Windowed


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
    // Registering the call-back function for the keyboard
    //-----------------------------------------------------
    glfwSetKeyCallback(window, keyboardCallback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetWindowSizeCallback(window, windowSize_callback);

    //-----------------------------------------------------
    // Registering the call-back function for the mouse
    //-----------------------------------------------------
  

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

    //Reset game if enter is pressed
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    {
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 2; j++){
                inHole[i] = false;
                ballPositions[i][j] = DefaultPositions[i][j];
                ballVelocities[i][j] = 0;
            }
        }

    }

    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    {
            poolStickX += poolStickInc;
    }

    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    {
        poolStickX -= poolStickInc;
    }

    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    {
            poolStickY += poolStickInc;
    }

    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
        poolStickY -= poolStickInc;
    }

    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        poolStickAngle += poolStickAngleInc;
    }

    if (key == GLFW_KEY_L && action == GLFW_PRESS)
    {
        poolStickAngle -= poolStickAngleInc;
    }
}

// ============ Call back function for window resize  ===============
// ==================================================================

void windowSize_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

bool isHoldingKey = false;
bool dragging = false;
float cueBallPosX, cueBallPosY;
float mouseX, mouseY;
float ballxdif, ballydif, totaldiff;
float cueballSpeed = 0;
float begin_time;


void mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        begin_time = clock();
        isHoldingKey = true;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        isHoldingKey = false;
        cueballSpeed = (clock() - begin_time)/1000;
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        mouseX = xpos;
        mouseY = ypos;
        ballxdif = (ballPositions[15][0] + 198.5) - (397 * (mouseX / 1280));
        ballydif = (ballPositions[15][1] + 111.5) - (223 * ((720 - mouseY) / 720));
        totaldiff = abs(ballxdif) + abs(ballydif);
        ballVelocities[15][0] = -(ballxdif / totaldiff * cueballSpeed);
        ballVelocities[15][1] = -(ballydif / totaldiff * cueballSpeed);
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
    ballPositions[15][0] = -10.0;
    ballPositions[15][1] = 0.0;

    //Store the default positions for resetting
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 2; j++)
            DefaultPositions[i][j] = ballPositions[i][j];
    }
    


    ballVelocities[15][0] = 0.0;


    //------------------------------------------------
    
    GLdouble xdif, ydif, temp;

    


    // ==============================================
// ====== Set up the stuff for our sphere =======
// ==============================================

// 1. Setup and compile the shader
    Shader poolBallShader("poolBallVertex.glsl", "poolBallFragment.glsl");


    // 2. Load the pool ball objects
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

    Model Balls[16] = { Ball1, Ball2, Ball3, Ball4, Ball5, Ball6, Ball7, Ball8, Ball9, Ball10, Ball11, Ball12, Ball13, Ball14, Ball15, CueBall };


    Model Hole1((GLchar*)"Hole.obj");
    Model Hole2((GLchar*)"Hole.obj");
    Model Hole3((GLchar*)"Hole.obj");
    Model Hole4((GLchar*)"Hole.obj");
    Model Hole5((GLchar*)"Hole.obj");
    Model Hole6((GLchar*)"Hole.obj");
    Model Pointer((GLchar*)"Hole.obj");


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




    // Define how and where the data will be passed to the shaders
    GLint lightPos = glGetUniformLocation(poolBallShader.Program, "lightPos");
    GLint viewPos = glGetUniformLocation(poolBallShader.Program, "viewPos");
    GLint lightCol = glGetUniformLocation(poolBallShader.Program, "lightColor");
    GLint lightType = glGetUniformLocation(poolBallShader.Program, "lightType");



     // ==================================================================
     // ====== Set up the changes we want while the window is open =======
     // ==================================================================

    while (!glfwWindowShouldClose(window))
    {

        // Pass the data in the variables to to go to the vertex shader
        glUniform3f(lightPos, 0, 0, 199.0); 
        glUniform3fv(viewPos, 1, glm::value_ptr(camera.Position));
        glUniform3fv(lightCol, 1, glm::value_ptr(lightColor));
        glUniform3fv(lightType, 1, glm::value_ptr(lightMode));

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

        //Array of pointers to reduce code repetition
        glm::mat4* BallModels[16] = { &Ball1Model, &Ball2Model, &Ball3Model, &Ball4Model, &Ball5Model, &Ball6Model, &Ball7Model, &Ball8Model, &Ball9Model, &Ball10Model, &Ball11Model, &Ball12Model, &Ball13Model, &Ball14Model, &Ball15Model, &CueBallModel };

        glm::mat4 Hole1Model = glm::mat4(1);
        glm::mat4 Hole2Model = glm::mat4(1);
        glm::mat4 Hole3Model = glm::mat4(1);
        glm::mat4 Hole4Model = glm::mat4(1);
        glm::mat4 Hole5Model = glm::mat4(1);
        glm::mat4 Hole6Model = glm::mat4(1);
        glm::mat4 PointerModel = glm::mat4(1);


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
                if (inHole[i]) {
                    continue;
                }
                if (inHole[j]) {
                    continue;
                }
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
                    //Forces applied at that angle
                    u1[0] = ballVelocities[i][0] * cos(theta) - ballVelocities[i][1] * sin(theta);
                    u1[1] = ballVelocities[i][0] * sin(theta) + ballVelocities[i][1] * cos(theta);
                    u2[0] = ballVelocities[j][0] * cos(theta) - ballVelocities[j][1] * sin(theta);
                    u2[1] = ballVelocities[j][0] * sin(theta) + ballVelocities[j][1] * cos(theta);
                    //Remove the theta rotation to calculate the final velocities of the ball
                    v1[0] = u2[0] * cos(-theta) - u1[1] * sin(-theta);
                    v1[1] = u2[0] * sin(-theta) + u1[1] * cos(-theta);
                    v2[0] = u1[0] * cos(-theta) - u2[1] * sin(-theta);
                    v2[1] = u1[0] * sin(-theta) + u2[1] * cos(-theta);

                    float overlapDirectionX = dx / distance;
                    float overlapDirectionY = dy / distance;
                    float totalOverlap = overlapDirectionX + overlapDirectionY;

                    ballVelocities[i][0] = ballVelocities[i][0] * 0.05 + v1[0];
                    ballVelocities[i][1] = ballVelocities[i][1] * 0.05 + v1[1];
                    ballVelocities[j][0] = ballVelocities[j][0] * 0.05 + v2[0];
                    ballVelocities[j][1] = ballVelocities[j][1] * 0.05 + v2[1];



                    // Move the balls away from each other along the direction of overlap

                    ballPositions[i][0] -= overlapDirectionX * overlap / 2.0f;
                    ballPositions[i][1] -= overlapDirectionY * overlap / 2.0f;
                    ballPositions[j][0] += overlapDirectionX * overlap / 2.0f;
                    ballPositions[j][1] += overlapDirectionY * overlap / 2.0f;

                }
            }
        }

        //Update all ball positions based on their velocities with a slight friction
        for (int i = 0; i < 16; i++) {
            ballPositions[i][0] += ballVelocities[i][0];
            ballPositions[i][1] += ballVelocities[i][1];
            if (ballVelocities[i][0] > 0) {
                ballVelocities[i][0] *= 0.997;
            }
            if (ballVelocities[i][0] < 0) {
                ballVelocities[i][0] *= 0.997;
            }
            if (ballVelocities[i][1] > 0) {
                ballVelocities[i][1] *= 0.997;
            }
            if (ballVelocities[i][1] < 0) {
                ballVelocities[i][1] *= 0.997;
            }
        }


      
        //If balls are in a hole, set inhole to true
        for (int i = 0; i < 16; i++) {
            if (inHole[i] == true) {
                continue;
            }
            //top right pocket
            if (ballPositions[i][0] > 180 && ballPositions[i][0] < 190 && ballPositions[i][1] > 90) {
                inHole[i] = true;
            }
            //top center pocket
            if (ballPositions[i][0] > -5 && ballPositions[i][0] < 5 && ballPositions[i][1] > 90) {
                inHole[i] = true;
            }
            //top left pocket
            if (ballPositions[i][0] > -190 && ballPositions[i][0] < -180 && ballPositions[i][1] > 90) {
                inHole[i] = true;
            }
            //Bottom left pocket
            if (ballPositions[i][0] > -190 && ballPositions[i][0] < -180 && ballPositions[i][1] < -90) {
                inHole[i] = true;
            }
            //Bottom middle pocket
            if (ballPositions[i][0] > -5 && ballPositions[i][0] < 5 && ballPositions[i][1] < -90) {
                inHole[i] = true;
            }
            //Bottom right pocket
            if (ballPositions[i][0] > 180 && ballPositions[i][0] < 190 && ballPositions[i][1] < -90) {
                inHole[i] = true;
            }
            
        }

        


        // 3. Translate all the ball positions in a loop
        for (int i = 0; i < 16; i++) {
            *BallModels[i] = glm::translate(*BallModels[i], glm::vec3(ballPositions[i][0], ballPositions[i][1], 0.0f));
        }
   

        Hole1Model = glm::translate(Hole1Model, glm::vec3(-195.0f, 110.0f, 0.0f));
        Hole2Model = glm::translate(Hole2Model, glm::vec3(-0.0f, 115.0f, 0.0f));
        Hole3Model = glm::translate(Hole3Model, glm::vec3(195.0f, 110.0f, 0.0f));
        Hole4Model = glm::translate(Hole4Model, glm::vec3(-195.0f, -110.0f, 0.0f));
        Hole5Model = glm::translate(Hole5Model, glm::vec3(0.0f, -115.0f, 0.0f));
        Hole6Model = glm::translate(Hole6Model, glm::vec3(195.0f, -110.0f, 0.0f));

        double xpos, ypos;
        float pointerX;
        float pointerY;
        glfwGetCursorPos(window, &xpos, &ypos);
        mouseX = xpos;
        mouseY = ypos;
        pointerX = (397 * (mouseX / 1280) ) - 198.5;
        pointerY = (223 * ((720 - mouseY) / 720)) - 111.5;

        PointerModel = glm::translate(PointerModel, glm::vec3(pointerX, pointerY, 0.0f));


        // 4. Scale all the balls in a loop
        for (int i = 0; i < 16; i++) {
            *BallModels[i] = glm::scale(*BallModels[i], glm::vec3(6.0f, 6.0f, 6.0f));
        }

        Hole1Model = glm::scale(Hole1Model, glm::vec3(15.0f, 15.0f, 15.0f));
        Hole2Model = glm::scale(Hole2Model, glm::vec3(15.0f, 15.0f, 15.0f));
        Hole3Model = glm::scale(Hole3Model, glm::vec3(15.0f, 15.0f, 15.0f));
        Hole4Model = glm::scale(Hole4Model, glm::vec3(15.0f, 15.0f, 15.0f));
        Hole5Model = glm::scale(Hole5Model, glm::vec3(15.0f, 15.0f, 15.0f));
        Hole6Model = glm::scale(Hole6Model, glm::vec3(15.0f, 15.0f, 15.0f));

        PointerModel = glm::scale(PointerModel, glm::vec3(6.0f, 6.0f, 6.0f));




        // Rotate all the balls in a loop
        for (int i = 0; i < 16; i++) {
            *BallModels[i] = glm::rotate(*BallModels[i], -45.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        }

        Hole1Model = glm::rotate(Hole1Model, -45.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        Hole2Model = glm::rotate(Hole2Model, -45.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        Hole3Model = glm::rotate(Hole3Model, -45.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        Hole4Model = glm::rotate(Hole4Model, -45.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        Hole5Model = glm::rotate(Hole5Model, -45.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        Hole6Model = glm::rotate(Hole6Model, -45.0f, glm::vec3(1.0f, 0.0f, 0.0f));

        PointerModel = glm::rotate(PointerModel, -45.0f, glm::vec3(1.0f, 0.0f, 0.0f));




        GLfloat BallAngles[16] = { -45.0f };

       


        //* JOSHUA LASHLEY BEGIN *//
        // Make the pool balls rotate in the direction they are moving
        for (int i = 0; i < 16; i++) {
            GLfloat poolBallvelocity = sqrt(ballVelocities[i][0] * ballVelocities[i][0] + ballVelocities[i][1] * ballVelocities[i][1]);
            if (poolBallvelocity != 0) {
                //Angle incremented based on the speed of the ball
                angle[i] += poolBallvelocity / 12;
                //Calculate the cross product of velocity vectors to find the axis of rotation perpendicular to them, y velocity is converted to z velocity in order to do this
                glm::vec3 axis = glm::cross(glm::vec3(ballVelocities[i][0], 0.0f, ballVelocities[i][1] ), glm::vec3(0.0f, 1.0f, 0.0f));
                *BallModels[i] = glm::rotate(*BallModels[i], angle[i], axis);
            }
        }
        //Draw the balls if they are not in a hole
        for (int i = 0; i < 16; i++) {
            if (inHole[i] == false) {
                glUniformMatrix4fv(glGetUniformLocation(poolBallShader.Program, "model"), 1,
                    GL_FALSE, glm::value_ptr(*BallModels[i]));
                Balls[i].Draw(poolBallShader);

            }
        }
        //* JOSHUA LASHLEY END *//

        glUniformMatrix4fv(glGetUniformLocation(poolBallShader.Program, "model"), 1,
            GL_FALSE, glm::value_ptr(Hole1Model));

        Hole1.Draw(poolBallShader);

        glUniformMatrix4fv(glGetUniformLocation(poolBallShader.Program, "model"), 1,
            GL_FALSE, glm::value_ptr(Hole2Model));

        Hole2.Draw(poolBallShader);

        glUniformMatrix4fv(glGetUniformLocation(poolBallShader.Program, "model"), 1,
            GL_FALSE, glm::value_ptr(Hole3Model));

        Hole3.Draw(poolBallShader);

        glUniformMatrix4fv(glGetUniformLocation(poolBallShader.Program, "model"), 1,
            GL_FALSE, glm::value_ptr(Hole4Model));

        Hole4.Draw(poolBallShader);

        glUniformMatrix4fv(glGetUniformLocation(poolBallShader.Program, "model"), 1,
            GL_FALSE, glm::value_ptr(Hole5Model));

        Hole5.Draw(poolBallShader);

        glUniformMatrix4fv(glGetUniformLocation(poolBallShader.Program, "model"), 1,
            GL_FALSE, glm::value_ptr(Hole6Model));

        Hole6.Draw(poolBallShader);

        glUniformMatrix4fv(glGetUniformLocation(poolBallShader.Program, "model"), 1,
            GL_FALSE, glm::value_ptr(PointerModel));

        Pointer.Draw(poolBallShader);



        /*--/////////////////Section below - Done by Zachary Farrell///////////--*/
        //////////////////////////////////////////////////////////////////////////



        // =======================================================================
        // Creating the model matrix 
        // =======================================================================
        glm::mat4 poolStickModel = glm::mat4(1);

        if (isHoldingKey)
        {
            poolStickX -= poolStickInc;
        }

        //Modify the model matrix with scaling, translation, rotation, etc
        poolStickModel = glm::scale(poolStickModel, glm::vec3(2.0f, 2.0f, 2.0f));
        poolStickModel = glm::translate(poolStickModel, glm::vec3(poolStickX, poolStickY, 20.0f));
        poolStickModel = glm::rotate(poolStickModel, poolStickAngle, glm::vec3(0.0f, 1.0f, 0.0f));

       
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
