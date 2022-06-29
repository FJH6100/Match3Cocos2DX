#include "Piece.h"

USING_NS_CC;


Piece::Piece(cocos2d::Scene* scene, int c, int x, int y)
{
	piece = Sprite::create(colors[c]);
	loc = Vec2(x, y);
	piece->setPosition(loc);
	color = c;
	scene->addChild(piece, 1);
}

Piece::~Piece()
{
}

cocos2d::Rect Piece::GetBounds()
{
	return piece->getBoundingBox();
}

cocos2d::Vec2 Piece::GetLoc()
{
	return loc;
}

int Piece::GetColor()
{
	return color;
}

void Piece::SetPiecePosition(cocos2d::Vec2 location)
{
	loc = location;
	if (piece != nullptr)
		piece->setPosition(location);
}

cocos2d::Sprite* Piece::GetSprite()
{
	return piece;
}

void Piece::SetSprite(cocos2d::Sprite* s)
{
	piece = s;
}

void Piece::SpawnNewPiece(cocos2d::Scene* scene, int c)
{
	piece = Sprite::create(colors[c]);
	piece->setPosition(loc);
	color = c;
	scene->addChild(piece, 1);
}
