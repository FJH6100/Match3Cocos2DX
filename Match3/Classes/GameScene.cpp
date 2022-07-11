#include "GameScene.h"
#include <string>
#include <vector>

USING_NS_CC;

Scene* Game::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();

    // 'layer' is an autorelease object
    auto layer = Game::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GameScene.cpp\n");
}

// on "init" you need to initialize your instance
bool Game::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(Game::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
        float y = origin.y + closeItem->getContentSize().height / 2;
        closeItem->setPosition(Vec2(x, y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    score = 0;
    scoreLabel = Label::createWithTTF(cocos2d::StringUtils::format("%i", score), "fonts/Marker Felt.ttf", 24);
    if (scoreLabel == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        scoreLabel->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height - scoreLabel->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(scoreLabel, 1);
    }
    //Set up touch events
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(Game::OnTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(Game::OnTouchEnd, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    int centerX = visibleSize.width / 2 + origin.x;
    int centerY = visibleSize.height / 2 + origin.y;
    int xOffset = -70;
    int yOffset = -70;
    srand(time(NULL));
    //Generate board
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            int random = 0 + rand() % 6;
            Piece * thisPiece = new Piece(this, random, centerX + xOffset, centerY + yOffset);
            pieces[i][j] = thisPiece;
            xOffset += 35;
        }
        yOffset += 35;
        xOffset = -70;
    }
    //Remove matches before the game begins
    scheduleOnce(CC_SCHEDULE_SELECTOR(Game::RemoveMatches), .5f);
    return true;
}


void Game::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}

//Pick first piece
bool Game::OnTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    Point touchPoint = touch->getLocation();
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (pieces[i][j]->GetSprite() != nullptr)
            {
                Rect rect1 = pieces[i][j]->GetBounds();

                if (rect1.containsPoint(touchPoint))
                {
                    log("Start: %d : %d", i, j);
                    startPiece = pieces[i][j];
                    starti = i;
                    startj = j;
                }
            }
        }
    }
    return true;
}

//Slide to other piece
void Game::OnTouchEnd(cocos2d::Touch* touch, cocos2d::Event* event)
{
    Point touchPoint = touch->getLocation();
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (pieces[i][j]->GetSprite() != nullptr)
            {
                Rect rect1 = pieces[i][j]->GetBounds();

                if (rect1.containsPoint(touchPoint))
                {
                    log("End: %d : %d", i, j);
                    endPiece = pieces[i][j];
                    endi = i;
                    endj = j;
                }
            }
        }
    }
    scheduleOnce(CC_SCHEDULE_SELECTOR(Game::SwapPieces), .5f);
    
}

void Game::SwapPieces(float dt)
{
    if (startPiece != nullptr && endPiece != nullptr)
    {
        //If the pieces are adjacent
        if (!(endi == starti + 1 || endi == starti - 1) != !(endj == startj + 1 || endj == startj - 1))
        {
            Vec2 startLoc = startPiece->GetLoc();
            Vec2 endLoc = endPiece->GetLoc();
            startPiece->SetPiecePosition(endLoc);
            endPiece->SetPiecePosition(startLoc);
            Piece* tempPiece = pieces[starti][startj];
            pieces[starti][startj] = pieces[endi][endj];
            pieces[endi][endj] = tempPiece;
            scheduleOnce(CC_SCHEDULE_SELECTOR(Game::RemoveMatches), .5f);
        }
    }
}

void Game::RemoveMatches(float dt)
{
    rowsMatched = 0;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (pieces[i][j]->GetSprite() != nullptr)
            {
                CheckForMatches(pieces[i][j], i, j);
            }
        }
    }
    if (rowsMatched > 0)
    {
        for (Piece* p : removePieces)
        {
            this->removeChild(p->GetSprite());
            p->SetSprite(nullptr);
            score += 10;
        }
        removePieces.clear();
        scoreLabel->setString(cocos2d::StringUtils::format("%i", score));
        scheduleOnce(CC_SCHEDULE_SELECTOR(Game::ShiftCells), .5f);
        startPiece = nullptr;
        endPiece = nullptr;
        starti, endi, startj, endj = NULL;
    }
    else
    {
        if (startPiece != nullptr && endPiece != nullptr)
        {
            //Move pieces back if no match
            Vec2 startLoc = startPiece->GetLoc();
            Vec2 endLoc = endPiece->GetLoc();
            startPiece->SetPiecePosition(endLoc);
            endPiece->SetPiecePosition(startLoc);
            Piece* tempPiece = pieces[starti][startj];
            pieces[starti][startj] = pieces[endi][endj];
            pieces[endi][endj] = tempPiece;
        }
    }
    startPiece = nullptr;
    endPiece = nullptr;
    starti, endi, startj, endj = NULL;
}

void Game::CheckForMatches(Piece * checkThis, int i, int j)
{
    std::vector<Piece*> verticalPieces;
    std::vector<Piece*> horizontalPieces;
    int rightIncrement = 1;
    int upIncrement = 1;
    //Up
    while (i + upIncrement < 5)
    {
        if (pieces[i + upIncrement][j]->GetSprite() != nullptr && checkThis->GetColor() == pieces[i + upIncrement][j]->GetColor())
        {
            verticalPieces.push_back((pieces[i + upIncrement][j]));
            upIncrement++;
        }
        else
        {
            break;
        }
    }
    //Right
    while (j + rightIncrement < 5)
    {
        if (pieces[i][j + rightIncrement]->GetSprite() != nullptr && checkThis->GetColor() == pieces[i][j + rightIncrement]->GetColor())
        {
            horizontalPieces.push_back((pieces[i][j + rightIncrement]));
            rightIncrement++;
        }
        else
        {
            break;
        }
    }
    if (verticalPieces.size() >= 2)
        verticalPieces.push_back(checkThis);
    if (horizontalPieces.size() >= 2)
        horizontalPieces.push_back(checkThis);
    //Remove Pieces
    if (verticalPieces.size() >= 3)
    {
        rowsMatched++;
        for (Piece* p : verticalPieces)
        {
            removePieces.push_back(p);          
        }
    }
    if (horizontalPieces.size() >= 3)
    {
        rowsMatched++;
        for (Piece* p : horizontalPieces)
        {
            removePieces.push_back(p);
        }
    }
}

//Shift pieces down
void Game::ShiftCells(float dt)
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (pieces[i][j]->GetSprite() == nullptr)
            {
                for (int k = i; k < 5; k++)
                {
                    if (pieces[k][j]->GetSprite() != nullptr)
                    {
                        Vec2 startLoc = pieces[k][j]->GetLoc();
                        Vec2 endLoc = pieces[i][j]->GetLoc();
                        pieces[k][j]->SetPiecePosition(endLoc);
                        pieces[i][j]->SetPiecePosition(startLoc);
                        Piece* tempPiece = pieces[k][j];
                        pieces[k][j] = pieces[i][j];
                        pieces[i][j] = tempPiece;
                        break;
                    }
                }
            }
        }
    }
    scheduleOnce(CC_SCHEDULE_SELECTOR(Game::SpawnNewCells), .2f);
}

//Spawn new cells in the empty spots
void Game::SpawnNewCells(float dt)
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (pieces[i][j]->GetSprite() == nullptr)
            {
                int random = 0 + rand() % 6;
                pieces[i][j]->SpawnNewPiece(this, random);
            }
        }
    }
    //Check if there are more matches
    scheduleOnce(CC_SCHEDULE_SELECTOR(Game::RemoveMatches), .5f);
}
