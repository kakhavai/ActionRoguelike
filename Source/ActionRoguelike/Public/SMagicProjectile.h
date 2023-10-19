// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Sound/SoundCue.h"
#include "SMagicProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASMagicProjectile();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* MovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* EffectComp;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* ParticleVfx;
	
	UPROPERTY(EditAnywhere, Category = "Audio")
	UAudioComponent* AudioComp;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundCue* FlightSound;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundCue* ImpactSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float DamageValue;
	
	FTimerHandle TimerHandle_Projectile;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void QuickDestroy();

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

};
