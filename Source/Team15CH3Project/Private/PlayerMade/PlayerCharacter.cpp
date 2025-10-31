// PlayerCharacter.cpp

#include "PlayerMade/PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "PlayerMade/CharacterStatsComponent.h"
#include "Components/CapsuleComponent.h"
#include "PlayerMade/AutoAttackComponent.h" 
#include "Animation/AnimMontage.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// === 1. 탑다운 카메라 설정 ===
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	CameraBoom->TargetArmLength = 1500.0f;
	CameraBoom->SetRelativeRotation(FRotator(-120.0f, 0.0f, 0.0f));
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bInheritRoll = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	StatsComponent = CreateDefaultSubobject<UCharacterStatsComponent>(TEXT("StatsComponent"));

	// === 2. 이동 및 회전 설정 ===
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// 💡 [수정됨] 회전 속도 제한 (보간) - 낮을수록 부드럽게 회전
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

	// 💡 [수정됨] 가속도를 최대로 올려 즉각 이동
	GetCharacterMovement()->MaxAcceleration = 99999.0f;

	// 💡 [수정됨] 감속도를 최대로 올려 즉각 정지
	GetCharacterMovement()->BrakingDecelerationWalking = 99999.0f;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 💡 [수정됨] BeginPlay에서 MaxWalkSpeed 설정
	if (UCharacterStatsComponent* StatsComp = FindComponentByClass<UCharacterStatsComponent>())
	{
		GetCharacterMovement()->MaxWalkSpeed = StatsComp->MoveSpeed;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		// 마우스 커서 표시
		PlayerController->bShowMouseCursor = true;
		PlayerController->DefaultMouseCursor = EMouseCursor::Crosshairs;

		// Enhanced Input Context 적용
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Enhanced Input 바인딩
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		}
	}
}

// Move 콜백 함수 (WASD 처리)
void APlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// 카메라의 Yaw 회전을 기준으로 방향을 계산 (탑다운 시점 이동)
		// const FRotator Rotation = CameraBoom->GetComponentRotation();
		//const FRotator Rotation = Controller->GetControlRotation();
		//const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

		if (!MovementVector.IsNearlyZero())
		{
			const FVector MoveDir = (ForwardDirection * MovementVector.Y + RightDirection * MovementVector.X).GetSafeNormal();
			// 💡 [수정됨] 직접 회전 코드를 제거하여 CharacterMovementComponent의 RotationRate를 따르게 함
			//SetActorRotation(MoveDir.Rotation());
		}

		// if (UCharacterStatsComponent* StatsComp = FindComponentByClass<UCharacterStatsComponent>())
		// {
		// 	GetCharacterMovement()->MaxWalkSpeed = StatsComp->MoveSpeed; //// maxwalkspeed 상태여서 속도에 가속이 적용된상태 -> 속도 일정하도록 
		// }


		//if (!FMath::IsNearlyZero(MovementVector.Y))
		//{
		//	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		//	AddMovementInput(ForwardDirection, MovementVector.Y);
		//	/*FRotator(0.0f, 0.0f, 0.0f);*/
		//}

		//if (!FMath::IsNearlyZero(MovementVector.X))
		//{
		//	const FVector RightDirection = FRotator(YawRotation).GetUnitAxis(EAxis::Y);
		//	AddMovementInput(RightDirection, MovementVector.X);
		//}
	}
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!StatsComponent)
	{
		return 0.0f;
	}

	// StatsComponent의 데미지 처리 로직 호출
	const float ActualDamage = StatsComponent->TakeDamage(DamageAmount);

	// 💡 [수정됨] 사망했는지 확인하고, 캐릭터의 물리적 반응 처리
	if (StatsComponent->IsDead() && GetLifeSpan() == 0.0f)
	{
		PlayerIsDead();
	}

	return ActualDamage;
}

void APlayerCharacter::PlayerIsDead()
{
	UE_LOG(LogTemp, Error, TEXT("PLAYER IS DEAD! Deactivating input and collision."));

	if (DeathMontage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		// 몽타주를 재생하여 Death_Forward 애니메이션을 즉시 출력
		GetMesh()->GetAnimInstance()->Montage_Play(DeathMontage, 1.0f);
	}

	// 1. 입력 비활성화
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		DisableInput(PC);
	}

	// 2. 캐릭터 충돌 비활성화
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (AutoAttackComponent)
	{
		AutoAttackComponent->StopAutoAttack();
	}

	// 5초 후에 액터를 제거하는 로직을 추가합니다. 
	SetLifeSpan(5.0f);
}