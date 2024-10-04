// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

void AShooterPlayerController::BeginPlay()
{
    Super::BeginPlay();

    HUD = CreateWidget(this, HUDClass);
    if (HUD != nullptr)
    {
        HUD->AddToViewport();
    }

}

void AShooterPlayerController::GameHasEnded(class AActor *EndGameFocus, bool bIsWinner)
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);

    HUD->RemoveFromViewport();  // 크로스헤어 숨기기
    if (bIsWinner)  // 이긴 경우
    {
        UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);
        if (WinScreen != nullptr)
        {
            // 뷰포트에 추가
            // LoseScreen이 추가되어 화면에 표시
            // 위젯이 생성된 후에 패배 화면이 됨
            WinScreen->AddToViewport();
        }
    }
    else
    {
        UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);
        if (LoseScreen != nullptr)
        {
            // 뷰포트에 추가
            // LoseScreen이 추가되어 화면에 표시
            // 위젯이 생성된 후에 패배 화면이 됨
            LoseScreen->AddToViewport();
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("종료되었습니다"));
    GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}

