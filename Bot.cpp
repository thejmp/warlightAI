#include <iostream>
#include <string>
#include "Bot.h"
#include <stdlib.h>


Bot::Bot()
{
   // printf("ctor\n");
}

Bot::~Bot()
{
     //printf("dtor\n");
    //dtor
}

void Bot::playGame()
{
    parser.initParser(this);
    parser.parseInput();
}    //plays a single game of Warlight

void Bot::makeMoves()
{

}   //makes moves for a single turn
void Bot::endTurn()
{

}     //indicates to the engine that it has made its move

void Bot::addRegion(unsigned noRegion, unsigned noSuperRegion)
{
    while (  regions.size() <= noRegion)
        {
            regions.push_back(Region());
        }
    regions[noRegion]  = Region(noRegion, noSuperRegion);
    superRegions[noSuperRegion].addRegion(noRegion);
}
void Bot::addNeighbors(unsigned noRegion, unsigned neighbors)
{
    regions[noRegion].addNeighbors(neighbors) ;
    regions[neighbors].addNeighbors(noRegion) ;
}

void Bot::addSuperRegion(unsigned noSuperRegion, int reward)
{
    while (  superRegions.size() <=  noSuperRegion)
        {
            superRegions.push_back(SuperRegion());
        }
    superRegions[noSuperRegion]  = SuperRegion(reward);
}

void Bot::setBotName(string name)
{
    botName = name;
}
void Bot::setOpponentBotName(string name)
{
    opponentBotName = name;
}

void Bot::setArmiesLeft(int nbArmies)
{
    armiesLeft = nbArmies;
}

void Bot::addStartingRegion(unsigned noRegion)
{
     startingRegionsreceived.push_back(noRegion);
}

void Bot::startDelay(int delay)
{

}
void Bot::setPhase(string pPhase)
{
    phase=pPhase;
}
void Bot::executeAction()
{
	vector<int> temp;
	int size = 0;
	int j = 0;
    if (phase=="")
        return;
    if (phase == "pickPreferredRegion")
    {
        unsigned i,nbAns=0;
        for (i = 0; i< startingRegionsreceived.size() && nbAns<6; i++)
        {
            cout << startingRegionsreceived[i];
            nbAns++;
            if (nbAns < 6)
                cout << " ";
            else
            {
                cout << "\n";
                break;
            }
        }
    }
    if (phase == "place_armies")
    {
		int armiesToPlace = armiesLeft / 2;
		for (int i = 0; i < ownedRegions.size(); i++)
		{
			temp = regions[ownedRegions[i]].getNeighbors();
			size = temp.size();
			for (j = 0; j < size - 1; j++)
			{
				if ((regions[temp[j]].getOwner() == "neutral" || regions[temp[j]].getOwner() == opponentBotName) && armiesLeft >= armiesToPlace)
				{
					cout << botName << " place_armies " << ownedRegions[i] << " " << armiesToPlace << ", ";
					regions[ownedRegions[i]].setArmies(regions[ownedRegions[i]].getArmies() + armiesToPlace);
					armiesLeft -= armiesToPlace;
					break;
				}
				else if ((regions[temp[j]].getOwner() == "neutral" || regions[temp[j]].getOwner() == opponentBotName) && armiesLeft > 0)
				{
					cout << botName << " place_armies " << ownedRegions[i] << " " << armiesLeft << ", ";
					regions[ownedRegions[i]].setArmies(regions[ownedRegions[i]].getArmies() + armiesLeft);
					armiesLeft = 0;
					break;
				}
			}
		}
		if (armiesLeft)
		{
			cout << botName << " place_armies " << ownedRegions[0] << " " << armiesLeft << ", ";
			regions[ownedRegions[0]].setArmies(regions[ownedRegions[0]].getArmies() + armiesLeft);
		}
		cout << endl;
    }
    if (phase == "attack/transfer")
	{
		bool move = false;
		for (int i = 0; i < ownedRegions.size(); i++)
		{
			if (regions[ownedRegions[i]].getArmies() > 4)
			{
				temp = regions[ownedRegions[i]].getNeighbors();
				size = temp.size();
				for (j = 0; j < size - 1; j++)
				{
					if (regions[temp[j]].getOwner() == "neutral" || regions[temp[j]].getOwner() == opponentBotName)
						break;
				}
				if (regions[temp[j]].getOwner() == botName)
					j = rand() % temp.size();
				cout << botName << " attack/transfer " << ownedRegions[i] << " " << temp[j] << " " << regions[ownedRegions[i]].getArmies() - 1 << ", ";
				move = true;
			}
		}
		if (move)
			cout << endl;
		else
			cout << "No moves\n";
    }
    phase.clear();
}

void Bot::updateRegion(unsigned noRegion, string playerName, int nbArmies)
{
    regions[noRegion].setArmies(nbArmies);
    regions[noRegion].setOwner(playerName);
    if (playerName == botName)
        ownedRegions.push_back(noRegion);
}
void Bot::addArmies(unsigned noRegion,int nbArmies)
{
    regions[noRegion].setArmies(regions[noRegion].getArmies() + nbArmies);
}
void Bot::moveArmies(unsigned noRegion,unsigned toRegion,int nbArmies)
{
    if (regions[noRegion].getOwner() == regions[toRegion].getOwner()
        && regions[noRegion].getArmies() > nbArmies)
    {
        regions[noRegion].setArmies(regions[noRegion].getArmies() - nbArmies);
        regions[toRegion].setArmies(regions[toRegion].getArmies() + nbArmies);

    }
    else if (regions[noRegion].getArmies() > nbArmies)
    {
        regions[noRegion].setArmies(regions[noRegion].getArmies() - nbArmies);
    }
}

void Bot::resetRegionsOwned()
{
    ownedRegions.clear();
}
