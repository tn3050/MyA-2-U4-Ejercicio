#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "SFMLRenderer.h"
#include <list>

using namespace sf;

struct Ragdoll {
	b2Body* cabeza;
	b2Body* pecho;
	b2Body* piernaIzq;
	b2Body* piernaDer;
	b2Body* manoIzq;
	b2Body* manoDer;
};
class Game
{
private:
	// Propiedades de la ventana
	int alto;
	int ancho;
	RenderWindow* wnd;
	Color clearColor;

	// Objetos de box2d
	b2World* phyWorld;
	SFMLRenderer* debugRender;

	// Tiempo de frame
	float frameTime;
	int fps;
	std::vector<Ragdoll> ragdolls;

public:
	//Constructores, destructores e inicializadores
	Game(int ancho, int alto, std::string titulo);
	//9.0f, 90.0f
	b2Vec2 cannonBase = b2Vec2(9.0f, 90.0f);
	void CheckCollitions();
	void CreateEnemy(int x, int y);
	~Game(void);
	void InitPhysics();
	void CreateRagdoll(b2Vec2 base);

	// Main game loop
	void Loop();
	void DrawGame();
	void UpdatePhysics();
	void DoEvents();
	void SetZoom();

};