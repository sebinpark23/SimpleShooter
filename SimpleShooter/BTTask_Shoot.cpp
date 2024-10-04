// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Shoot.h"
#include "AIController.h"
#include "ShooterCharacter.h"

UBTTask_Shoot::UBTTask_Shoot()
{
    NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    // 폰 가져오기: ShooterCharacter.h의 Shoot 함수를 호출해야 하기 때문
    // AI 컨트롤러에도 GetPawn 메소드를 사용하면 제어 하에 있는 폰을 얻을 수 있음
    if (OwnerComp.GetAIOwner() == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    AShooterCharacter *Character = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (Character == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    Character->Shoot();

    // 연속 발사가 아님
    return EBTNodeResult::Succeeded;
}
