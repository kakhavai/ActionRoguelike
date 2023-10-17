// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USAttackComponent();

	void BlackholeAttack();
	
	void PrimaryAttack();

	void TeleportAttack();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FTimerHandle TimerHandle_PrimaryAttack;

	UPROPERTY(EditAnywhere, Category = "Attack");
	UAnimMontage* AttackAnim;
	
	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* MuzzleVfx;

	UPROPERTY(EditAnywhere, Category = "Attack");
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack");
	TSubclassOf<AActor> BlackholeProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack");
	TSubclassOf<AActor> TeleportProjectileClass;
	
	void FireProjectile(TSubclassOf<AActor> ActorClass);

	UFUNCTION()
	void FireProjectile_TimeElapsed(TSubclassOf<AActor> ActorClass);
	

	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
