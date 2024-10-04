// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AGun;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// const인 이유: 이 함수를 호출했다고 캐릭터의 어떠한 상태도 바뀌면 안되기 때문
	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	// 0과 1사이의 백분율(퍼센트)을 나타내는 float 값을 리턴
	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser) override;

	void Shoot();
	
private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	// AddControllerYawInput에 매핑하지 않음
	// -> 프레임 속도에 대해 독립적으로 만들기 위해 속도를 계산해야 하기 때문
	// LookUp 속도제어함
	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);
	

	// 회전속도
	// 상하좌우 동일하게
	UPROPERTY(EditAnywhere)
	float RotationRate = 10;

	// 초기 체력 값
	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;

	UPROPERTY(VisibleAnywhere)
	float Health;

	// 런타임에서 편집하길 원치 않기 때문에
	// 총 자체가 아님
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> GunClass;

	// 진짜 총을 말함
	UPROPERTY()
	AGun *Gun;
};
