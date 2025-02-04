// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ClearBlakcboardValue.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ClearBlakcboardValue::UBTTask_ClearBlakcboardValue()
{
    NodeName = TEXT("Clear Blackboard Value");
}

EBTNodeResult::Type UBTTask_ClearBlakcboardValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());

    return EBTNodeResult::Succeeded;
}