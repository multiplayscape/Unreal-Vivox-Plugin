/* Copyright (C) Siqi.Wu - All Rights Reserved

 * Written by Siqi.Wu <lion547016@gmail.com>, July 2019

 */



#pragma once



#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"

#include "Engine/LatentActionManager.h"



#include "VivoxCore.h"



#include "VivoxPresenceLocation.generated.h"



class UVivoxPresence;

class UVivoxPresenceSubscription;



UCLASS(BlueprintType)

class VIVOX_API UVivoxPresenceLocation : public UObject {

public:

    GENERATED_BODY()



public:

    static UVivoxPresenceLocation *CreateVivoxPresenceLocation(IPresenceLocation *VivoxPresenceLocation);



	virtual void BeginDestroy() override;



    /**

     * \brief The unique identifier for this account's specific login session.

     */

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")

    FString LocationId() const;



    /**

     * \brief The presence for this account at this location.

     */

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")

    UVivoxPresence *CurrentPresence() const ;



    /**

     * \brief The subscription that owns this presence location. This does not change and therefore will not raise a PropertyChangedEvent.

     */

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")

    UVivoxPresenceSubscription *Subscription() const ;



    IPresenceLocation *VivoxPresenceLocation;

};

