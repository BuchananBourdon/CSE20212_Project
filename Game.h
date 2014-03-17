#ifndef GAME_H
#define GAME_H

class Game {
public:
	Game();
	~Game();

	void play(); // Game loop

private:
	bool playing; // Quit game loop?

	void handleEvents(); // Process a chunk of events
};

#endif

