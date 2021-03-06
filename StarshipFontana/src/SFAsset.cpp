#include "SFAsset.h"

using namespace std;

int SFAsset::SFASSETID = 0;

SFAsset::SFAsset(SFASSETTYPE type, std::shared_ptr<SFWindow> window) : type(type), sf_window(window) {
	this->id = ++SFASSETID;

	switch (type) {
	case SFASSET_PLAYER:
		sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/playerUp.png");
		break;
	case SFASSET_PROJECTILE:
		sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/projectile.png");
		break;
	case SFASSET_SKELETONARCHER:
		sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/skeletonArcher.png");
		break;
	case SFASSET_SKELETONARCHERFLIP:
		sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/skeletonArcherFlip.png");
		break;
	case SFASSET_COIN:
		sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/coin.png");
		break;
	case SFASSET_WALL:
		sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/wall.png");
		break;
	}

	if (!sprite) {
		cerr << "Could not load asset of type " << type << endl;
		throw SF_ERROR_LOAD_ASSET;
	}

	// Get texture width & height
	int w, h;
	SDL_QueryTexture(sprite, NULL, NULL, &w, &h);

	// Initialise bounding box
	bbox = make_shared<SFBoundingBox>(SFBoundingBox(Vector2(0.0f, 0.0f), w, h));
}

SFAsset::SFAsset(const SFAsset& a) {
	sprite = a.sprite;
	sf_window = a.sf_window;
	bbox = a.bbox;
	type = a.type;
	prevposition = a.prevposition;
}

SFAsset::~SFAsset() {
	bbox.reset();
	if (sprite) {
		SDL_DestroyTexture(sprite);
		sprite = nullptr;
	}
}

/**
* The logical coordinates in the game assume that the screen
* is indexed from 0,0 in the bottom left corner.  The blittable
* coordinates of the screen map 0,0 to the top left corner. We
* need to convert between the two coordinate spaces.  We assume
* that there is a 1-to-1 quantisation.
*/
Vector2 GameSpaceToScreenSpace(SDL_Renderer* renderer, Vector2 &r) {
	int w, h;
	SDL_GetRendererOutputSize(renderer, &w, &h);

	return Vector2(
		r.getX(),
		(h - r.getY())
	);
}

void SFAsset::SetPosition(Point2 & point) {
	Vector2 v(point.getX(), point.getY());
	bbox->SetCentre(v);
}

void SFAsset::SetPrevPosition(Point2 & prevposition)
{
	Vector2 v(prevposition.getX(), prevposition.getY());
	bbox->SetCentre(v);
}

Point2 SFAsset::GetPosition() {
	return Point2(bbox->centre->getX(), bbox->centre->getY());
}

SFAssetId SFAsset::GetId() {
	return id;
}

void SFAsset::OnRender() {
	// 1. Get the SDL_Rect from SFBoundingBox
	SDL_Rect rect;

	Vector2 gs = (*(bbox->centre) + (*(bbox->extent_x) * -1)) + (*(bbox->extent_y) * 1);
	Vector2 ss = GameSpaceToScreenSpace(sf_window->getRenderer(), gs);
	rect.x = ss.getX();
	rect.y = ss.getY();
	rect.w = bbox->extent_x->getX() * 2;
	rect.h = bbox->extent_y->getY() * 2;

	// 2. Blit the sprite onto the level
	SDL_RenderCopy(sf_window->getRenderer(), sprite, NULL, &rect);
}

void SFAsset::GoWest() {
	prevposition = make_shared<Point2>(GetPosition());
	Vector2 c = *(bbox->centre) + Vector2(-5.0f, 0.0f);
	if (!(c.getX() - bbox->extent_x->getX() < 0)) {
		bbox->centre.reset();
		bbox->centre = make_shared<Vector2>(c);
		sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/playerLeft.png");
	}
}

void SFAsset::EnemyGoWest() {
	int w, h;
	SDL_GetRendererOutputSize(sf_window->getRenderer(), &w, &h);
	Vector2 c = *(bbox->centre) + Vector2(-15.0f, 0.0f);
	if (!(c.getX() - bbox->extent_x->getX() < 0)) {
		bbox->centre.reset();
		bbox->centre = make_shared<Vector2>(c);
}
	
}

void SFAsset::GoEast() {
	prevposition = make_shared<Point2>(GetPosition());
	int w, h;
	SDL_GetRendererOutputSize(sf_window->getRenderer(), &w, &h);

	Vector2 c = *(bbox->centre) + Vector2(5.0f, 0.0f);
	if (!(c.getX() + bbox->extent_x->getX() > w)) {
		bbox->centre.reset();
		bbox->centre = make_shared<Vector2>(c);
		sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/playerRight.png");
	}
}

void SFAsset::EnemyGoEast() {
	int w, h;
	SDL_GetRendererOutputSize(sf_window->getRenderer(), &w, &h);
Vector2 c = *(bbox->centre) + Vector2(15.0f, 0.0f);
	if (!(c.getX() + bbox->extent_x->getX() > w)) {
	
		bbox->centre.reset();
		bbox->centre = make_shared<Vector2>(c);
}	
}

void SFAsset::GoNorth() {
	prevposition = make_shared<Point2>(GetPosition());
	int w, h;
	SDL_GetRendererOutputSize(sf_window->getRenderer(), &w, &h);

	Vector2 c = *(bbox->centre) + Vector2(0.0f, 5.0f);
	if ((c.getY() + bbox->extent_y->getY() < h)) {
		bbox->centre.reset();
		bbox->centre = make_shared<Vector2>(c);
		sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/playerUp.png");
	}
}

void SFAsset::GoNorthBullet() {
	int w, h;
	SDL_GetRendererOutputSize(sf_window->getRenderer(), &w, &h);

	Vector2 c = *(bbox->centre) + Vector2(0.0f, 5.0f);
	
		bbox->centre.reset();
		bbox->centre = make_shared<Vector2>(c);
		sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/swordThrow.png");
}

void SFAsset::GoSouth() {
	prevposition = make_shared<Point2>(GetPosition());
	int w, h;
	SDL_GetRendererOutputSize(sf_window->getRenderer(), &w, &h);

	Vector2 c = *(bbox->centre) + Vector2(0.0f, -5.0f);
	if (!(c.getY() - bbox->extent_y->getY() < 0)) {
		bbox->centre.reset();
		bbox->centre = make_shared<Vector2>(c);
		sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/playerDown.png");
	}
}

bool SFAsset::CollidesWith(shared_ptr<SFAsset> other) {
	return bbox->CollidesWith(other->bbox);
}

shared_ptr<SFBoundingBox> SFAsset::GetBoundingBox() {
	return bbox;
}

void SFAsset::SetNotAlive() {
	type = SFASSET_DEAD;
}

bool SFAsset::IsAlive() {
	return (SFASSET_DEAD != type);
}

void SFAsset::HandleCollision() {
	if (SFASSET_PROJECTILE == type || SFASSET_SKELETONARCHER == type || SFASSET_SKELETONARCHERFLIP == type || SFASSET_COIN == type || SFASSET_WALL == type) {
		SetNotAlive();
	}
}

void SFAsset::HandleCollisionWall() {
	if (SFASSET_PLAYER == type) {
		SetPrevPosition(*(prevposition.get()));
	}
}
