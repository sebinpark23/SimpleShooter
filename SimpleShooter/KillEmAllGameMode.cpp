// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "EngineUtils.h"    // 엔진 헬퍼
#include "GameFramework/Controller.h"
#include "ShooterAIController.h"


void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled)
{
    Super::PawnKilled(PawnKilled);

    UE_LOG(LogTemp, Warning, TEXT("폰이 죽었습니다!"));

    APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
    // PlayerController에는 GameHasEnded라는 메소드가 있음
    // 주로 플레이어가 직접 조종하는 Pawn을 제어하는 데 사용
    if (PlayerController != nullptr)
    {
        EndGame(false);
    }

    // For loop 월드 내 ShooterAI를 순회
    for (AShooterAIController* Controller : TActorRange<AShooterAIController>(GetWorld()))
    {
        // 사망하지 않았다면
        if (!Controller->IsDead())
        {
            return;
        }
    }

    // 월드 내 살아있는 AI가 없다는 의미로 게임 종료
    // 모든 플레이어 컨트롤러와 AI 컨트롤러에게 알리는 것
    EndGame(true);
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner)
{
    // 씬에서 모든 컨트롤러를 찾아서 해당 컨트롤러에서 GameHasEnded를 호출
    // 월드에 있는 모든 컨트롤러에 대해 for 반복문을 수행하며 Controller라는 변수로 저장
    for (AController* Controller : TActorRange<AController>(GetWorld()))
    {
        // 승자인지 확인
        bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
        // 플레이어 컨트롤러에 초점을 맞춰야 한다고 알려주는 것으로 카메라는 계속해서 폰을 향하고 있어야 함
        Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
    }
}
