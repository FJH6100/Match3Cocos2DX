#ifndef __PIECE_H__
#define __PIECE_H__

#include "cocos2d.h"
#include <string>

class Piece : public cocos2d::Node
{
public:
	Piece(cocos2d::Scene* scene, int c, int x, int y);
	~Piece();

	cocos2d::Rect GetBounds();
	cocos2d::Vec2 GetLoc();
	int GetColor();
	void SetPiecePosition(cocos2d::Vec2 location);
	cocos2d::Sprite* GetSprite();
	void SetSprite(cocos2d::Sprite* s);
	void SpawnNewPiece(cocos2d::Scene* scene, int c);

private:
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;

	cocos2d::Sprite* piece;

	int color;

	cocos2d::Vec2 loc;
	std::string colors[6] = { "Red.png", "Yellow.png", "Green.png", "Blue.png", "Orange.png", "Purple.png" };
};

#endif // __PIECE_H__
