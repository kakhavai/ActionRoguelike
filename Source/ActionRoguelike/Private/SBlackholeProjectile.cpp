// Fill out your copyright notice in the Description page of Project Settings.


#include "SBlackholeProjectile.h"

ASBlackholeProjectile::ASBlackholeProjectile() 
{
	// Your initialization code here...

	UE_LOG(LogTemp, Log, TEXT("ASBlackholeProjectile::ASBlackholeProjectile instantiated"));

}


void ASBlackholeProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}