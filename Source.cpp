#include <iostream>         // cout, cerr
#include <cstdlib>          // EXIT_FAILURE

#include "textrenderer.h"
#include <GL/glew.h>        // GLEW library
#include <GLFW/glfw3.h>     // GLFW library

// GLM Math Header inclusions
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h "     // Image loading Utility functions
#include "meshes.h"

// Uses the standard namespace for debug output
using namespace std;

// Unnamed namespace for C++ defines //
namespace
{
	// Macro for OpenGL window title
	const char* const WINDOW_TITLE = "Jon-Kayla Pointer, Link's Dog House";

	// Variables for window width and height
	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 800;

	// Main GLFW window
	GLFWwindow* gWindow = nullptr;
	// Shader program
	GLuint gProgramId1;
	GLuint gProgramId2;
	// Texture Ids
	GLuint gTextureId;
	GLuint gTextureId2;
	GLuint gTextureId3;
	GLuint gTextureId4;
	GLuint gTextureId5;
	GLuint gTextureId6;
	GLuint gTextureId7;
	Meshes meshes;
	glm::vec2 gUVScale(5.0f, 5.0f);
	GLint gTexWrapMode = GL_REPEAT;

	// Front Camera

	GLuint gLampProgramId;


	Camera gCamera(glm::vec3(0.0f, 0.0f, 5.0f));

	// camera
	glm::vec3 gCameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 gCameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 gCameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 gCameraRight = glm::vec3(1.0f, 0.0f, -1.0f);

	float gLastX = WINDOW_WIDTH / 2.0f;
	float gLastY = WINDOW_HEIGHT / 2.0f;
	// Set the camera parameters 

	GLuint gCurrentCameraIndex = 1;
	GLfloat gCameraZoom = 50.0f;
	GLuint gCameraOrtho = 0;

	bool gFirstMouse = true;
	// timing
	float gDeltaTime = 0.0f; // time between current frame and last frame
	float gLastFrame = 0.0f;

}

// User-defined Function prototypes //
bool Initialize(int, char* [], GLFWwindow** window);
void ProcessInput(GLFWwindow* window);
void Render();
bool CreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId);
void DestroyShaderProgram(GLuint programId);
bool CreateTexture(const char* filename, GLuint& textureId);
void DestroyTexture(GLuint textureId);
void UResizeWindow(GLFWwindow* window, int width, int height);
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos);
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

// Shader program Macro //
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

// Vertex Shader Source Code //
const GLchar* vertexShaderSource1 = GLSL(440,
	// Load vertex data from VBO location 0
	layout(location = 0) in vec3 vertex;
// Load vertex normal data from VBO location 1
layout(location = 1) in vec3 vertexNormal;
// Load texture coordinate data from VBO location 2
layout(location = 2) in vec2 textureCoordinate;

// Output vertex normal values to fragment shader
out vec3 vs_vertexNormal;
// Output texture coordinates to fragment shader
out vec2 vs_textureCoordinate;

//Global variables for the model-view-projection
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	// Transforms vertices to clip coordinates
	gl_Position = projection * view * model * vec4(vertex, 1.0f);
	vs_textureCoordinate = textureCoordinate;
	vs_vertexNormal = vertexNormal;
}
);

// Fragment Shader Source Code //
const GLchar* fragmentShaderSource1 = GLSL(440,
	// Vertex normal from vertex shader
	in vec3 vs_vertexNormal;
// Texture coordinates from vertex shader
in vec2 vs_textureCoordinate;

// Fragment output color
out vec4 fragmentColor;

// Texture image data passed in from outside shader
uniform sampler2D uTextureBase;
uniform vec2 uvScale;
uniform bool ubHasTexture;
uniform vec4 uCustomColor;

void main()
{
	// if a texture is passed in, go ahead and use it
	if (ubHasTexture)
		fragmentColor = texture(uTextureBase, vs_textureCoordinate);// *uvScale);
	else // otherwise just use preset color
		fragmentColor = uCustomColor;
}
);

// main function. Entry point to the OpenGL program //


