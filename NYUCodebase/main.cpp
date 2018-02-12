#ifdef _WINDOWS
	#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include "Matrix.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;

/*GLuint LoadTexture(const char *filePath) {
	int w, h, comp;
	unsigned char* image = stbi_load(filePath, &w, &h, &comp, STBI_rgb_alpha);
	if (image == NULL) {
		std::cout << "Unable to load image. Make sure the path is correct\n";
		return -1;
	}
	GLuint retTexture;
	glGenTextures(1, &retTexture);
	glBindTexture(GL_TEXTURE_2D, retTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(image);
	return retTexture;
}*/

GLuint LoadTexture(const char *filePath) {
	int w, h, comp;
	unsigned char* image = stbi_load(filePath, &w, &h, &comp, STBI_rgb_alpha);
	if (image == NULL)
	{
		return -1;
	}
	GLuint retTexture;
	glGenTextures(1, &retTexture);
	glBindTexture(GL_TEXTURE_2D, retTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(image);
	return retTexture;
}

/*GLuint LoadTexture(const char *filePath)
{
	int w, h, comp;
	unsigned char* image = stbi_load(filePath, &w, &h, &comp, STBI_rgb_alpha);
	return 1;
}*/

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	#ifdef _WINDOWS
		glewInit();
	#endif

	SDL_Event event;
	bool done = false;
	glClearColor(.4f, .5f,.4f, 1.0f);
	ShaderProgram program;
	program.Load(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	Matrix projectionMatrix;
	Matrix modelMatrix;
	Matrix viewMatrix;
	projectionMatrix.SetOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
	float lastFrameTicks = 0.0f;
	GLuint duck = LoadTexture(RESOURCE_FOLDER"KOICHI.png");
	GLuint trueduck = LoadTexture(RESOURCE_FOLDER"ISINK.jpg");
	GLuint moreduck = LoadTexture(RESOURCE_FOLDER"IDROWN.jpg");
	if (duck == -1)
	{
		std::cout << "texture failed to load\n";
	}
	int growmaybe = 0;
	bool indeedgrow = true;
	glUseProgram(program.programID);

	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;
		if (indeedgrow)
		{
			modelMatrix.Scale(1.01f, 1.01f, 1.0f);
			growmaybe += 1;
		}
		else
		{
			modelMatrix.Scale((1/1.01f), (1/1.01f), 1.0f);
			growmaybe -= 1;
		}
		if (growmaybe >= 20)
		{
			indeedgrow = false;
		}
		if (growmaybe <= 0)
		{
			indeedgrow = true;
		}

		modelMatrix.Rotate(20.0f * elapsed * (3.1415926f / 180.0f));
		glClear(GL_COLOR_BUFFER_BIT);
		//set models
		program.SetModelMatrix(modelMatrix);
		program.SetProjectionMatrix(projectionMatrix);
		program.SetViewMatrix(viewMatrix);
		glBindTexture(GL_TEXTURE_2D, duck);
		float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
		glEnableVertexAttribArray(program.positionAttribute);
		float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(program.texCoordAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//second
		float newverts[] = { 1.0, 1.0, 1.5, 1.0, 1.0, 1.5 };
		float newtexcoords[] = { 0.0, 0.0, 1.0,1.0, 1.0,0.0 };
		glBindTexture(GL_TEXTURE_2D, trueduck);
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, newverts);
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, newtexcoords);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// last
		float moreverts[] = { -.75,-.25,-.25,-.25,-.25,.25,-.75,-.25,-.25,.25,-.75,.25};
		glBindTexture(GL_TEXTURE_2D, moreduck);
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, moreverts);
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);
		
		SDL_GL_SwapWindow(displayWindow);
	}
	SDL_Quit();
	return 0;
}
