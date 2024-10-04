// Fill out your copyright notice in the Description page of Project Settings.

#include "Gun.h"

#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"

// Sets default values
AGun::AGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

void AGun::PullTrigger()
{
	// 파티클 이펙트 스포닝
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

	FHitResult Hit;
	FVector ShotDirection;
	bool bSuccess = GunTrace(Hit, ShotDirection);
	if (bSuccess)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Hit.Location);

		AActor *HitActor = Hit.GetActor(); // 데미지를 입힐 대상 액터 찾기
		// 존재하지 않는 액터에 데미지를 입히면, 크래시됨
		if (HitActor != nullptr)
		{
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
			AController* OwnerController = GetOwnerController();
			// 총 컨트롤러가 바로 데미지를 발생
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AGun::GunTrace(FHitResult &Hit, FVector &ShotDirection)
{
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr)
		// 오너 컨트롤러를 가져오지 못하면 GunTrace를 처리하지 못함
		return false;

	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	// 회전
	ShotDirection = -Rotation.Vector();

	// 끝점을 계산하는 방법
	// 정지 지점에 회전(회전 방향을 가리키는 벡터)을 더하고, 우리가 가고자 하는 거리를 곱하기
	FVector End = Location + Rotation.Vector() * MaxRange;
	Hit;
	FCollisionQueryParams Params;
	// 무시되는 액터
	Params.AddIgnoredActor(this);		// 총 무시
	Params.AddIgnoredActor(GetOwner()); // 소유주가 총에 맞지 않게
	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

AController* AGun::GetOwnerController() const
{
	// 캐릭터를 가져옴
	APawn *OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
		// 폰을 가져오지 못하면 컨트롤러를 가져오지 못하므로 널 포인터를 리턴
		return nullptr;
	// 컨트롤러 가져오기
	AController *OwnerController = OwnerPawn->GetController();
	
	return OwnerController;
	
}
