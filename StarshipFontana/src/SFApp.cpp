#include "SFApp.h"
int j = 0;
SFApp::SFApp(std::shared_ptr<SFWindow> window) : fire(0), is_running(true), sf_window(window) {
	int canvas_w, canvas_h;
	SDL_GetRendererOutputSize(sf_window->getRenderer(), &canvas_w, &canvas_h);

	app_box = make_shared<SFBoundingBox>(Vector2(canvas_w, canvas_h), canvas_w, canvas_h);
	player = make_shared<SFAsset>(SFASSET_PLAYER, sf_window);
	auto player_pos = Point2(canvas_w / 2, 22);
	player->SetPosition(player_pos);

	const int number_of_aliens = 3;
	
	for (int i = 0; i < number_of_aliens; i++)  
	{
		// place an alien at width/number_of_aliens * i
		auto alien = make_shared<SFAsset>(SFASSET_SKELETONARCHER, sf_window);
		auto pos = Point2((canvas_w - 50), canvas_h / number_of_aliens * (i + 0.5)); //Position of the aliens
		alien->SetPosition(pos);
		aliens.push_back(alien);
	}

	const int number_of_aliensflip = 2;

	for (int i = 0; i < number_of_aliensflip; i++)
	{
		// place an alien at width/number_of_aliens * i
		auto alien = make_shared<SFAsset>(SFASSET_SKELETONARCHERFLIP, sf_window);
		auto pos = Point2((canvas_w - 500), canvas_h -300 / number_of_aliensflip * (i + 1)); //Position of the aliens
		alien->SetPosition(pos);
		aliens.push_back(alien);
	}

	auto alienf = make_shared<SFAsset>(SFASSET_SKELETONARCHERFLIP, sf_window);
	auto apos = Point2((canvas_w /2 + 60), canvas_h/2 - 20); //Position of the aliens
	alienf->SetPosition(apos);
	aliens.push_back(alienf);

	auto alien1 = make_shared<SFAsset>(SFASSET_SKELETONARCHER, sf_window);
	auto apos1 = Point2((canvas_w /2 - 100), canvas_h / 2 + 70); //Position of the aliens
	alien1->SetPosition(apos1);
	aliens.push_back(alien1);

	//Walls
	const int number_of_wallsBottom = 6;

	for (int i = 0; i < number_of_wallsBottom; i++) {
		auto wall = make_shared<SFAsset>(SFASSET_WALL, sf_window);
		auto wallPos = Point2((canvas_w / number_of_wallsBottom * (i + 0.5) ), 16);
		wall->SetPosition(wallPos);
		walls.push_back(wall);
	}
	//Walls
	const int number_of_wallsTop = 19;

	for (int i = 0; i < number_of_wallsTop; i++) {
		auto wall = make_shared<SFAsset>(SFASSET_WALL, sf_window);
		auto wallPos = Point2((canvas_w / number_of_wallsTop * (i + 0.7)), 464);
		wall->SetPosition(wallPos);
		walls.push_back(wall);
	}
	//Walls

	auto wall = make_shared<SFAsset>(SFASSET_WALL, sf_window);
	auto wallPos = Point2((canvas_w / 2 - 60 ), canvas_h / 2);
	wall->SetPosition(wallPos);
	walls.push_back(wall);

	auto wall1 = make_shared<SFAsset>(SFASSET_WALL, sf_window);
	auto wallPos1 = Point2((canvas_w / 2 + 90), canvas_h - 590 / 2);
	wall1->SetPosition(wallPos1);
	walls.push_back(wall1);

	auto wall2 = make_shared<SFAsset>(SFASSET_WALL, sf_window);
	auto wallPos2 = Point2((canvas_w / 2 -90), canvas_h - 690 / 2);
	wall2->SetPosition(wallPos2);
	walls.push_back(wall2);

	auto wall3 = make_shared<SFAsset>(SFASSET_WALL, sf_window);
	auto wallPos3 = Point2((canvas_w / 2 + 70), canvas_h - 290 / 2);
	wall3->SetPosition(wallPos3);
	walls.push_back(wall3);

	//Coins

	auto coin = make_shared<SFAsset>(SFASSET_COIN, sf_window);
	auto pos = Point2((canvas_w /2), canvas_h / 2);
	coin->SetPosition(pos);
	coins.push_back(coin);

	auto coin1 = make_shared<SFAsset>(SFASSET_COIN, sf_window);
	auto pos1 = Point2((canvas_w / 2 - 110), canvas_h - 100 / 2);
	coin1->SetPosition(pos1);
	coins.push_back(coin1);

	auto coin2 = make_shared<SFAsset>(SFASSET_COIN, sf_window);
	auto pos2 = Point2((canvas_w / 2 - 200), canvas_h - 600 / 2);
	coin2->SetPosition(pos2);
	coins.push_back(coin2);

	auto coin3 = make_shared<SFAsset>(SFASSET_COIN, sf_window);
	auto pos3 = Point2((canvas_w / 2 + 280), canvas_h - 630 / 2);
	coin3->SetPosition(pos3);
	coins.push_back(coin3);
	
}