int main(int argc, char** argv)
{
	if (!Initialize(argc, argv, &gWindow))
		return EXIT_FAILURE;
	// Load texture links roof
	const char* texFilename = "container.png";
	if (!CreateTexture(texFilename, gTextureId))
	{
		cout << "Failed to load texture " << texFilename << endl;
		return EXIT_FAILURE;
	}
	// Load texture links panels
	const char* texFilename2 = "container2.png";
	if (!CreateTexture(texFilename2, gTextureId2))
	{
		cout << "Failed to load texture " << texFilename << endl;
		return EXIT_FAILURE;
	}
	// Load texture grass
	const char* texFilename3 = "container3.png";
	if (!CreateTexture(texFilename3, gTextureId3))
	{
		cout << "Failed to load texture " << texFilename << endl;
		return EXIT_FAILURE;
	}
	// Load texture ball
	const char* texFilename4 = "container4.png";
	if (!CreateTexture(texFilename4, gTextureId4))
	{
		cout << "Failed to load texture " << texFilename << endl;
		return EXIT_FAILURE;
	}

	// Load texture tire
	const char* texFilename5 = "container5.png";
	if (!CreateTexture(texFilename5, gTextureId5))
	{
		cout << "Failed to load texture " << texFilename5 << endl;
		return EXIT_FAILURE;
	}
	const char* texFilename6 = "container6.jpg";
	if (!CreateTexture(texFilename6, gTextureId6))
	{
		cout << "Failed to load texture " << texFilename6 << endl;
		return EXIT_FAILURE;
	}
	const char* texFilename7 = "container7.jpg";
	if (!CreateTexture(texFilename7, gTextureId7))
	{
		cout << "Failed to load texture " << texFilename7 << endl;
		return EXIT_FAILURE;
	}
	// Create the mesh, send data to VBO
	meshes.CreateMeshes();

	// Create the shader program
	if (!CreateShaderProgram(vertexShaderSource1, fragmentShaderSource1, gProgramId1))
		return EXIT_FAILURE;

	// Load texture data from file
	//const char * texFilename1 = "../../resources/textures/blue_granite.jpg";
	//if (!CreateTexture(texFilename1, gTextureIdBlue))
	//{
	//	cout << "Failed to load texture " << texFilename1 << endl;
	//	return EXIT_FAILURE;
	//}

	// Activate the program that will reference the texture
	glUseProgram(gProgramId1);
	// We set the texture as texture unit 0
	glUniform1i(glGetUniformLocation(gProgramId1, "uTextureBase"), 0);

	// Sets the background color of the window to black (it will be implicitely used by glClear)
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glfreetype::font_data our_font;


	our_font.init("morganchalk.ttf", 25 /* size */);

	// Render loop
	while (!glfwWindowShouldClose(gWindow))
	{
		// Process keyboard input before rendering
		// per-frame timing
		// --------------------
		float currentFrame = glfwGetTime();
		gDeltaTime = currentFrame - gLastFrame;
		gLastFrame = currentFrame;
		ProcessInput(gWindow);
		glPushMatrix();
		glLoadIdentity();

		// Blue text
		glColor3ub(0, 0, 0xff);

		glfreetype::print(our_font, 20 /* xpos */, 20 /* ypos */,
			"The quick brown fox blah blah blah");

		glPopMatrix();
		// Render this frame
		Render();

		glfwPollEvents();
	}
	our_font.clean();
	glfwTerminate();
	return 0;
	// Release mesh data
	meshes.DestroyMeshes();
	// Release shader program
	DestroyShaderProgram(gProgramId1);
	// Release the textures
	//DestroyTexture(gTextureId);

	exit(EXIT_SUCCESS); // Terminates the program successfully

   
   
 }



