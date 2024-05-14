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
const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;
const int NUM_IMAGES = 25;
const int matrixsize=5;
int randtimes=20;
std::vector<std::vector<int> > matrix(matrixsize,std::vector<int>(matrixsize));
std::vector<int> game_map(matrixsize*matrixsize);
char* imagePaths[NUM_IMAGES];
char* tomMap[NUM_IMAGES] = {
        "itstime/tom00.png", "itstime/tom01.png", "itstime/tom02.png", "itstime/tom03.png", "itstime/tom04.png",
        "itstime/tom10.png", "itstime/tom11.png", "itstime/tom12.png", "itstime/tom13.png", "itstime/tom14.png",
        "itstime/tom20.png", "itstime/tom21.png", "itstime/tom22.png", "itstime/tom23.png", "itstime/tom24.png",
        "itstime/tom30.png", "itstime/tom31.png", "itstime/tom32.png", "itstime/tom33.png", "itstime/tom34.png",
        "itstime/tom40.png", "itstime/tom41.png", "itstime/tom42.png", "itstime/tom43.png", "itstime/44.png",
};
char* jerryMap[NUM_IMAGES] = {
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
int choseTom = 0;
int choseJerry = 1;
int player_chose;
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
SDL_Texture* gBackgroundTexture = NULL;
SDL_Texture* gEasyTexture = NULL;
SDL_Texture* gMediumTexture = NULL;
SDL_Texture* gHardTexture = NULL;
SDL_Texture* gTomTexture = NULL;
SDL_Texture* gJerryTexture = NULL;

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


    gBackgroundTexture = loadTexture("itstime/background.png");
    gEasyTexture = loadTexture("itstime/easy.png");
    gMediumTexture = loadTexture("itstime/medium.png");
    gHardTexture = loadTexture("itstime/hard.png");
    gTomTexture = loadTexture("itstime/tom.png");
    gJerryTexture = loadTexture("itstime/jerry.png");
	return success;
}

void load_map_textures()
{
    if (player_chose == choseTom)
    {
        for (int i=0; i<NUM_IMAGES; i++)
        {
            imagePaths[i] = tomMap[i];
        }
    }
    if (player_chose == choseJerry)
    {
        for (int i=0; i<NUM_IMAGES; i++)
        {
            imagePaths[i] = jerryMap[i];
        }
    }
    for (int i = 0; i < NUM_IMAGES; ++i)
    {
        textures[i] = loadTexture(imagePaths[i]);
    }
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
    SDL_DestroyTexture(gBackgroundTexture);
    SDL_DestroyTexture(gEasyTexture);
    SDL_DestroyTexture(gMediumTexture);
    SDL_DestroyTexture(gHardTexture);


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

void makeEasyMap()
{
    randtimes = 5;
}
void makeMediumMap()
{
    randtimes = 20;
}
void makeHardMap()
{
    randtimes = 100;
}

void displayLevelSelection()
{
    bool levelSelected = false;
    SDL_Event e;
    SDL_Rect easyRect= {100,100,300,100}, mediumRect = {100,200,300,100}, hardRect={100,300,300,100};
    SDL_Point mousePos;

    //SDL_Texture* easyTexture = loadTexture("easy.png");
    //SDL_Texture* mediumTexture = loadTexture("medium.png");
    //SDL_Texture* hardTexture = loadTexture("hard.png");

    SDL_RenderCopy(gRenderer, gBackgroundTexture, NULL, NULL);
        // Render the easy, medium, and hard level option images at specific positions
    SDL_RenderCopy(gRenderer, gEasyTexture, NULL, &easyRect);
    SDL_RenderCopy(gRenderer, gMediumTexture, NULL, &mediumRect);
    SDL_RenderCopy(gRenderer, gHardTexture, NULL, &hardRect);
    SDL_RenderPresent(gRenderer);
    while (!levelSelected)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                close();
                exit(0); // Exit the program if the user closes the window
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                mousePos.x = mouseX;
                mousePos.y = mouseY;
                if (SDL_PointInRect(&mousePos, &easyRect))
                {
                    makeEasyMap();
                    levelSelected = true;
                }
                if (SDL_PointInRect(&mousePos, &mediumRect))
                {
                    makeMediumMap();
                    levelSelected = true;
                }
                if (SDL_PointInRect(&mousePos, &hardRect))
                {
                    makeHardMap();
                    levelSelected = true;
                }

            }
        }

    }
    // Free level selection textures
    //SDL_DestroyTexture(gBackgroundTexture);
    SDL_DestroyTexture(gEasyTexture);
    SDL_DestroyTexture(gMediumTexture);
    SDL_DestroyTexture(gHardTexture);
}

void displayImageSelection()
{
    bool imageSelected = false;
    SDL_Event e;
    SDL_Rect tomRect= {100,100,300,150}, jerryRect = {100,250,300,150};
    SDL_Point mousePos;

    //SDL_Texture* easyTexture = loadTexture("easy.png");
    //SDL_Texture* mediumTexture = loadTexture("medium.png");
    //SDL_Texture* hardTexture = loadTexture("hard.png");

    SDL_RenderCopy(gRenderer, gBackgroundTexture, NULL, NULL);
        // Render the easy, medium, and hard level option images at specific positions
    SDL_RenderCopy(gRenderer, gTomTexture, NULL, &tomRect);
    SDL_RenderCopy(gRenderer, gJerryTexture, NULL, &jerryRect);

    SDL_RenderPresent(gRenderer);
    while (!imageSelected)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                close();
                exit(0); // Exit the program if the user closes the window
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                mousePos.x = mouseX;
                mousePos.y = mouseY;
                if (SDL_PointInRect(&mousePos, &tomRect))
                {
                    player_chose = choseTom;
                    imageSelected = true;
                    load_map_textures();
                }
                if (SDL_PointInRect(&mousePos, &jerryRect))
                {
                    player_chose = choseJerry;
                    imageSelected = true;
                    load_map_textures();
                }

            }
        }

    }
    // Free level selection textures
    SDL_DestroyTexture(gBackgroundTexture);
    SDL_DestroyTexture(gTomTexture);
    SDL_DestroyTexture(gJerryTexture);

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
		    displayLevelSelection();
		    displayImageSelection();
		    generate_map();
			//Main loop flag
			mainloop();

		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
