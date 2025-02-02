// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Character/Attributes/StandardAttributeSet.h"
#include "Character/AbilitySet.h"
#include "C_PlayerBase.generated.h"

USTRUCT()
struct FAbilityInputToInputActionBinding
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly);
	UInputAction* InputAction;

	UPROPERTY(EditDefaultsOnly);
	EAbilityInput AbilityInput;
};

USTRUCT()
struct FAbilityInputBindings
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Input");
	TArray<FAbilityInputToInputActionBinding> Bindings;
};



UCLASS()
class PROJECTDECREE_API AC_PlayerBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AC_PlayerBase(const FObjectInitializer& objectInitializer = FObjectInitializer::Get());

	virtual void SetupPlayerInputComponent(class UInputComponent* playerInputComponent) override;
	virtual void PawnClientRestart() override;
	virtual void BeginPlay() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }

	bool IsAlive() const { return GetHealth() > 0.f; }

	float GetHealth() const;

protected:
	// Called when the game starts or when spawned

	UPROPERTY(EditAnywhere, Category = "Input|Binding")
	UInputMappingContext* InputMappingContext{ nullptr };

	UPROPERTY(EditAnywhere, Category="Input|Binding")
	UInputAction* MovementAction{ nullptr };

	UPROPERTY(EditAnywhere, Category = "Input|Binding")
	UInputAction* JumpAction{ nullptr };

	UPROPERTY(EditAnywhere, Category = "Input|Binding")
	UInputAction* LookAction{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	UAbilitySystemComponent* AbilitySystemComponent { nullptr };

	UPROPERTY(VisibleAnywhere, Category = "Abilities")
	UStandardAttributeSet* StandardAttributes{ nullptr };

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	UAbilitySet* InitialAbilitySet{ nullptr };

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<UGameplayEffect> InitialGameplayEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Binding")
	FAbilityInputBindings AbilityInputBindings;

	void MovementInputHandler(const FInputActionValue& inputActionValue);
	void LookInputInputHandler(const FInputActionValue& inputActionValue);
	void AbilityInputBindingPressedHandler(EAbilityInput abilityInput);
	void AbilityInputBindingReleasedHandler(EAbilityInput abilityInput);

	void SetupInitialAbilitiesAndEffects();

	void OnHealthAttributeChanged(const FOnAttributeChangeData& onAttributeChangeData) const;

private:

	UPROPERTY(Transient)
	TArray<FGameplayAbilitySpecHandle> InitiallyGrantedAbilitySpecHandles;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
