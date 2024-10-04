// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"

void AShooterAIController::BeginPlay()
{
    // BeginPlay를 생성 후 블루프린트에서 문제가 생기면 Super를 붙였는지 확인
    Super::BeginPlay();

    if (AIBehavior != nullptr)
    {
        RunBehaviorTree(AIBehavior);
        APawn *AIPawn = GetPawn();
        // AI 컨트롤러에도 GetPawn 메소드를 사용하면 제어 하에 있는 폰을 얻을 수 있음
        // 폰을 제어할 수 있음
        // BeginPlay에서 위치도 얻어서 시작 위치를 설정할 수 있음
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), AIPawn->GetActorLocation());
    }
}

void AShooterAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

bool AShooterAIController::IsDead() const
{
    AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn());
    // 널 포인터가 아니라면 사망했는지 아닌지 상태를 쿼리할 수 있음
    if (ControlledCharacter != nullptr)
    {
        return ControlledCharacter->IsDead();
    }

    // 캐릭터 사망(폰을 컨트롤하고 있지 않다면 이 컨트롤러는 사망한 것이나 다름없음)
    return true;
}
