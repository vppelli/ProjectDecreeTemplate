// Fill out your copyright notice in the Description page of Project Settings.


#include "C_PlayerBase.h"
#include "Character/Attributes/StandardAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
AC_PlayerBase::AC_PlayerBase(const FObjectInitializer& objectInitializer) : Super(objectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	StandardAttributes = CreateDefaultSubobject<UStandardAttributeSet>(TEXT("StandardAttributeSet"));
}

// Called to bind functionality to input
void AC_PlayerBase::SetupPlayerInputComponent(UInputComponent* playerInputComponent)
{
	Super::SetupPlayerInputComponent(playerInputComponent);
	if (UEnhancedInputComponent* playerEnhancedInputComponent = Cast<UEnhancedInputComponent>(playerInputComponent))
	{
		if (MovementAction)
		{
			playerEnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ThisClass::MovementInputHandler);
		}
		if (LookAction)
		{
			playerEnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::LookInputInputHandler);
		}
		for (const FAbilityInputToInputActionBinding& binding : AbilityInputBindings.Bindings)
		{
			playerEnhancedInputComponent->BindAction(binding.InputAction, ETriggerEvent::Triggered, this, &ThisClass::AbilityInputBindingPressedHandler, binding.AbilityInput);
			playerEnhancedInputComponent->BindAction(binding.InputAction, ETriggerEvent::Completed, this, &ThisClass::AbilityInputBindingReleasedHandler, binding.AbilityInput);
		}
	}

}

void AC_PlayerBase::PawnClientRestart()
{
	Super::PawnClientRestart();
	if (const APlayerController* playerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			subsystem->ClearAllMappings();
			subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

// Called every frame
void AC_PlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AC_PlayerBase::SetupInitialAbilitiesAndEffects()
{
	if (IsValid(AbilitySystemComponent) == false || IsValid(StandardAttributes) == false)
	{
		return;
	}
	if (IsValid(InitialAbilitySet))
	{
		InitiallyGrantedAbilitySpecHandles.Append(InitialAbilitySet->GrantAbilitiesToAbilitySystem(AbilitySystemComponent));
	}
	if (IsValid(InitialGameplayEffect))
	{
		AbilitySystemComponent->ApplyGameplayEffectToSelf(InitialGameplayEffect->GetDefaultObject<UGameplayEffect>(), 0.f, AbilitySystemComponent->MakeEffectContext());
	}

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UStandardAttributeSet::GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthAttributeChanged);
}

void AC_PlayerBase::OnHealthAttributeChanged(const FOnAttributeChangeData& onAttributeChangeData) const
{
	if (FMath::IsNearlyEqual(onAttributeChangeData.NewValue, 0.f) && onAttributeChangeData.OldValue > 0)
	{
		// Trigger Death Effect
	}
}

// Called when the game starts or when spawned
void AC_PlayerBase::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		SetupInitialAbilitiesAndEffects();
	}
}

float AC_PlayerBase::GetHealth() const
{
	if (IsValid(StandardAttributes) == false)
	{
		return 0.f;
	}
	return StandardAttributes->GetHealth();
}

void AC_PlayerBase::MovementInputHandler(const FInputActionValue& inputActionValue)
{
	const FVector valueAsVector = inputActionValue.Get<FVector>();
	AddMovementInput(GetActorForwardVector(), valueAsVector.Y);
	AddMovementInput(GetActorRightVector(), valueAsVector.X);
}

void AC_PlayerBase::LookInputInputHandler(const FInputActionValue& inputActionValue)
{
	const FVector valueAsVector = inputActionValue.Get<FVector>();
	AddControllerYawInput(valueAsVector.X);
	AddControllerPitchInput(valueAsVector.Y);
}

void AC_PlayerBase::AbilityInputBindingPressedHandler(EAbilityInput abilityInput)
{
	AbilitySystemComponent->AbilityLocalInputPressed(static_cast<uint32>(abilityInput));
}

void AC_PlayerBase::AbilityInputBindingReleasedHandler(EAbilityInput abilityInput)
{
	AbilitySystemComponent->AbilityLocalInputReleased(static_cast<uint32>(abilityInput));
}