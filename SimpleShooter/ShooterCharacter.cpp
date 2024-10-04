// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterCharacter.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Error, TEXT("MoveRight AxisValue:"));

	Health = MaxHealth;	// 체력 최대치를 할당 받고 그 다음부터 줄임

	// 매개변수를 받을 건데 기본적으로는 C++를 스포닝함
	// 하지만 여기서는 Gun의 블루프린트 하위를 스포닝
	// -> 블루프린트에서 후킹해야 할 것이 뭔지 알아야 함
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);

	// 뼈 감추기
	// SkinnedMeshComponent.h 들어가서 양식 보기
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	// 생성자에서 할 때와 같이 어태치먼트를 설정하지 않음
	// 런타임에서 어태치먼트를 할 때는 생성자에서와는 다른 함수를 사용
	// actor.h 에 정의되어 있음
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	// Gun이 이 클래스를 오너로 갖게 해야함
	Gun->SetOwner(this);
}

bool AShooterCharacter::IsDead() const
{
	return Health <= 0;
}

float AShooterCharacter::GetHealthPercent() const
{
	// 전체 체력 대비 현재 체력의 백분율
	return Health / MaxHealth;
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// 입력 설정
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AShooterCharacter::LookRightRate);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	// 직접 그 총에 바인딩하는 게 아니라 바인딩을 설정하기 위해 캐릭터에 함수를 넣어야 함
	// 총에 직접 바인딩을 하게 되면 작동하지 않게 됨
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);
}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser)
{
	// 부모 클래스의 기능이 필요하다면 Super 사용
	// 이 정보가 부모의 TakeDamage 함수로 다 전달됨
	// 적용되는 데미지 값
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//DamageToApply 값이 남은 Health 값보다 큰 경우
	DamageToApply = FMath::Min(Health, DamageToApply);
	//남아있는 체력 정보
	Health -= DamageToApply;
	
	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);

	if (IsDead())
	{
		// 캐릭터가 죽으면 게임 모드를 찾아 PawnKilled 호출
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		// 폰이 죽었다는 걸 게임 모드가 알게 됨
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
		// 캐릭터에서 컨트롤러 분리
		// 캐릭터가 죽었으니 컨트롤러가 할 일은 없음
		// 더 이상 총을 쏘지 못함
		DetachFromControllerPendingDestroy();	// 모두 비활성화됨
		// 이동하지 못하게
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 캡슐 콜리전을 비활성화
	}

	return DamageToApply;
}

// MovementInput은 우리가 입력을 넣으면 이미 그게 속도라는 걸 가정함
void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookUpRate(float AxisValue)
{
	// 거리구하기
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::Shoot()
{
	Gun->PullTrigger();
}
