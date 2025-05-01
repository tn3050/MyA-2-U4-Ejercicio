#include "Game.h"
#include "Box2DHelper.h"

// Constructor de la clase Game
Game::Game(int ancho, int alto, std::string titulo)
{
	wnd = new RenderWindow(VideoMode(ancho, alto), titulo); // Crea una ventana con las dimensiones y título especificados
	wnd->setVisible(true); // Hace visible la ventana
	fps = 60; // Establece el límite de cuadros por segundo (FPS)
	wnd->setFramerateLimit(fps); // Aplica el límite de FPS a la ventana
	frameTime = 1.0f / fps; // Calcula el tiempo por cuadro en segundos
	SetZoom(); // Configura el "zoom" o vista de la cámara
	InitPhysics(); // Inicializa la simulación de física
}

void Game::CreateRagdoll(b2Vec2 base) {
	const float SCALE = 1.0f / 3.0f;
	Ragdoll doll;

	doll.cabeza = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 7.0f * SCALE, 7.0f * SCALE, 1.0f, 0.1f, 1.0f);
	doll.cabeza->SetTransform(base + b2Vec2(0.0f, -10.0f * SCALE), 0.0f);

	doll.pecho = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 5.0f * SCALE, 10.0f * SCALE, 1.0f, 0.1f, 1.0f);
	doll.pecho->SetTransform(base + b2Vec2(0.0f, 0.0f), 0.0f);

	Box2DHelper::CreateDistanceJoint(phyWorld, doll.cabeza, doll.cabeza->GetWorldCenter() + b2Vec2(0.0f, 3.4f * SCALE),
		doll.pecho, doll.pecho->GetWorldCenter() + b2Vec2(0.0f, -5.0f * SCALE), 20.0f, 0.1f, 0.0f);

	doll.piernaIzq = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 2.5f * SCALE, 7.5f * SCALE, 1.0f, 0.1f, 1.0f);
	doll.piernaIzq->SetTransform(base + b2Vec2(-1.5f * SCALE, 10.0f * SCALE), 0.0f);

	doll.piernaDer = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 2.5f * SCALE, 7.5f * SCALE, 1.0f, 0.1f, 1.0f);
	doll.piernaDer->SetTransform(base + b2Vec2(1.5f * SCALE, 10.0f * SCALE), 0.0f);

	Box2DHelper::CreateDistanceJoint(phyWorld, doll.pecho, doll.pecho->GetWorldCenter() + b2Vec2(-2.0f * SCALE, 4.9f * SCALE),
		doll.piernaIzq, doll.piernaIzq->GetWorldCenter() + b2Vec2(0.0f, -3.25f * SCALE), 20.0f, 0.1f, 0.0f);

	Box2DHelper::CreateDistanceJoint(phyWorld, doll.pecho, doll.pecho->GetWorldCenter() + b2Vec2(2.0f * SCALE, 4.9f * SCALE),
		doll.piernaDer, doll.piernaDer->GetWorldCenter() + b2Vec2(0.0f, -3.25f * SCALE), 20.0f, 0.1f, 0.0f);

	doll.manoIzq = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 2.5f * SCALE, 7.0f * SCALE, 1.0f, 0.1f, 1.0f);
	doll.manoIzq->SetTransform(base + b2Vec2(-4.0f * SCALE, -1.0f * SCALE), 0.0f);

	doll.manoDer = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 2.5f * SCALE, 7.0f * SCALE, 1.0f, 0.1f, 1.0f);
	doll.manoDer->SetTransform(base + b2Vec2(4.0f * SCALE, -1.0f * SCALE), 0.0f);

	Box2DHelper::CreateDistanceJoint(phyWorld, doll.pecho, doll.pecho->GetWorldCenter() + b2Vec2(-2.4f * SCALE, -4.9f * SCALE),
		doll.manoIzq, doll.manoIzq->GetWorldCenter() + b2Vec2(1.24f * SCALE, -3.4f * SCALE), 20.0f, 0.1f, 0.0f);

	Box2DHelper::CreateDistanceJoint(phyWorld, doll.pecho, doll.pecho->GetWorldCenter() + b2Vec2(2.4f * SCALE, -4.9f * SCALE),
		doll.manoDer, doll.manoDer->GetWorldCenter() + b2Vec2(-1.24f * SCALE, -3.4f * SCALE), 20.0f, 0.1f, 0.0f);

	ragdolls.push_back(doll);
}

void Game::Loop()
{
	while (wnd->isOpen()) // Bucle principal del juego que se ejecuta mientras la ventana esté abierta
	{
		wnd->clear(clearColor); // Limpia la ventana con el color de fondo
		DoEvents(); // Maneja los eventos (input del usuario)
		CheckCollitions(); // Verifica colisiones (a implementar)
		UpdatePhysics(); // Actualiza la simulación de física
		DrawGame(); // Dibuja los elementos del juego
		wnd->display(); // Muestra los cambios en la ventana
	}
}

void Game::UpdatePhysics()
{
	phyWorld->Step(frameTime, 8, 8); // Avanza la simulación de física un paso
	phyWorld->ClearForces(); // Limpia las fuerzas acumuladas
	phyWorld->DebugDraw(); // Dibuja la representación de debug de la simulación
}

