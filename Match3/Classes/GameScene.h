/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Piece.h"

class Game : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(Game);

    bool OnTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void OnTouchEnd(cocos2d::Touch* touch, cocos2d::Event* event);

    void SwapPieces(float dt);
    void RemoveMatches(float dt);

    void CheckForMatches(Piece * checkThis, int i, int j);

    void ShiftCells(float dt);
    void SpawnNewCells(float dt);

private:
    Piece * pieces[5][5];
    Piece * startPiece;
    Piece * endPiece;
    std::vector<Piece*> removePieces;
    int starti, startj, endi, endj;
    int rowsMatched;
    int score;
    int cascadeMultiplier;
    cocos2d::Label* scoreLabel;
    cocos2d::Label* multiplierLabel;
};

#endif // __GAME_SCENE_H__s
