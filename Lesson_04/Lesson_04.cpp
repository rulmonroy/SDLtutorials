#include <iostream>
#include <SDL.h>
#include <string>
#include <SDL_image.h>

/**
* Log an SDL error with some error message to the output stream of our choice
* @param os The output stream to write the message too
* @param msg The error message to write, format will be msg error: SDL_GetError()
*/
void logSDLError(std::ostream &os, const std::string &msg) {
	std::cout << msg << " error: " << IMG_GetError() << std::endl;
}

/*---------------------------------------------------------------------
 Loads an image into a texture on the rendering device
 @param file The image file to load
 @param ren The renderer to load the texture onto
 @return the loaded texture, or nullptr if something went wrong.
---------------------------------------------------------------------*/
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren){
	SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());

	if (texture == nullptr)		
		logSDLError(std::cout, "LoadTexture");
	return texture;
}

/*---------------------------------------------------------------------
 Draw an SDL_Texture to an SDL_Renderer at position x, y, with some desired
 width and height
 @param tex The source texture we want to draw
 @param rend The renderer we want to draw too
 @param x The x coordinate to draw too
 @param y The y coordinate to draw too
 @param w The width of the texture to draw
 @param h The height of the texture to draw
---------------------------------------------------------------------*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h){
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

/*---------------------------------------------------------------------
 Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
 the texture's width and height
 @param tex The source texture we want to draw
 @param ren The renderer we want to draw too
 @param x The x coordinate to draw too
 @param y The y coordinate to draw too
---------------------------------------------------------------------*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y) {
	int w, h;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	renderTexture(tex, ren, x, y, w, h);
}

// --------------------------------------------------------------------
// --------------------------------------------------------------------
int main(int argc, char **argv){

	const int SCREEN_WIDTH  = 640;
	const int SCREEN_HEIGHT = 480;
	const int TILE_SIZE = 40;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		logSDLError(std::cout, "SDL_Init");
		return 1;
	}

	// Inicializaci�n de SDL_Image
	if ( (IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG ) {
		logSDLError(std::cout, "IMG_Init");
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow("Lesson 4", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (window == nullptr) {
		logSDLError(std::cout, "CreateWindow");
		return 2;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,	SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (renderer == nullptr) {
		logSDLError(std::cout, "CreateRenderer");
		return 3;
	}

	SDL_Texture *image = loadTexture("../img/lesson_04/image.png", renderer);

	if (image == nullptr)
		return 4;

	SDL_Event e;
	bool quit = false;

	while ( !quit ) {
		while (SDL_PollEvent(&e)){
			//If user closes the window
			if (e.type == SDL_QUIT)
				quit = true;

			//If user presses any key
			if (e.type == SDL_KEYDOWN)
				quit = true;

			//If user clicks the mouse
			if (e.type == SDL_MOUSEBUTTONDOWN)
				quit = true;
		}
		// Mostrar imagen
		SDL_RenderClear(renderer);
		renderTexture(image, renderer, 0, 0);
		SDL_RenderPresent(renderer);
	}

	// Liberar memoria.
	SDL_DestroyTexture(image);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	
	IMG_Quit();
	SDL_Quit();

	return 0;
}
