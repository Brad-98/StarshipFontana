#ifndef SFASSET_H
#define SFASSET_H

#include <string>
#include <memory>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


using namespace std;

#include "SFCommon.h"
#include "SFEvent.h"
#include "SFWindow.h"
#include "SFBoundingBox.h"

enum SFASSETTYPE {SFASSET_DEAD, SFASSET_PLAYER, SFASSET_PROJECTILE, SFASSET_SKELETONARCHER, SFASSET_SKELETONARCHERFLIP, SFASSET_COIN, SFASSET_WALL};

class SFAsset {
public:
  SFAsset(const SFASSETTYPE, const std::shared_ptr<SFWindow>);
  SFAsset(const SFAsset&);
  virtual ~SFAsset();

  virtual void      SetPosition(Point2 &);
  virtual void      SetPrevPosition(Point2 &);
  virtual Point2    GetPosition();
  virtual SFAssetId GetId();
  virtual void      OnRender();
  virtual void      GoEast();
  virtual void      EnemyGoEast();
  virtual void      GoWest();
  virtual void      EnemyGoWest();
  virtual void      GoNorth();
  virtual void      GoNorthBullet();
  virtual void		GoSouth();
  virtual void      SetNotAlive();
  virtual bool      IsAlive();
  virtual void      HandleCollision(); 
  virtual void      HandleCollisionWall();
  virtual bool                      CollidesWith(shared_ptr<SFAsset>);
  virtual shared_ptr<SFBoundingBox> GetBoundingBox();
private:
  SDL_Texture               * sprite;
  shared_ptr<SFBoundingBox>   bbox;
  SFASSETTYPE                 type;
  SFAssetId                   id;
  std::shared_ptr<SFWindow>   sf_window;
  std::shared_ptr<Point2>	  prevposition;

  static int SFASSETID;
};

#endif