SFApp::~SFApp() {
}

/**
* Handle all events that come from SDL.
* These are timer or keyboard events.
*/
void SFApp::OnEvent(SFEvent& event) {
	SFEVENT the_event = event.GetCode();
	switch (the_event) {
	case SFEVENT_QUIT:
		is_running = false;
		break;
	case SFEVENT_UPDATE:
		OnUpdateWorld();
		OnRender();
		break;
	case SFEVENT_PLAYER_LEFT:
		player->GoWest();
		break;
	case SFEVENT_PLAYER_RIGHT:
		player->GoEast();
		break;
	case SFEVENT_PLAYER_UP:
		player->GoNorth();
		break;
	case SFEVENT_PLAYER_DOWN:
		player->GoSouth();
		break;
	case SFEVENT_FIRE:
		fire++;
		FireProjectile();
		break;
	}
}

int SFApp::OnExecute() {
	// Execute the app
	SDL_Event event;
	while (SDL_WaitEvent(&event) && is_running) {
		// wrap an SDL_Event with our SFEvent
		SFEvent sfevent((const SDL_Event)event);
		// handle our SFEvent
		OnEvent(sfevent);
	}
	return 0;
}

int SFApp::Counter()
{
	j++;
	if(j == 502)
    {
	j=0;
    }
}

void SFApp::OnUpdateWorld() {
	// Update projectile positions
	for (auto p : projectiles) {
		p->GoNorthBullet();
	}
	// Detect collision player to wall
	for (auto w : walls) {
		if (w->CollidesWith(player)) {
			player->HandleCollisionWall();
			w->HandleCollision();
			cout << "Touching Wall" << endl;
		}
	}int coinAmount = 0;
	// Detect collision player to coin
	for (auto c : coins) {
		
		if (player->CollidesWith(c)) 
		{
			player->HandleCollision();
			c->HandleCollision();

			cout << "All Coins Collected!" << endl;
			cout << "Congratulations You've Beaten The Game!" << endl;
		}		
	}
	
	// remove coin
	list<shared_ptr<SFAsset>> tmpCoin;
	for (auto c : coins) {
		if (c->IsAlive()) {
			tmpCoin.push_back(c);
			coinAmount++;
			cout << "Coins To Collect : " << coinAmount << endl;
		}
	}
	
	coins.clear();
	coins = list<shared_ptr<SFAsset>>(tmpCoin);
	
	
	// Collision player to enemy
	for (auto a : aliens) {
		if (player->CollidesWith(a)) {
			player->HandleCollision();
			a->HandleCollision();
			is_running = false;
		}
	}

	//Make aliens move
        for (auto a : aliens) {
	   	Counter();
		if(j == 500){
			a->EnemyGoWest();
			
}		
		if(j == 501){
			a->EnemyGoEast();
			
	}
}

	// Detect collision bullet to wall
	for (auto p : projectiles) {
		for (auto w : walls) {
			if (p->CollidesWith(w)) {
				p->HandleCollision();
				w->HandleCollision();
			}
		}
	}

	// Detect collision bullet to alien
	for (auto p : projectiles) {
		for (auto a : aliens) {
			if (p->CollidesWith(a)) {
				p->HandleCollision();
				a->HandleCollision();
			}
		}
	}
	// remove dead skeleton
	list<shared_ptr<SFAsset>> tmp;
	for (auto a : aliens) {
		if (a->IsAlive()) {
			tmp.push_back(a);
		}
	}
	aliens.clear();
	aliens = list<shared_ptr<SFAsset>>(tmp);

	// remove projectile
	list<shared_ptr<SFAsset>> tmpProjectiles;
	for (auto p : projectiles) {
		if (p->IsAlive()) {
			tmpProjectiles.push_back(p);
		}
	}

	projectiles.clear();
	projectiles = list<shared_ptr<SFAsset>>(tmpProjectiles);

}

void SFApp::OnRender() {
	SDL_RenderClear(sf_window->getRenderer());

	// draw the player
	player->OnRender();

	for (auto p : projectiles) {
		if (p->IsAlive()) { p->OnRender(); }
	}

	for (auto a : aliens) {
		if (a->IsAlive()) { a->OnRender(); }
	}

	for (auto c : coins) {
		c->OnRender();
	}

	for (auto w : walls)
	{
		w->OnRender();
	}

	// Switch the off-screen buffer to be on-screen
	SDL_RenderPresent(sf_window->getRenderer());
}

void SFApp::FireProjectile() {
	auto pb = make_shared<SFAsset>(SFASSET_PROJECTILE, sf_window);
	auto v = player->GetPosition();
	pb->SetPosition(v);
	projectiles.push_back(pb);
}