// Initialize GLFW, GLEW, and create a window //
bool Initialize(int argc, char* argv[], GLFWwindow** window)
{
	// GLFW: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// GLFW: create OpenGL output window, return error if fails
	*window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (*window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	// Set the context for the current window
	glfwMakeContextCurrent(*window);
	glfwSetFramebufferSizeCallback(*window, UResizeWindow);
	glfwSetCursorPosCallback(*window, UMousePositionCallback);
	glfwSetScrollCallback(*window, UMouseScrollCallback);
	glfwSetMouseButtonCallback(*window, UMouseButtonCallback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// GLEW: initialize
	// ----------------
	// Note: if using GLEW version 1.13 or earlier
	glewExperimental = GL_TRUE;
	GLenum GlewInitResult = glewInit();
	// If init fails, output error string, return error
	if (GLEW_OK != GlewInitResult)
	{
		std::cerr << glewGetErrorString(GlewInitResult) << std::endl;
		return false;
	}

	// Displays GPU OpenGL version
	cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << endl;

	return true;
}

// Process keyboard input
void ProcessInput(GLFWwindow* window)
{
	static const float cameraSpeed = 2.5f;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraOffset = cameraSpeed * gDeltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		gCameraPos += cameraOffset * gCameraFront;
	


	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		gCameraPos -= cameraOffset * gCameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		gCameraPos -= glm::normalize(glm::cross(gCameraFront, gCameraUp)) * cameraOffset;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		gCameraPos += glm::normalize(glm::cross(gCameraFront, gCameraUp)) * cameraOffset;

	float xoffset = 0.0;
	float yoffset = 0.0;
	gCamera.ProcessMouseMovement(xoffset, yoffset);

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		//gCameraPos -= float(xoffset) + float(yoffset) * gCameraFront;
		gCameraPos -= glm::normalize(glm::cross(gCameraFront, gCameraRight)) * cameraOffset;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		gCameraPos += glm::normalize(glm::cross(gCameraFront, gCameraRight)) * cameraOffset;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		
		gCameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && gTexWrapMode != GL_REPEAT)
	{
		glBindTexture(GL_TEXTURE_2D, gTextureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBindTexture(GL_TEXTURE_2D, 0);

		gTexWrapMode = GL_REPEAT;

		cout << "Current Texture Wrapping Mode: REPEAT" << endl;
	}
	else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && gTexWrapMode != GL_MIRRORED_REPEAT)
	{
		glBindTexture(GL_TEXTURE_2D, gTextureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glBindTexture(GL_TEXTURE_2D, 0);

		gTexWrapMode = GL_MIRRORED_REPEAT;

		cout << "Current Texture Wrapping Mode: MIRRORED REPEAT" << endl;
	}
	else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && gTexWrapMode != GL_CLAMP_TO_EDGE)
	{
		glBindTexture(GL_TEXTURE_2D, gTextureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_2D, 0);

		gTexWrapMode = GL_CLAMP_TO_EDGE;

		cout << "Current Texture Wrapping Mode: CLAMP TO EDGE" << endl;
	}
	else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && gTexWrapMode != GL_CLAMP_TO_BORDER)
	{
		float color[] = { 1.0f, 0.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

		glBindTexture(GL_TEXTURE_2D, gTextureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glBindTexture(GL_TEXTURE_2D, 0);

		gTexWrapMode = GL_CLAMP_TO_BORDER;

		cout << "Current Texture Wrapping Mode: CLAMP TO BORDER" << endl;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS)
	{
		gUVScale += 0.1f;
		cout << "Current scale (" << gUVScale[0] << ", " << gUVScale[1] << ")" << endl;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS)
	{
		gUVScale -= 0.1f;
		cout << "Current scale (" << gUVScale[0] << ", " << gUVScale[1] << ")" << endl;
	}
}
void UResizeWindow(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
// Render the next frame to the OpenGL viewport //
void Render()
{
	GLuint uHasTextureLoc;
	bool ubHasTextureVal;
	GLuint uCustomColorLoc;
	glm::mat4 scale;
	glm::mat4 rotation;
	glm::mat4 rotation1;
	glm::mat4 rotation2;
	glm::mat4 translation;
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 model;
	GLint modelLoc;
	GLint viewLoc;
	GLint projLoc;

	// Enable z-depth
	glEnable(GL_DEPTH_TEST);

	// Clear the background
	//
	glClearColor((float)149 / 255, (float)225 / 255, (float)233 / 255, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	view = glm::lookAt(gCameraPos, gCameraPos + gCameraFront, gCameraUp);


	projection = glm::perspective(glm::radians(50.0f), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 10.0f);

	// Set the program to be used
	glUseProgram(gProgramId1);

	// Get the has texture location
	uHasTextureLoc = glGetUniformLocation(gProgramId1, "ubHasTexture");
	// Get the custom color location
	uCustomColorLoc = glGetUniformLocation(gProgramId1, "uCustomColor");

	ubHasTextureVal = true;
	glUniform1i(uHasTextureLoc, ubHasTextureVal);


	//**LINKS HOUSE
	// Retrieves and passes transform matrices to the Shader program
	modelLoc = glGetUniformLocation(gProgramId1, "model");
	viewLoc = glGetUniformLocation(gProgramId1, "view");
	projLoc = glGetUniformLocation(gProgramId1, "projection");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	GLint UVScaleLoc = glGetUniformLocation(gProgramId1, "uvScale");
	glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

	// Activate the VBOs contained within the mesh's VAO
	glBindVertexArray(meshes.gBoxMesh.vao);

	// Set the mesh transfomation values
	scale = glm::scale(glm::vec3(0.4f, 0.2f, 0.2f));
	rotation = glm::rotate(-0.2f, glm::vec3(0.35f, 0.5f, 0.0f));
	translation = glm::translate(glm::vec3(0.5f, -0.05f, 0.0f));
	model = translation * rotation * scale;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// Set the custom color before drawing
	//glUniform4fv(uCustomColorLoc, 1, glm::value_ptr(glm::vec4(0.53f, 0.62f, 0.69f, 1.0f)));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTextureId2);

	glDrawElements(GL_TRIANGLES, meshes.gBoxMesh.nIndices, GL_UNSIGNED_INT, (void*)0);

	// Deactivate the VAO for box
	glBindVertexArray(0);




	///
	//***Makinking Link's roof
	// Activate the VBOs contained within the mesh's VAO
	glBindVertexArray(meshes.gTorusMesh.vao);

	// Set the mesh transfomation values
	scale = glm::scale(glm::vec3(0.25f, 0.26f, 0.65f));
	rotation = glm::rotate(-1.73f, glm::vec3(4.0f, -3.0f, 0.0f));
	translation = glm::translate(glm::vec3(-1.0f, -0.03f, 0.15f));
	model = translation * rotation * scale;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// Set the custom color before drawing
	//glUniform4fv(uCustomColorLoc, 1, glm::value_ptr(glm::vec4(0.53f, 0.62f, 0.69f, 1.0f)));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTextureId5);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, meshes.gTorusMesh.nVertices);

	// Deactivate the VAO for box
	glBindVertexArray(0);


	///	obstacle by tire

	glBindVertexArray(meshes.gCylinderMesh.vao);

	// Set the mesh transfomation values
	scale = glm::scale(glm::vec3(0.2f, 0.5f, 0.2f));
	rotation = glm::rotate(-1.73f, glm::vec3(9.8f, -3.5f, 0.0f));
	translation = glm::translate(glm::vec3(-1.3f, 0.19f, 0.5f));
	model = translation * rotation * scale;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// Set the custom color before drawing
	//glUniform4fv(uCustomColorLoc, 1, glm::value_ptr(glm::vec4(0.53f, 0.62f, 0.69f, 1.0f)));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTextureId7);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, meshes.gCylinderMesh.nVertices);

	// Deactivate the VAO for box
	glBindVertexArray(0);

	//legs for dog obstacle 1
	glBindVertexArray(meshes.gCylinderMesh.vao);

	// Set the mesh transfomation values
	scale = glm::scale(glm::vec3(0.008f, 0.24f, 0.008f));
	rotation = glm::rotate( 0.0f, glm::vec3(1.0f, 0.8f, 0.8f));
	translation = glm::translate(glm::vec3(-1.3f, -0.15f, 0.0f));
	model = translation * rotation * scale;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// Set the custom color before drawing
	//glUniform4fv(uCustomColorLoc, 1, glm::value_ptr(glm::vec4(0.53f, 0.62f, 0.69f, 1.0f)));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTextureId6);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, meshes.gCylinderMesh.nVertices);

	// Deactivate the VAO for box
	glBindVertexArray(0);


	//legs for dog obstacle 2
	glBindVertexArray(meshes.gCylinderMesh.vao);

	// Set the mesh transfomation values
	scale = glm::scale(glm::vec3(0.008f, 0.24f, 0.008f));
	rotation = glm::rotate(0.0f, glm::vec3(1.0f, 0.8f, 0.8f));
	translation = glm::translate(glm::vec3(-1.60f, -0.15f, 0.2f));
	model = translation * rotation * scale;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// Set the custom color before drawing
	//glUniform4fv(uCustomColorLoc, 1, glm::value_ptr(glm::vec4(0.53f, 0.62f, 0.69f, 1.0f)));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTextureId6);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, meshes.gCylinderMesh.nVertices);

	// Deactivate the VAO for box
	glBindVertexArray(0);


	//legs for dog obstacle 3
	glBindVertexArray(meshes.gCylinderMesh.vao);

	// Set the mesh transfomation values
	scale = glm::scale(glm::vec3(0.008f, 0.22f, 0.008f));
	rotation = glm::rotate(0.0f, glm::vec3(1.0f, 0.8f, 0.8f));
	translation = glm::translate(glm::vec3(-1.45f, -0.15f, 0.50f));
	model = translation * rotation * scale;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// Set the custom color before drawing
	//glUniform4fv(uCustomColorLoc, 1, glm::value_ptr(glm::vec4(0.53f, 0.62f, 0.69f, 1.0f)));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTextureId6);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, meshes.gCylinderMesh.nVertices);

	// Deactivate the VAO for box
	glBindVertexArray(0);

	//legs for dog obstacle 4
	glBindVertexArray(meshes.gCylinderMesh.vao);

	// Set the mesh transfomation values
	scale = glm::scale(glm::vec3(0.008f, 0.22f, 0.008f));
	rotation = glm::rotate(0.0f, glm::vec3(1.0f, 0.8f, 0.8f));
	translation = glm::translate(glm::vec3(-1.2f, -0.15f, 0.30f));
	model = translation * rotation * scale;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// Set the custom color before drawing
	//glUniform4fv(uCustomColorLoc, 1, glm::value_ptr(glm::vec4(0.53f, 0.62f, 0.69f, 1.0f)));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTextureId6);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, meshes.gCylinderMesh.nVertices);

	// Deactivate the VAO for box
	glBindVertexArray(0);

	//***Makinking Link's roof
	// Activate the VBOs contained within the mesh's VAO
	glBindVertexArray(meshes.gPrismMesh.vao);

	// Set the mesh transfomation values
	scale = glm::scale(glm::vec3(0.72f, 0.16f, 0.2f));
	rotation = glm::rotate(-1.73f, glm::vec3(4.8f, 0.0f, 0.0f));
	translation = glm::translate(glm::vec3(0.5f, 0.15f, 0.0f));
	model = translation * rotation * scale;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// Set the custom color before drawing
	//glUniform4fv(uCustomColorLoc, 1, glm::value_ptr(glm::vec4(0.53f, 0.62f, 0.69f, 1.0f)));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTextureId);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, meshes.gPrismMesh.nVertices);

	// Deactivate the VAO for box
	glBindVertexArray(0);
	
	//***Making Link's ball
	// Activate the VBOs contained within the mesh's VAO
	glBindVertexArray(meshes.gSphereMesh.vao);

	// Set the mesh transfomation values
	scale = glm::scale(glm::vec3(0.04f, 0.04f, 0.04f));
	rotation = glm::rotate(0.0f, glm::vec3(0.8f, 0.0f, 0.0f));
	translation = glm::translate(glm::vec3(-0.5f, -0.10f, 0.0f));
	model = translation * rotation * scale;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// Set the custom color before drawing
	//glUniform4fv(uCustomColorLoc, 1, glm::value_ptr(glm::vec4(0.53f, 0.62f, 0.69f, 1.0f)));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTextureId4);
	glDrawElements(GL_TRIANGLES, meshes.gSphereMesh.nIndices, GL_UNSIGNED_INT, (void*)0);

	// Deactivate the VAO for box
	glBindVertexArray(0);



	//***making the grass(PLANE)


	// Activate the VBOs contained within the mesh's VAO
	glBindVertexArray(meshes.gPlaneMesh.vao);

	// Set the mesh transfomation values
	scale = glm::scale(glm::vec3(2.0f, 2.39f, 2.2f));
	rotation = glm::rotate(0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	translation = glm::translate(glm::vec3(0.0f, -0.15f, 0.0f));
	model = translation * rotation * scale;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// Set the custom color before drawing
	//glUniform4fv(uCustomColorLoc, 1, glm::value_ptr(glm::vec4(0.53f, 0.62f, 0.69f, 1.0f)));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTextureId3);
	glDrawElements(GL_TRIANGLES, meshes.gPlaneMesh.nIndices, GL_UNSIGNED_INT, (void*)0);




	// Deactivate the VAO for box
	glBindVertexArray(0);
	// Flips the the back buffer with the front buffer every frame (refresh)
	glfwSwapBuffers(gWindow);
}

//****************************************************
//  const char* vtxShaderSource: vertex shader source code
//  const char* fragShaderSource: fragment shader source code
//  GLuint &programId: unique ID of program associated with shaders
//****************************************************
bool CreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId)
{
	// Compilation and linkage error reporting
	int success = 0;
	char infoLog[512];

	// Create a Shader program object.
	programId = glCreateProgram();

	// Create the vertex and fragment shader objects
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	// Retrieve the shader source
	glShaderSource(vertexShaderId, 1, &vtxShaderSource, NULL);
	glShaderSource(fragmentShaderId, 1, &fragShaderSource, NULL);

	// Compile the vertex shader, and print compilation errors (if any)
	glCompileShader(vertexShaderId);
	// Check for vertex shader compile errors
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}

	// Compile the fragment shader, and print compilation errors (if any)
	glCompileShader(fragmentShaderId);
	// Check for fragment shader compile errors
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderId, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}

	// Attached compiled shaders to the shader program
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);

	// Links the shader program
	glLinkProgram(programId);
	// Check for linking errors
	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(programId, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		return false;
	}

	// Uses the shader program
	glUseProgram(programId);

	return true;
}

// Destroy the linked shader program //
void DestroyShaderProgram(GLuint programId)
{
	glDeleteProgram(programId);
}

// Images are loaded with Y axis going down, but OpenGL's Y axis goes up, so let's flip it //
void flipImageVertically(unsigned char* image, int width, int height, int channels)
{
	for (int j = 0; j < height / 2; ++j)
	{
		int index1 = j * width * channels;
		int index2 = (height - 1 - j) * width * channels;

		for (int i = width * channels; i > 0; --i)
		{
			unsigned char tmp = image[index1];
			image[index1] = image[index2];
			image[index2] = tmp;
			++index1;
			++index2;
		}
	}
}
// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (gFirstMouse)
	{
		gLastX = xpos;
		gLastY = ypos;
		gFirstMouse = false;
	}

	float xoffset = xpos - gLastX;
	float yoffset = gLastY - ypos; // reversed since y-coordinates go from bottom to top

	gLastX = xpos;
	gLastY = ypos;

	gCamera.ProcessMouseMovement(xoffset, yoffset);

	gCameraPos += glm::normalize(glm::cross(gCameraFront, gCameraUp)) * (float(xoffset) + float(yoffset)) * 0.0025f;
}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	
	gCamera.ProcessMouseScroll(yoffset);
	gCameraPos += float(xoffset) + float(yoffset) * gCameraFront;
}

