#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <cstdlib>
//Screen dimension constants
const int SCREEN_WIDTH = 520;
const int SCREEN_HEIGHT = 520;
const int NUM_IMAGES = 25;
const int matrixsize=5;
std::vector<std::vector<int> > matrix(matrixsize,std::vector<int>(matrixsize));
std::vector<int> game_map(matrixsize*matrixsize);
char* imagePaths[NUM_IMAGES] = {
        "itstime/00.png", "itstime/01.png", "itstime/02.png", "itstime/03.png", "itstime/04.png",
        "itstime/10.png", "itstime/11.png", "itstime/12.png", "itstime/13.png", "itstime/14.png",
        "itstime/20.png", "itstime/21.png", "itstime/22.png", "itstime/23.png", "itstime/24.png",
        "itstime/30.png", "itstime/31.png", "itstime/32.png", "itstime/33.png", "itstime/34.png",
        "itstime/40.png", "itstime/41.png", "itstime/42.png", "itstime/43.png", "itstime/44.png",
    };
enum direction
    {
        up, down, left, right

    };
//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

// Array to store textures
SDL_Texture* textures[25] = { NULL };
SDL_Texture* changeTexture= NULL;

// Function to load images
SDL_Texture* loadTexture(char* path) {
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL Error: %s\n", path, SDL_GetError());
        return NULL;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    if (texture == NULL) {
        printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
        return NULL;
    }

    return texture;
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( ( IMG_Init( imgFlags ) != imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

/*void reorganize_textures()
{
    for (int i=0; i<NUM_IMAGES; i++)
    {
        textures[i] = loadTexture(imagePaths[game_map[i]]);

    }
}*/

bool loadMedia()
{
	//Loading success flag
	bool success = true;

    changeTexture = loadTexture("itstime/youwin.png");

    for (int i = 0; i < NUM_IMAGES; ++i) {
        textures[i] = loadTexture(imagePaths[i]);
        if (textures[i] == NULL) {
            success = false;
        }
    }

	return success;
}

void renderTextures() {
    SDL_Rect srcRect = { 0, 0, 100, 100 };
    SDL_Rect destRects[NUM_IMAGES];
    int numImagesPerRow = sqrt(NUM_IMAGES);
    for (int i=0; i<numImagesPerRow; i++)
    {
        for (int j=0; j<numImagesPerRow; j++)
        {
            destRects[i*numImagesPerRow+j] = {j*100, i*100, 100, 100};
        }
    }

    for (int i = 0; i < NUM_IMAGES; ++i) {
        SDL_RenderCopy(gRenderer, textures[game_map[i]], &srcRect, &destRects[i]);
    }
}

void close() {
    for (int i = 0; i < NUM_IMAGES; ++i) {
        SDL_DestroyTexture(textures[i]);
    }

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);

    IMG_Quit();
    SDL_Quit();
}

bool change_check(std::vector<std::vector<int> >&matrix)
{
    bool b = false;
    int i,j;
    for (i=0; i<matrixsize; i++)
    {
        for (j=0; j<matrixsize; j++)
        {
            if (matrix[i][j] != matrixsize*(i) + j) b=true ;
        }
    }
    return b;
}

bool swap_block (std::vector<std::vector<int> >& matrix, int direction, int emptyBlock )
{
    int i,j;
    int x,y;
    for (i=0; i< matrix[0].size(); i++)
    {
        for (j=0; j<matrix[0].size(); j++)
        {
            if (matrix[i][j]==emptyBlock) {x=i; y=j;}
        }
    }

    if (direction == up)
    {
        if (y==0) return false;
        std::swap(matrix[x][y],matrix[x][y-1]);
    }
    if (direction == down)
    {
        if (y== matrix[0].size()-1 ) return false;
        std::swap(matrix[x][y],matrix[x][y+1]);
    }
    if (direction == left)
    {
        if (x==0) return false;
        std::swap(matrix[x][y],matrix[x-1][y]);
    }
    if (direction == right)
    {
        if (x == matrix[0].size() - 1) return false;
        std::swap(matrix[x][y],matrix[x+1][y]);
    }
    return true;
}

void generate_map()
{

    int i,j;
    for (i=0; i<matrixsize; i++)
    {
        for (j=0; j<matrixsize; j++)
        {
            matrix[i][j] = matrixsize*(i) + j ;
        }
    }
    int emptyBlock = matrixsize*matrixsize-1;
    /*for (i=0; i<matrixsize; i++)
    {
        for (j=0; j<matrixsize; j++)
        {
            std::cout << std::setw(4) << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }*/

    int randtimes=20;

    std::srand(static_cast<unsigned int>(std::time(NULL)));
    std::vector<int> random_numbers;
    i=0;
    while (i<randtimes)
    {
        random_numbers.push_back( std::rand() % 4); // Generate random number between 0 and 3
        int direction = random_numbers.back();
        if (swap_block(matrix, direction, emptyBlock) == true)
        {
            i++;
            /*for (int k=0; k<matrixsize; k++)
            {
                for (int q=0; q<matrixsize; q++)
                {
                    std::cout << std::setw(4) <<matrix[k][q] << " ";
                }
                std::cout << std::endl;*/
            }
        }
        if ( (i==randtimes) && (change_check(matrix)==false)) randtimes++;

    /*std::cout << "Random numbers between 0 and 3:" << std::endl;
    for (i = 0; i < randtimes; ++i) {
        std::cout << random_numbers[i] << " ";
    }
    std::cout << std::endl;*/
            for (i=0; i<matrixsize; i++)
    {
        for (j=0; j<matrixsize; j++)
        {
            game_map[i*matrixsize+j] = matrix[i][j];
        }
    }
}

void update_game_map()
{
    for (int i=0; i<matrixsize; i++)
    {
        for (int j=0; j<matrixsize; j++)
        {
            game_map[i*matrixsize+j] = matrix[i][j];
        }
    }
}

void handleKeyPress(SDL_Event &e)
{
    int emptyBlock, emptyX, emptyY;

    switch (e.key.keysym.sym)
    {
    case SDLK_UP:
        emptyBlock = matrixsize * matrixsize - 1;
        emptyX = -1, emptyY = -1;
        for (int i = 0; i < matrixsize; ++i)
        {
            for (int j = 0; j < matrixsize; ++j)
            {
                if (matrix[i][j] == emptyBlock)
                {
                    emptyX = i;
                    emptyY = j;
                    break;
                }
            }
            if (emptyX != -1 && emptyY != -1)
                break;
        }

        if (emptyX < matrixsize-1)
        {
            std::swap(matrix[emptyX][emptyY], matrix[emptyX + 1][emptyY]);
        }
        break;

    case SDLK_DOWN:
        emptyBlock = matrixsize * matrixsize - 1;
        emptyX = -1, emptyY = -1;
        for (int i = 0; i < matrixsize; ++i)
        {
            for (int j = 0; j < matrixsize; ++j)
            {
                if (matrix[i][j] == emptyBlock)
                {
                    emptyX = i;
                    emptyY = j;
                    break;
                }
            }
            if (emptyX != -1 && emptyY != -1)
                break;
        }

        if (emptyX > 0)
        {
            std::swap(matrix[emptyX][emptyY], matrix[emptyX - 1][emptyY]);
        }
        break;

    case SDLK_LEFT:
        emptyBlock = matrixsize * matrixsize - 1;
        emptyX = -1, emptyY = -1;
        for (int i = 0; i < matrixsize; ++i)
        {
            for (int j = 0; j < matrixsize; ++j)
            {
                if (matrix[i][j] == emptyBlock)
                {
                    emptyX = i;
                    emptyY = j;
                    break;
                }
            }
            if (emptyX != -1 && emptyY != -1)
                break;
        }

        if (emptyY < matrixsize - 1)
        {
            std::swap(matrix[emptyX][emptyY], matrix[emptyX][emptyY + 1]);
        }
        break;

    case SDLK_RIGHT:
        emptyBlock = matrixsize * matrixsize - 1;
        emptyX = -1, emptyY = -1;
        for (int i = 0; i < matrixsize; ++i)
        {
            for (int j = 0; j < matrixsize; ++j)
            {
                if (matrix[i][j] == emptyBlock)
                {
                    emptyX = i;
                    emptyY = j;
                    break;
                }
            }
            if (emptyX != -1 && emptyY != -1)
                break;
        }

        if (emptyY > 0)
        {
            std::swap(matrix[emptyX][emptyY], matrix[emptyX][emptyY - 1]);
        }
        break;

    default:
        // Handle other key presses here
        break;
    }



}

void end_game(bool& quit)
{
            SDL_Delay(1000);
            if (changeTexture != NULL) {
                SDL_Rect srcRect = { 0, 0, 500, 500 };
                SDL_Rect destRects = {0, 0, 500, 500};

                SDL_RenderCopy(gRenderer, changeTexture, &srcRect, &destRects);
                SDL_RenderPresent(gRenderer);
                SDL_Delay(4000); // Pause for 3 seconds to display the picture
                SDL_DestroyTexture(changeTexture);
                quit = true; // End the program
            }
}

void mainloop()
{
    bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while( !quit )
			{
			    //Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF ); // vi tri ?
				SDL_RenderClear( gRenderer );




				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					// User presses a key
					else if (e.type == SDL_KEYDOWN)
                    {
                        handleKeyPress(e);
                    }
				}

                //update game_map
                update_game_map();

                //reorganize_textures();


				renderTextures();
				SDL_RenderPresent( gRenderer );

                if (change_check(matrix) == false) { end_game(quit);SDL_RenderPresent( gRenderer );}
				//Update screen

			}
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
		    generate_map();
			//Main loop flag
			mainloop();

		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
