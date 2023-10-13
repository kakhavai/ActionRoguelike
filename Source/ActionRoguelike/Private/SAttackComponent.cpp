// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttackComponent.h"

#include "SCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
USAttackComponent::USAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void USAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USAttackComponent::FireProjectile(TSubclassOf<AActor> ActorClass)
{
	ASCharacter* MyOwner = Cast<ASCharacter>(GetOwner());

	if(ensure(MyOwner))
	{
		MyOwner->PlayAnimMontage(AttackAnim);
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "FireProjectile_TimeElapsed", ActorClass);
		MyOwner->GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, Delegate, .2, false);
	}
	
	//GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack);
}

void USAttackComponent::FireProjectile_TimeElapsed(TSubclassOf<AActor> ActorClass)
{
	ASCharacter* MyOwner = Cast<ASCharacter>(GetOwner());

	if(ensure(MyOwner))
	{
		FVector HandLocation = MyOwner->GetMesh()->GetSocketLocation("Muzzle_01");
	
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(MyOwner); // Ignore this actor during the trace
		FHitResult OutHit;
		float Radius = 30.0f;
	
		// Perform the line trace
		// The ECollisionChannel::ECC_Visibility means it will trace against any object that is visible
		FVector EndLocation = MyOwner->GetCameraComp()->GetComponentLocation() + (MyOwner->GetCameraComp()->GetForwardVector()) * 5000;
		bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, MyOwner->GetCameraComp()->GetComponentLocation(), EndLocation,
														 ECollisionChannel::ECC_Visibility, CollisionParams);
	
		// DrawDebugLine(GetWorld(), MyOwner->GetCameraComp()->GetComponentLocation(), EndLocation, FColor::Orange, false, 2.0f, 0, 2.0f);
		// DrawDebugSphere(GetWorld(), OutHit.ImpactPoint, Radius, 32, FColor::Orange, false, 2.0f);
	
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation,
																		 bHit ? OutHit.ImpactPoint : OutHit.TraceEnd);
		FTransform SpawnTM = FTransform(LookAtRotation, HandLocation);

		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = MyOwner;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		GetWorld()->SpawnActor<AActor>(ActorClass, SpawnTM, SpawnParams);
	}

}

void USAttackComponent::BlackholeAttack()
{
	FireProjectile(BlackholeProjectileClass);
}

void USAttackComponent::PrimaryAttack()
{
	FireProjectile(ProjectileClass);
}


void USAttackComponent::TeleportAttack()
{
	FireProjectile(TeleportProjectileClass);
}
