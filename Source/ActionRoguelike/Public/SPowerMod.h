// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "SPowerMod.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASPowerMod : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

public:

	// Sets default values for this actor's properties
	ASPowerMod();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void ApplyMod(AActor* AffectedActor);

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere)
	float HealthDelta;
	
	FTimerHandle TimerHandle_Respawn;

private:
	void AdjustCapsuleSize() const;
	void Respawn();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
