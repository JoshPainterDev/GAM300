// Fill out your copyright notice in the Description page of Project Settings.

#include "DarkShotVR.h"
#include "ProjectileArrow.h"


// Sets default values
AProjectileArrow::AProjectileArrow()
{
    PrimaryActorTick.bCanEverTick = true;
    
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow"));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("StaticMesh'/Game/ViveMaterials/arrowTest_01.arrowTest_01'"));
    StaticMesh->SetStaticMesh(SphereVisualAsset.Object);
    current_arrow_state = FOLLOW;
}

// Called when the game starts or when spawned
void AProjectileArrow::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AProjectileArrow::Tick( float DeltaTime )
{
	Super::Tick(DeltaTime);
    
    switch (current_arrow_state) 
    {
    case FOLLOW:
        FollowRightMotionController();
        break;
    case AIM:
        SnapToBow();
        break;
    case SHOOT:
        this->Destroy(true);
        break;
    }
}


void AProjectileArrow::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    for (int i = 0; i < 2000; ++i)
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("COLLISION DETECTED"));
}

void AProjectileArrow::FollowRightMotionController()
{
    FVector pos;
    FRotator orientation;
    if (USteamVRFunctionLibrary::GetHandPositionAndOrientation(0, EControllerHand::Right, pos, orientation))
    {
        SetActorLocationAndRotation(pos, orientation);
    }
    
}

void AProjectileArrow::SnapToBow()
{
	FVector righthand_pos;
	FRotator righthand_orientation;
	FVector lefthand_pos;
	FRotator lefthand_orientation;
	if (USteamVRFunctionLibrary::GetHandPositionAndOrientation(0, EControllerHand::Right, righthand_pos, righthand_orientation))
	{
		if (USteamVRFunctionLibrary::GetHandPositionAndOrientation(0, EControllerHand::Left, lefthand_pos, lefthand_orientation))
		{
			FVector new_locked_arrow_pos = lefthand_pos;
			if (righthand_pos.Y > lefthand_pos.Y + 20)
				new_locked_arrow_pos.Y = lefthand_pos.Y + 20;
			else
				new_locked_arrow_pos.Y = righthand_pos.Y;

			lefthand_orientation.Yaw += 45;
			SetActorLocationAndRotation(new_locked_arrow_pos, lefthand_orientation);
		}
	}
    //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("AIM"));
}

