#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

sf::Color HSVToRGB(float h, float s, float v) {
	float hPrime = h / 60.0f;
	unsigned int hIndex = unsigned int(hPrime) % 6;
	float chroma = s * v;
	float min = (v - chroma);
	float x = chroma * (1.0f - abs(fmod(hPrime, 2.0f) - 1.0f));
	float outRGB[6][3] = {

		{chroma, x, 0.0f},
		{x, chroma, 0.0f},
		{0.0f, chroma, x},
		{0.0f, x, chroma},
		{x, 0.0f, chroma},
		{chroma, 0.0f, x}

	};
	float rF = (outRGB[hIndex][0] + min);
	float gF = (outRGB[hIndex][1] + min);
	float bF = (outRGB[hIndex][2] + min);
	rF *= 255;
	gF *= 255;
	bF *= 255;
	std::uint8_t rI = std::uint8_t(rF);
	std::uint8_t gI = std::uint8_t(gF);
	std::uint8_t bI = std::uint8_t(bF);
	return sf::Color(rI, gI, bI);
}
void PollEvents(sf::RenderWindow& window) {
	while (const std::optional event = window.pollEvent()) {
		if (event->is<sf::Event::Closed>()) {
			window.close();
		}
		else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
			if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
				window.close();
			}
		}
	}
}

int main() {
	unsigned int width = 640;
	unsigned int height = 320;
	sf::Vector2u windowSize = { width, height };
	sf::VideoMode videomode = sf::VideoMode(windowSize);
	std::string title = "SFMLL";
	sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({ width, height }), title);
	window.setFramerateLimit(60);

	sf::Texture tex;
	tex.loadFromFile("Textures/HELLO.png");
	if (!tex.loadFromFile("Textures/HELLO.png")) {
		std::cerr << "Critical error NO FILE FOUND AT Textures/\n";
	}
	sf::Sprite background(tex);
	background.setScale({ 1.5f, 1.5f });
	sf::Texture jet;
	jet.loadFromFile("Textures/jet.png");
	if (!jet.loadFromFile("Textures/jet.png")) std::cerr << "Error, no file found at Textures/jet.png\n";
	sf::Sprite jettik(jet);
	jettik.setScale({ 0.15f, 0.15f });
	jettik.setPosition({ 310.0f, 242.0f });

	sf::Texture invTexture;
	if (!invTexture.loadFromFile("Textures/Invader.png")) {
		std::cerr << "Error, no file found at Textures/Invader.png\n";
	}
	sf::Font font;
	if (!font.openFromFile("Fonts/ARIAL.TTF")) std::cerr << "FAILED TO LOAD FONT Fonts/ARIAL.TTF\n";
	sf::Text wintext(font);
	wintext.setString("YOU WIN!");
	wintext.setCharacterSize(40);
	wintext.setFillColor(sf::Color::Red);
	wintext.setOutlineThickness(2.0f);
	wintext.setOutlineColor(sf::Color::White);
	wintext.setPosition({ width / 2.0f, height / 2.0f });

	std::vector<sf::Sprite> invaders;

	const unsigned int Inam = 5;
	const unsigned int rows = 2;
	const float spacing = 10.0f;
	const float startX = 180.0f;
	const float startY = 10.0f;

	for (unsigned int i = 0; i < rows; i++) {
		for (unsigned int j = 0; j < Inam; j++) {
			sf::Sprite inv(invTexture);
			inv.setScale({ 0.1f, 0.1f });

			float x = startX + j * (inv.getGlobalBounds().size.x + spacing);
			float y = startY + i * (inv.getGlobalBounds().size.y + spacing);
			inv.setPosition({ x, y });

			invaders.push_back(inv);
		}
	}
	


	while (window.isOpen()) {


		//Updating

		PollEvents(window);
		bool isWin = invaders.empty();

		sf::Vector2i pos = sf::Mouse::getPosition(window);
		std::cerr << pos.x << "," << pos.y << '\n';

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) {
			jettik.move({ -5.0f, 0.0f });
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) {
			jettik.move({ 5.0f, 0.0f });
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) {
			jettik.move({ 0.0f, -5.0f });
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) {
			jettik.move({ 0.0f, 5.0f });
		}
		if (jettik.getPosition().x < 0) jettik.setPosition({ 0.0f, jettik.getPosition().y });
		if (jettik.getPosition().x > width - jettik.getGlobalBounds().size.x) jettik.setPosition({ width - jettik.getGlobalBounds().size.x, jettik.getPosition().y});
		if (jettik.getPosition().y < 0) jettik.setPosition({ jettik.getPosition().x, 0.0f });
		if (jettik.getPosition().y > height - jettik.getGlobalBounds().size.y) jettik.setPosition({ jettik.getPosition().x, height - jettik.getGlobalBounds().size.y });
		for (unsigned int i = 0; i < invaders.size(); i++) {
			if (invaders[i].getGlobalBounds().findIntersection(jettik.getGlobalBounds())) {
				invaders.erase(invaders.begin() + i);
			}
		}
		
		
		//Render

		window.clear(sf::Color::Black);

		// Draw
		
		window.draw(background);
		window.draw(jettik);
		for (int i = 0; i < invaders.size(); i++) {
			window.draw(invaders[i]);
		}
		if (isWin) {
			window.draw(wintext);
		}

		// Display screen

		window.display();
	}
	return 0;
}