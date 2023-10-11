// Fill out your copyright notice in the Description page of Project Settings.


#include "STeleportProjectile.h"

#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

ASTeleportProjectile::ASTeleportProjectile()
{
    SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASTeleportProjectile::OnOverlapBegin);
}


void ASTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_Projectile, this, &ASTeleportProjectile::QuickDestroy, 2.0f);
	
}

void ASTeleportProjectile::QuickDestroy()
{
	FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f);
	TeleportLocation = this->GetActorLocation();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TeleportDisband, this->GetActorLocation(), SpawnRotation, true);
	this->Destroy();
}

void ASTeleportProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if(EndPlayReason == EEndPlayReason::Destroyed)
	{
		ASCharacter* Player = Cast<ASCharacter>(GetInstigator());

		Player->TeleportTo(TeleportLocation, Player->GetCameraComp()->GetComponentRotation());
	}
}


void ASTeleportProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASCharacter* Player = Cast<ASCharacter>(OtherActor);
	if(!Player)
	{
		QuickDestroy();
	}
}
