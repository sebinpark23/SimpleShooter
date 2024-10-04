// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGun();

	// 캐릭터의 총 또는 우리가 연결한 입력으로부터 방아쇠를 당길 수 있게
	// 적의 경우는 AI를 이용해서 방아쇠를 당김
	void PullTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent *Root;

	// 실제로 해당 메시가 스켈레탈 메시로 되어있기 때문에
	// USkeletalMeshComponent로 해주어야 함
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent *Mesh;

	// 게임 플레이 중 파티클을 변경하고 싶으면,
	// 예를 들어 파티클 이펙트를 개선한다든지 뭔가를 조종하려고 하거나 할 때 이게 실제로 작동하게 되기 때문
	UPROPERTY(EditAnywhere)
	UParticleSystem *MuzzleFlash;

	UPROPERTY(EditAnywhere)
	USoundBase* MuzzleSound;

	UPROPERTY(EditAnywhere)
	UParticleSystem *ImpactEffect;

	UPROPERTY(EditAnywhere)
	USoundBase* ImpactSound;

	// 가고자 하는 거리
	UPROPERTY(EditAnywhere)
	float MaxRange = 1000;

	UPROPERTY(EditAnywhere)
	float Damage = 10;

	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);

	AController* GetOwnerController() const;	// Gun 클래스의 변수를 수정하지 않아야 하니까
};
