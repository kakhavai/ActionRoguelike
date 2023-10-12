// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SMagicProjectile.h"
#include "STeleportProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASTeleportProjectile : public ASMagicProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASTeleportProjectile();



protected:
	virtual void BeginPlay() override;
	void StageTeleportAndDestroy();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	FTimerHandle TimerHandle_StageTeleport;
	FTimerHandle TimerHandle_TeleportAndDestroy;
	

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* TeleportDisband;


	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	void TeleportAndDestroy();
	FVector TeleportLocation;

};
