// Fill out your copyright notice in the Description page of Project Settings.


#include "Pathfinder.h"
#include "Algo/Reverse.h"

Pathfinder::Pathfinder()
{
}
int Pathfinder::actionPointsSpentLast;

TArray<UWalkableComponent*> Pathfinder::FindPath(UWalkableComponent* start, UWalkableComponent* end, int actionPoints)
{
    Node* currentNode = new Node();
    currentNode->stepsTaken = 0;
    currentNode->currentPosition = start;
    TArray<UWalkableComponent*> searched;
    TArray<Node*> searchList;
    bool foundGoal = false;
    while (!foundGoal)
    {
       
        for (size_t i = 0; i < currentNode->currentPosition->connectedWalkables.Num(); i++)
        {
            if (currentNode->stepsTaken > actionPoints) {
                continue;
            }
            UWalkableComponent* searchNode = currentNode->currentPosition->connectedWalkables[i];
            if (searchNode == end)
            {
                foundGoal = true;
                break;
            }
            if (!searchNode->blocked && !searched.Contains(searchNode))
            {
                if (searched.Contains(currentNode->currentPosition)) {
                    searched.Add(currentNode->currentPosition);
                }
                    
                Node* node = new Node();
                node->stepsTaken = currentNode->stepsTaken + searchNode->Cost();
                node->currentPosition = searchNode;
                node->previous = currentNode;
                searchList.Add(node);
                if (foundGoal)
                {
                    currentNode = node;
                }
            }
        }
        if (!foundGoal)
        {
            if (searchList.Num() > 0)
            {
                currentNode = searchList[0];

            }
            else
            {
                return TArray<UWalkableComponent*>();
            }
        }
    }
    Node* backwardsWalk = currentNode;
    actionPointsSpentLast = currentNode->stepsTaken;
    searched.Empty();
    while (backwardsWalk->currentPosition != start)
    {
        searched.Add(backwardsWalk->currentPosition);
        if (backwardsWalk->previous != nullptr)
        {
            backwardsWalk = backwardsWalk->previous;
        }
    }
    searched.Add(start);
    Algo::Reverse(searched);

    return searched;
}

Pathfinder::~Pathfinder()
{
}