// glfw: handle mouse button events
// --------------------------------
void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
	{
		if (action == GLFW_PRESS)
			cout << "Left mouse button pressed" << endl;
		else
			cout << "Left mouse button released" << endl;
	}
	break;

	case GLFW_MOUSE_BUTTON_MIDDLE:
	{
		if (action == GLFW_PRESS)
			cout << "Middle mouse button pressed" << endl;
		else
			cout << "Middle mouse button released" << endl;
	}
	break;

	case GLFW_MOUSE_BUTTON_RIGHT:
	{
		if (action == GLFW_PRESS)
			cout << "Right mouse button pressed" << endl;
		else
			cout << "Right mouse button released" << endl;
	}
	break;

	default:
		cout << "Unhandled mouse button event" << endl;
		break;
	}
}


// Generate and load the texture //
bool CreateTexture(const char* filename, GLuint& textureId)
{
	int width, height, channels;
	unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);
	if (image)
	{
		flipImageVertically(image, width, height, channels);

		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);

		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (channels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		else if (channels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		else
		{
			cout << "Not implemented to handle image with " << channels << " channels" << endl;
			return false;
		}

		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

		return true;
	}

	// Error loading the image
	return false;
}

// Release the texture attached to textureId //
void DestroyTexture(GLuint textureId)
{
	glGenTextures(1, &textureId);
}


