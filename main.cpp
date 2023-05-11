/*
Author:Supriya Sundar
Class:ECE 6122
Last Date Modified: 10/27/2021
Description: Implementation of buzzy's revenge game was executed.
Reason for Errors: I had a biopsy the previous week which has affected my health,which can be verified from stamps health services as well
and thus I was short on time to execute this and other course assignments as well. 
Please consider grading on the basis of my logic as I have attempted to cover everything accurately except for the projectile motion of Buzzy 
which I couldn't get to work despite my best efforts. My previous labs weren't good as well and my GPA will be direly affected. 
Kindly excuse me just this once and consider my logic.
*/

#include <sstream>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
using namespace sf;
int main()
{
	VideoMode vm(1920, 1080); //setting up background
	RenderWindow window(vm, "Buzzy's Revenge", Style::Fullscreen);
	Texture textureBackground;
	textureBackground.loadFromFile("graphics/background.png");
	Sprite spriteBackground;
	spriteBackground.setTexture(textureBackground);
	spriteBackground.setPosition(0, 0);

	Texture textureBee; //loading small bee insect
	textureBee.loadFromFile("graphics/insect.png");
	Sprite spriteBee;
	spriteBee.setScale(-1, 1);
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(1650, 1080/3.0f);

	Texture textureBuzzy; // loading buzzy
	textureBuzzy.loadFromFile("graphics/smallbuzzy.png");
	Sprite spriteBuzzy;
	spriteBuzzy.setTexture(textureBuzzy);
	spriteBuzzy.setPosition(80, 1080 / 1.5f);

	bool acceptInput = false; // flag for input
	bool paused = true; //flag for game activity
	int score = 0; // constant score
	int pi = 3.14; 
	bool reset = false; //flag to check if game has to move to next level
	bool shuffle = false; // bool to check if animals have been shuffled which would indicate next level

	bool beeActive = false; //flag to check bee activity
	float beeSpeed = 80.0f; //bee's intial speed
	
	float buzzySpeed = 80.0f; // buzzy's initial speed
	float buzzyMaxSpeed = 150.0f; //max limit for buzzy's speed
	float buzzyMinSpeed = 80.0f; // min limit for buzzy's speed
	float buzzyVelocityX = 0.0f; // buzzy's initial speed horizontally
	float buzzyVelocityY = 0.0f; // buzzy's initial speed vertically
	float acceleration =-9.8; //acceleration value
	float buzzyCurrentVelocityX = 0.0f; // buzzy's current speed horizontally
	float buzzyCurrentVelocityY = 0.0f; //// buzzy's current speed vertically
	float buzzyNewVelocityX = 0.0f; //current X location
	float buzzyNewVelocityY = 0.0f;// current Y location
	float buzzyTime = 0.0f;  // buzzy time
	bool buzzyMoves = false; //flag to check buzzy activity

	bool tigerHit = false; // flag to check if animals have been hit
	bool bullDogHit = false;// flag to check if animals have been hit
	bool tigerDraw = false;// flag to check if animals need to been redrawn
	bool bullDogDraw = false;// flag to check if animals need to been redrawn
	bool otherAnimalsDraw = true;// flag to check if animals need to been redrawn
	bool otherAnimalsCollision = false;// flag to check if animals have been hit

	SoundBuffer chopBuffer; //setting up sound
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);

	SoundBuffer deathBuffer;//setting up sound
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	SoundBuffer timeBuffer;//setting up sound
	timeBuffer.loadFromFile("sound/out_of_time.wav");
	Sound timeSound;
	timeSound.setBuffer(timeBuffer);

	//variables to control time
	Clock clock;
	//power bar
	RectangleShape powerBar;
	float powerBarStartWidth = 400;
	float powerBarHeight = 80;
	powerBar.setSize(Vector2f(powerBarStartWidth, powerBarHeight));
	powerBar.setFillColor(Color::Red);
	powerBar.setPosition(200, 900);

	//variables to monitor time remaining
	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float powerBarWidthPerSecond = powerBarStartWidth / timeRemaining;

	Time dt = clock.restart(); //measure time

	sf::Text titleText; //text for various textual data to be displayed on screen
	sf::Text livesText;
	sf::Text messageText;
	sf::Text scoreText;
	sf::Text powerText;
	sf::Text authorText;


	sf::Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf"); //loading font file

	FloatRect textRect = titleText.getLocalBounds();//positioning text
	titleText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	titleText.setPosition(1920 / 4.0f, 1080 / 5.0f);
	FloatRect textRect1 = messageText.getLocalBounds();//positioning text
	messageText.setOrigin(textRect1.left + textRect1.width / 2.0f, textRect1.top + textRect1.height / 2.0f);
	messageText.setPosition(1920 / 3.0f, 1080 / 3.0f);
	FloatRect textRect2 = authorText.getLocalBounds();//positioning text
	authorText.setOrigin(textRect2.left + textRect2.width / 2.0f, textRect2.top + textRect2.height / 2.0f);
	authorText.setPosition(1920 / 2.5f, 1080 / 2.0f);
	livesText.setPosition(0, 0);
	scoreText.setPosition(1700, 0);
	powerText.setPosition(40, 900);

	titleText.setFont(font);//font file loaded is applied to text
	livesText.setFont(font);
	messageText.setFont(font);
	scoreText.setFont(font);
	powerText.setFont(font);
	authorText.setFont(font);

	titleText.setString("Buzzy's Revenge"); // typing actual text 
	livesText.setString("Lives");
	messageText.setString("Press Enter to Restart Game\n Press Esc to exit\n Press Space to Powerup\n Press up / down arrow to aim");
	scoreText.setString("Score:");
	powerText.setString("Power");
	authorText.setString("created by\n Supriya Sundar");

	titleText.setCharacterSize(100);
	livesText.setCharacterSize(36);//setting text size
	messageText.setCharacterSize(36);
	scoreText.setCharacterSize(36);
	powerText.setCharacterSize(36);
	authorText.setCharacterSize(36);


	titleText.setFillColor(Color::Red);
	livesText.setFillColor(Color::White);//setting text color
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);
	powerText.setFillColor(Color::White);
	authorText.setFillColor(Color::White);

	//commented out the randomiser and removed from animals owing to compilation error that I was unable to resolve

	/*std::vector < vector<int>> initial_values = // used vector of vectors to store positions of animals
	{
		{1530,1000},
		{1530,850},
		{1530,700},
		{1530,550},
		{1530,400},
		{1730,1000},
		{1730,850},
		{1730,700},
		{1730,550},
		{1730,400}
	};

	std::vector < vector<int>> values =
	{
		{1530,1000},
		{1530,850},
		{1530,700},
		{1530,550},
		{1530,400},
		{1730,1000},
		{1730,850},
		{1730,700},
		{1730,550},
		{1730,400}
	};


		void randomisingPosition()
	{
		std::random_shuffle(values.begin(), values.end()); // randomising animal positions

	}*/

	Sprite sprites[11]; //creating array of sprites for all the animals

	Texture textureSheep; //making sprite for sheep
	textureSheep.loadFromFile("graphics/sheep.png");
	sprites[0].setTexture(textureSheep);
	FloatRect imgRect = sprites[0].getLocalBounds();
	sprites[0].setOrigin(imgRect.left + imgRect.width / 2.0f, imgRect.top + imgRect.height / 2.0f);
	sprites[0].setPosition(1530, 1000);

	Texture textureChicken; //making sprite for chicken
	textureChicken.loadFromFile("graphics/chicken.png");
	sprites[1].setTexture(textureChicken);
	FloatRect imgRect1 = sprites[1].getLocalBounds();
	sprites[1].setOrigin(imgRect1.left + imgRect1.width / 2.0f, imgRect1.top + imgRect1.height / 2.0f);
	sprites[1].setPosition(1530, 850);
	
	Texture textureDog; //making sprite for dog
	textureDog.loadFromFile("graphics/dog.png");
	sprites[2].setTexture(textureDog);
	FloatRect imgRect2 = sprites[2].getLocalBounds();
	sprites[2].setOrigin(imgRect2.left + imgRect2.width / 2.0f, imgRect2.top + imgRect2.height / 2.0f);
	sprites[2].setPosition(1530, 700);

	Texture textureTiger;//making sprite for tiger
	textureTiger.loadFromFile("graphics/tiger.png");
	sprites[3].setTexture(textureTiger);
	sprites[3].scale(0.1f, 0.1f);
	FloatRect imgRect3 = sprites[3].getLocalBounds();
	sprites[3].setOrigin(imgRect3.left + imgRect3.width / 2.0f, imgRect3.top + imgRect3.height / 2.0f);
	sprites[3].setPosition(1530, 550);

	Texture textureBunny;//making sprite for bunny
	textureBunny.loadFromFile("graphics/bunny.png");
	sprites[4].setTexture(textureBunny);
	FloatRect imgRect4 = sprites[4].getLocalBounds();
	sprites[4].setOrigin(imgRect4.left + imgRect4.width / 2.0f, imgRect4.top + imgRect4.height / 2.0f);
	sprites[4].setPosition(1530, 400);

	Texture textureMouse;//making sprite for mouse
	textureMouse.loadFromFile("graphics/mouse.png");
	sprites[5].setTexture(textureMouse);
	FloatRect imgRect5 = sprites[5].getLocalBounds();
	sprites[5].setOrigin(imgRect5.left + imgRect5.width / 2.0f, imgRect5.top + imgRect5.height / 2.0f);
	sprites[5].setPosition(1730, 1000);

	Texture texturePig;//making sprite for pig
	texturePig.loadFromFile("graphics/pig.png");
	sprites[6].setTexture(texturePig);
	FloatRect imgRect6 = sprites[6].getLocalBounds();
	sprites[6].setOrigin(imgRect6.left + imgRect6.width / 2.0f, imgRect6.top + imgRect6.height / 2.0f);
	sprites[6].setPosition(1730, 850);

	Texture textureFrog; //making sprite for frog
	textureFrog.loadFromFile("graphics/frog.png");
	sprites[7].setTexture(textureFrog);
	FloatRect imgRect7 = sprites[7].getLocalBounds();
	sprites[7].setOrigin(imgRect7.left + imgRect7.width / 2.0f, imgRect7.top + imgRect7.height / 2.0f);
	sprites[7].setPosition(1730, 700);

	Texture textureUnicorn; //making sprite for unicorn
	textureUnicorn.loadFromFile("graphics/angry_unicorn.png");
	sprites[8].setTexture(textureUnicorn);
	FloatRect imgRect8 = sprites[8].getLocalBounds();
	sprites[8].setOrigin(imgRect8.left + imgRect8.width / 2.0f, imgRect8.top + imgRect8.height / 2.0f);
	sprites[8].setPosition(1730, 550);

	Texture textureBullDog;//making sprite for Georgia Bulldog
	textureBullDog.loadFromFile("graphics/Georgia_Bulldogs_logo_dog_g.png");
	sprites[9].setTexture(textureBullDog);
	FloatRect imgRect9 = sprites[9].getLocalBounds();
	sprites[9].setOrigin(imgRect9.left + imgRect9.width / 2.0f, imgRect9.top + imgRect9.height / 2.0f);
	sprites[9].setPosition(1730, 400);

	Texture textureBuzzLife;//making sprite for buzzy lives at top left corner
	textureBuzzLife.loadFromFile("graphics/buzzy life.png");
	Sprite buzzLife[6];
	for (int i = 0; i < 5; i++) //creating 5 lives
	{
		buzzLife[i].setTexture(textureBuzzLife);
		buzzLife[i].setPosition(95 + 65 * i, 20);
		buzzLife[i].scale(sf::Vector2f(0.5f, 0.5f));
	}

	int lifeCount = 5; // constant to monitor buzzy lives lost and incremented

	Sprite otherAnimalsColumnOne[] = { sprites[0],sprites[1],sprites[2],sprites[4] }; // array of animals except evil mascots in column 1
	Sprite otherAnimalsColumnTwo[] = { sprites[5],sprites[6],sprites[7],sprites[8] }; // array of animals except evil mascots in column 2
	Sprite evilAnimals[] = { sprites[3],sprites[9] };//array of evil mascots

	while (window.isOpen())
	{
		
		sf::Event event;
		while (window.pollEvent(event))
		{
			
			if (event.type == sf::Event::Closed)
				window.close();
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape))//exiting game
		{
			window.close();
		}
		if (event.type == Event::KeyReleased && !paused)
		{
			acceptInput = true;//checking for keypresses/inputs from user
		}
		//starting game
		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			spriteBuzzy.setPosition(250.f, 550.f);//intialising buzzy values once game has started
			spriteBuzzy.setRotation(300.f);
			buzzyTime = 0.f;
			buzzyMoves = false;
			buzzySpeed = buzzyMinSpeed;
			acceptInput = true;
			powerBar.setSize(Vector2f(0.0f, powerBarHeight)); //decreasing time of game once it has started
			titleText.setPosition(5000.f,1080/4.5f);//making textual data disappear once game has started
			messageText.setPosition(5500.f,1080/4.5f);
			authorText.setPosition(6000.f,1080/4.5f);
		}

		if (!paused) //when game is not paused
		{
			Time dt = clock.restart();

			// Subtract from the amount of time remaining
			timeRemaining -= dt.asSeconds();
			// size up the time bar
			powerBar.setSize(Vector2f(powerBarWidthPerSecond * timeRemaining, powerBarHeight));
			if (timeRemaining <= 0.0f)
			{
				// Pause the game
				paused = true;
				messageText.setString("Out of time!!");

				//Reposition the text based on its new size
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left +textRect.width / 2.0f,textRect.top +textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				// Play the out of time sound
				timeSound.play();
			}
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) //setting booleans for game starts,ie., when enter is pressed
		{
			paused = false;
			acceptInput = true;
			std::stringstream ss;
			ss << "Score = " << score;//updating integer score value to score text
			scoreText.setString(ss.str());
		}
		
		if (acceptInput)//when user input exists
		{
			if (!beeActive)
			{

				// How fast is the bee
				srand((int) time(0) * 10);
				beeSpeed = (rand()%10 + 100);
				// How high is the bee
				srand((int) time(0) * 10000);
				float height = (rand() % 400);
				spriteBee.setPosition(1800, height);
				//spriteBee.setPosition(500, 500);
				beeActive = true;
			}
			else
				// Move the bee
			{
				sf::Time dt = clock.getElapsedTime();
				spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);// change y 
				clock.restart();
				// Has the bee reached the right hand edge of the screen?
				if (spriteBee.getPosition().x < -10)
				{
					// Set it up ready to be a whole new cloud next frame
					beeActive = false;
				}
			}
			if (Keyboard::isKeyPressed(Keyboard::Space))
			{
				if (buzzySpeed != buzzyMaxSpeed) //when buzzy has just started moving with initial speed
				{
					buzzySpeed += 1;
					powerBar.setSize(Vector2f(powerBarWidthPerSecond*timeRemaining, powerBarHeight));//incrementing power bar value when space is pressed
					timeRemaining += 0.2;
				}
				else if (buzzySpeed == buzzyMaxSpeed) //when buzzy has reached max speed limit
				{
					buzzyMoves = true;
					acceptInput = false;
					float theta = (spriteBuzzy.getRotation() * (pi / 180));
					buzzyVelocityX = buzzySpeed * (cos(theta));
					buzzyVelocityY = buzzySpeed * (-sin(theta));
				}
			}
			if (Keyboard::isKeyPressed(Keyboard::Up))
			{
					spriteBuzzy.setRotation(50.0f);//increaing angle
			}
			if (Keyboard::isKeyPressed(Keyboard::Down))
			{
					spriteBuzzy.setRotation(-40.0f);//decreasing angle
			}
			if (event.type==sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::Space)
				{
					buzzyMoves = true;
					acceptInput = false;
					float theta = (spriteBuzzy.getRotation() * (pi / 180));//getting buzzy's angle of rotation
					buzzyVelocityX = buzzySpeed * (cos(theta));//auxillary equations
					buzzyVelocityY = buzzySpeed * (-sin(theta));//auxillary equations
					buzzyTime += dt.asSeconds();
					buzzyCurrentVelocityX = buzzyVelocityX;
					buzzyCurrentVelocityY = buzzyVelocityY + acceleration * buzzyTime;
					buzzyNewVelocityX = buzzyVelocityX * buzzyTime;//horizontal direction projectile motion equation
					buzzyNewVelocityY = (buzzyVelocityY * buzzyTime) + ((acceleration  *100* buzzyTime * buzzyTime) / 2);//vertical direction projectile motion equation
					spriteBuzzy.setPosition(buzzyNewVelocityX, -buzzyNewVelocityY);
					spriteBuzzy.setRotation(atan(buzzyVelocityY + acceleration  * 100*buzzyTime / buzzyVelocityX)* (pi / 180)); //implementing (atan(vy+gt)/vx)*pi/180))
				}
			}
		}

		if (spriteBuzzy.getGlobalBounds().intersects(sprites[3].getGlobalBounds())) // when buzzy collides with tiger
		{
			score += 25;//increase score by 25 points
			for (int i = 0; i < 5; i++)
			{
				otherAnimalsColumnOne[i].setColor(Color::Transparent);//making all other animals in column with tiger disappear
				death.play();//playing death sound
			}
			otherAnimalsCollision = false;
		}

		if (spriteBuzzy.getGlobalBounds().intersects(sprites[9].getGlobalBounds()))// when buzzy collides with Georgia bulldog
		{
			score += 25;//increase score by 25 points
			for (int i = 0; i < 5; i++)
			{
				otherAnimalsColumnTwo[i].setColor(Color::Transparent);//making all other animals in column with Georgia bull dog disappear
				death.play();//playing death sound
			}
			otherAnimalsCollision = false;
		}

		if (spriteBuzzy.getGlobalBounds().intersects(spriteBee.getGlobalBounds()))// when buzzy collides with bee insect
		{
			score += 75;//increase score by 75 points
			while (reset == false)//when playing in same level
			{
				spriteBee.setColor(Color::Transparent);//make bee disappear
				death.play();
			}
			otherAnimalsCollision = false;
		}

		if (!(spriteBuzzy.getGlobalBounds().intersects(spriteBee.getGlobalBounds()) || spriteBuzzy.getGlobalBounds().intersects(sprites[3].getGlobalBounds()) || spriteBuzzy.getGlobalBounds().intersects(sprites[9].getGlobalBounds()) || spriteBuzzy.getGlobalBounds().intersects(sprites[8].getGlobalBounds())))
		{
			lifeCount -= 1; // when bee doesn't hit evil mascot or bee or unicorn, decrease buzzy's life
			otherAnimalsCollision = true;
		}

		if (spriteBuzzy.getGlobalBounds().intersects(sprites[8].getGlobalBounds()))// when buzzy hits unicorn
		{
			sprites[8].setColor(Color::Transparent);//make unicorn disappear
			otherAnimalsCollision = true;
			if (lifeCount < 5)
			{
				lifeCount += 1;//increase life if it is below 5
				buzzLife[0].setColor(Color::Transparent);//make a life buzzy symbol disappear
			}
		}
		 
		if (spriteBuzzy.getGlobalBounds().intersects(sprites[3].getGlobalBounds()))//when buzzy hits tiger
		{
			score += 25;
			for (int i = 0; i < 5; i++)
			{
				otherAnimalsColumnOne[i].setColor(Color::Transparent);
			}
			otherAnimalsCollision = false;
			if (spriteBuzzy.getGlobalBounds().intersects(sprites[9].getGlobalBounds()))// when buzzy hits Georgia bull dog after having hit tiger
			{
				reset = true; //reset the game or move to next level
			}
		}

		if (spriteBuzzy.getGlobalBounds().intersects(sprites[9].getGlobalBounds()))//when buzzy hits bulldog
		{
			score += 25;
			for (int i = 0; i < 5; i++)
			{
				otherAnimalsColumnOne[i].setColor(Color::Transparent);
			}
			otherAnimalsCollision = false;
			if (spriteBuzzy.getGlobalBounds().intersects(sprites[3].getGlobalBounds()))//when buzzy hits tiger after having hit bulldog
			{
				reset = true;//reset the game or move to next level
			}
		}

		if (otherAnimalsCollision == true)
		{
			reset = true;//reset game if other woodland creatures are hit
		}
		for (int i=0; i < 10; i++)
		{
			if (otherAnimalsCollision == false && (!(spriteBuzzy.getGlobalBounds().intersects(sprites[i].getGlobalBounds()))))
			{
				reset = true;//reset game if nothing is hit
			}
		}
		
		window.clear();//clearing window to display stuff anew
		window.draw(spriteBackground); // draw all the entitties used above on the window
		window.draw(titleText);
		window.draw(livesText);
		window.draw(messageText);
		window.draw(scoreText);
		window.draw(powerText);
		window.draw(authorText);
		window.draw(powerBar);
		window.draw(spriteBuzzy);
		window.draw(sprites[0]);
		window.draw(sprites[1]);
		window.draw(sprites[2]);
		window.draw(sprites[3]);
		window.draw(sprites[4]);
		window.draw(sprites[5]);
		window.draw(sprites[6]);
		window.draw(sprites[7]);
		window.draw(sprites[8]);
		window.draw(sprites[9]);
		for (int i = 0; i < 6; i++)
		{
			window.draw(buzzLife[i]);
		}
		window.draw(spriteBee);
		window.display(); //displaying all stuff drawn
	}

	return 0;
}


