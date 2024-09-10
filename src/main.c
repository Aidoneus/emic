#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "util.h"

const int wAspectX = 16;
const int wAspectY = 9;
const int wMulMin = 10;
const int wMulDefault = 60;
#define OPTC 4
const opt_config optv[OPTC] = {
	{'v', "version"},
	{'e', "echo"},
};

int shouldQuit = 0;
GLFWwindow* window;

void optVersion() {
	printf(
		"GIT_BRANCH: %s\nGITHUB_BRANCH: %s\nGIT_COMMIT_HASH: %s\nGITHUB_COMMIT_HASH: %s\n",
		GIT_BRANCH,
		GITHUB_BRANCH,
		GIT_COMMIT_HASH,
		GITHUB_COMMIT_HASH
	);
	shouldQuit = 1;
}

int main(int argc, char* argv[]) {
	int argIdx = 0, optIdx = -1;
	while ((optIdx = u_getopt(argc, argv, OPTC, optv, &argIdx, &optIdx)) != -1) {
		switch (optIdx) {
			case 0: optVersion(); break;
			case 1: printf("echo: %s\n", argv[++argIdx]); break;
		}
	}
	if (shouldQuit) return 0;

	// OpenGL setup
	glewExperimental = true;
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x anitaliasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Mainly for macOS
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#ifdef _WIN32
	// Consider possible scaling on Windows
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
#elif __APPLE__
	// For macOS too, at least as long as we don't have drawing adapted to these
	glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif

	// Create window & context
	window = glfwCreateWindow(wAspectX * wMulDefault, wAspectY * wMulDefault, "Emic", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. Might be caused by Intel GPU (they are not OpenGL 3.3 compatible).\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // Init GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	glfwSetWindowSizeLimits(window, wAspectX * wMulMin, wAspectY * wMulMin, GLFW_DONT_CARE, GLFW_DONT_CARE);
	glfwSetWindowAspectRatio(window, wAspectX, wAspectY);
	// Capture input
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	while (!shouldQuit) {
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();

		if (
			(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			|| glfwWindowShouldClose(window)
		) shouldQuit = 1;
	}

	return 0;
}
