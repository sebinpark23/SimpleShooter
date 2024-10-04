// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_ClearBlackboardValue.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ClearBlackboardValue::UBTTask_ClearBlackboardValue()
{
    NodeName = TEXT("Clear Blackboard Value");
}

EBTNodeResult::Type UBTTask_ClearBlackboardValue::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    // OwnerComp: 행동 트리와 관련된 블랙보드
    // 선택된 키가 무엇인지 확인
    // 선택된 블랙보드 키의 값 지움
    OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());

    return EBTNodeResult::Succeeded;
}
