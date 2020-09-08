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
    TArray<Node*> blockedPath;
    bool foundGoal = false;
    while (!foundGoal)
    {
        for (size_t i = 0; i < currentNode->currentPosition->connectedWalkables.Num(); i++)
        {
            if (currentNode->stepsTaken > actionPoints) {
                if (!blockedPath.Contains(currentNode)) {
                    blockedPath.Add(currentNode);
                }
                continue;
            }
            UWalkableComponent* searchNode = currentNode->currentPosition->connectedWalkables[i];
            if (searchNode == end)
            {
                foundGoal = true;
            }
            if (!searched.Contains(searchNode))
            {
                if (!searched.Contains(currentNode->currentPosition)) {
                    searched.Add(currentNode->currentPosition);
                }
                    
                Node* node = new Node();
                node->stepsTaken = currentNode->stepsTaken + searchNode->Cost();
                node->currentPosition = searchNode;
                node->previous = currentNode;
                if (searchNode->blocked) {
                    blockedPath.Add(node);
                }
                else {
                    searchList.Add(node);
                }
               
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
                searchList.RemoveAt(0);

            }
            else
            {
                if (blockedPath.Num() > 0) {
                    int closest = 0;
                    FVector endPoint = end->GetOwner()->GetActorLocation();
                    float closestDist = FVector::Dist(endPoint, blockedPath[0]->currentPosition->GetOwner()->GetActorLocation());
                    for (size_t i = 0; i < blockedPath.Num(); i++)
                    {
                        float temp = FVector::Dist(endPoint, blockedPath[i]->currentPosition->GetOwner()->GetActorLocation());
                        if (temp < closestDist) {
                            closestDist = temp;
                            closest = i;
                        }
                        
                    }
                    if (blockedPath[closest]->previous) {
                        currentNode = blockedPath[closest]->previous;
                        foundGoal = true;
                    }
                    else {
                        return TArray<UWalkableComponent*>();
                    }
                    
                }
                else {
                    return TArray<UWalkableComponent*>();
                }
                
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
