// Fill out your copyright notice in the Description page of Project Settings.


#include "Pathfinder.h"
#include "Algo/Reverse.h"

Pathfinder::Pathfinder()
{
}


TArray<UWalkableComponent*> Pathfinder::FindPath(UWalkableComponent* start, UWalkableComponent* end)
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
                node->stepsTaken = currentNode->stepsTaken + 1;
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
