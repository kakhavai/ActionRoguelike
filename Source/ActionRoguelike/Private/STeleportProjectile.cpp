// Fill out your copyright notice in the Description page of Project Settings.


#include "STeleportProjectile.h"

#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

ASTeleportProjectile::ASTeleportProjectile()
{
    //SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASTeleportProjectile::OnBeginOverlap);
}


void ASTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_StageTeleport, this, &ASTeleportProjectile::StageTeleportAndDestroy, 1.0f);
	
}

void ASTeleportProjectile::StageTeleportAndDestroy()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_StageTeleport);
	const FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f);
	TeleportLocation = this->GetActorLocation();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TeleportDisband, this->GetActorLocation(), SpawnRotation, true);
	SetActorHiddenInGame(true);
	
	GetWorldTimerManager().SetTimer(TimerHandle_TeleportAndDestroy, this, &ASTeleportProjectile::TeleportAndDestroy, .2);
}

void ASTeleportProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if(EndPlayReason == EEndPlayReason::Destroyed)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_StageTeleport);
		GetWorldTimerManager().ClearTimer(TimerHandle_TeleportAndDestroy);

	}
}




void ASTeleportProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASCharacter* Player = Cast<ASCharacter>(OtherActor);
	UE_LOG(LogTemp, Warning, TEXT("ASBlackholeProjectile::OnBeginOverlap!"));

	if(!Player)
	{
		SetActorEnableCollision(false);
		StageTeleportAndDestroy();
	}
}

void ASTeleportProjectile::TeleportAndDestroy()
{
	UE_LOG(LogTemp, Log, TEXT("ASTeleportProjectile::TeleportPlayer instantiated"));
	ASCharacter* Player = Cast<ASCharacter>(GetInstigator());
	bool checkPorted = Player->TeleportTo(TeleportLocation, Player->GetCameraComp()->GetComponentRotation());
	if(!checkPorted)
	{
		UE_LOG(LogTemp, Warning, TEXT("ASBlackholeProjectile::TeleportAndDestroy Failed to find safe landing!"));
	}
	this->QuickDestroy();
}