void Game::DrawGame() {

	Vector2f mousePos = wnd->mapPixelToCoords(Mouse::getPosition(*wnd));


	Vector2f dir(mousePos.x - cannonBase.x, mousePos.y - cannonBase.y);


	float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
	if (length > 0.0f) {
		dir.x /= length;
		dir.y /= length;
	}


	float cannonLength = 20.0f;


	Vertex line[] = {
		Vertex(Vector2f(cannonBase.x, cannonBase.y), Color::Red),
		Vertex(Vector2f(cannonBase.x + dir.x * cannonLength, cannonBase.y + dir.y * cannonLength), Color::Yellow)
	};


	wnd->draw(line, 2, Lines);


	RectangleShape cannonShape(Vector2f(15.0f, 5.0f)); 


	cannonShape.setOrigin(15.0f / 2.0f, 2.5f); 

	cannonShape.setPosition(Vector2f(cannonBase.x, cannonBase.y));


	float angle = std::atan2(dir.y, dir.x) * 180.0f / 3.14159265f; 
	cannonShape.setRotation(angle);

	cannonShape.setFillColor(Color::Yellow);


	wnd->draw(cannonShape);
}




void Game::DoEvents() {
	Event evt;
	while (wnd->pollEvent(evt)) {
		switch (evt.type) {
		case Event::Closed:
			wnd->close();
			break;
		case Event::MouseButtonPressed:
			if (evt.mouseButton.button == Mouse::Left) {
				Vector2f mousePosF = wnd->mapPixelToCoords(Vector2i(evt.mouseButton.x, evt.mouseButton.y));
				b2Vec2 mousePos(mousePosF.x, mousePosF.y);


				b2Vec2 direction = mousePos - cannonBase;
				float distance = direction.Length();

				if (distance > 0.0f) {
					direction.Normalize(); // Convertimos a dirección unitaria

					// Limitar la potencia
					float maxPower = 1650.0f; 
					float power = std::min(distance * 12.0f, maxPower);

					// Crear un nuevo ragdoll
					CreateRagdoll(cannonBase);

					// Aplicar fuerza inicial 
					if (!ragdolls.empty()) {
						b2Body* cuerpo = ragdolls.back().cabeza;

						// Aplicar impulso
						cuerpo->ApplyLinearImpulseToCenter(power * direction, true);
					}
				}
			}
			break;
		}
	}
}




void Game::CheckCollitions()
{
	// Verificación de colisiones (a implementar)
}

void Game::SetZoom()
{
	View camara;
	camara.setSize(100.0f, 100.0f); // Establece el tamaño de la vista
	camara.setCenter(50.0f, 50.0f); // Centra la vista en un punto del mundo
	wnd->setView(camara); // Aplica la vista a la ventana
}

void Game::InitPhysics()
{
	const float SCALE = 1.0f / 3.0f;
	// Inicializa el mundo de Box2D con una gravedad hacia abajo
	phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

	debugRender = new SFMLRenderer(wnd); // Crea un renderizador de debug para SFML
	debugRender->SetFlags(UINT_MAX); // Configura el renderizador para dibujar todas las formas de debug
	phyWorld->SetDebugDraw(debugRender); // Establece el renderizador de debug para el mundo de Box2D

	// Crea cuerpos estáticos para simular el suelo y las paredes
	// También crea cuerpos dinámicos (como círculos) y los une con resortes al techo para demostrar la simulación física
	b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

	b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

	b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	rightWallBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);

	// Creamos un techo
	b2Body* topWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	topWallBody->SetTransform(b2Vec2(50.0f, 0.0f), 0.0f);

	//Objetos estatico
	b2Body* Plataforma = Box2DHelper::CreateRectangularStaticBody(phyWorld, 30, 5);
	Plataforma->SetTransform(b2Vec2(65.0f, 60.0f), 0.0f);
	b2Body* Plataforma2 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 5, 5);
	Plataforma2->SetTransform(b2Vec2(80.0f, 30.0f), 0.0f);

	//Objeto dinamicos
	b2Body* Barril = Box2DHelper::CreateRectangularDynamicBody(phyWorld,4.5f, 8.0f,1.0f,0.3f,0.2f);
	Barril->SetTransform(b2Vec2(55.0f, 60.0f), 0.0f);
	b2Body* Barril2 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 4.5f, 8.0f, 1.0f, 0.3f, 0.2f);
	Barril2->SetTransform(b2Vec2(63.0f, 60.0f), 0.0f);
	b2Body* Barril3 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 4.5f, 8.0f, 1.0f, 0.3f, 0.2f);
	Barril3->SetTransform(b2Vec2(59.0f, 45.0f), 0.0f);
	b2Body* Barril4 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 4.5f, 8.0f, 1.0f, 0.3f, 0.2f);
	Barril4->SetTransform(b2Vec2(80.0f, 25.0f), 0.0f);
    b2Body* Pelota1 = Box2DHelper::CreateCircularDynamicBody(phyWorld, 3.0f, 1.0f, 0.3f, 0.6f);
	Pelota1->SetTransform(b2Vec2(80.0f, 17.0f), 0.0f);
	b2Body* Barril5 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 4.5f, 8.0f, 1.0f, 0.3f, 0.2f);
	Barril5->SetTransform(b2Vec2(55.0f, 92.0f), 0.0f);
	b2Body* Barril6 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 4.5f, 8.0f, 1.0f, 0.3f, 0.2f);
	Barril6->SetTransform(b2Vec2(70.0f, 92.0f), 0.0f);
	b2Body* Estante = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 18.5f, 2.0f, 1.0f, 0.3f, 0.2f);
	Estante->SetTransform(b2Vec2(62.5f, 87.5f), 0.0f);
	b2Body* Barril7 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 4.5f, 8.0f, 1.0f, 0.3f, 0.2f);
	Barril7->SetTransform(b2Vec2(62.5f, 84.9f), 0.0f);
	
}

Game::~Game(void)
{
	// Destructor de la clase Game (a implementar si es necesario)
}