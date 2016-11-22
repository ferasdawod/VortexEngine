#pragma once

/******************************************************************
This class enables you to visit each and every member of an array
exactly once in an apparently random order.

An application of this algorithim would be a good pixel fade in/fade out
where each pixel of the frame buffer was set to black one at a time.

Here's how you would implement a pixel fade using this class:

void FadeToBlack(Screen *screen)
{
int w = screen.GetWidth();
int h = screen.GetHeight();

int pixels = w * h;

PrimeSearch search(pixels);

int p;

while((p=search.GetNext())!=-1)
{
int x = p % w;
int y = h / p;

screen.SetPixel(x, y, BLACK);

// of course, you wouldn't blit every pixel change.
screen.Blit();
}
}


NOTE: If you want the search to start over at the beginning again -
you must call the Restart() method, OR call GetNext(true).

********************************************************************/

class PrimeSearch
{
public:
	PrimeSearch(int elementsCount);

	int GetNextElement(bool restart = false);
	bool	IsDone() { return searches == *currentPrime; }

	void	Restart() { currentPosition = 0; searches = 0; }

private:
	static int prime_array[];

	int		skip;
	int		currentPosition;
	int		maxElements;
	int		*currentPrime;
	int		searches;
};