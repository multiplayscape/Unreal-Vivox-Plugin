/* Copyright (C) Siqi.Wu - All Rights Reserved

 * Written by Siqi.Wu <lion547016@gmail.com>, July 2019

 */



#pragma once



#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"

#include "Engine/LatentActionManager.h"

#include "Delegates/IDelegateInstance.h"



#include "VivoxCore.h"



#include "VivoxAccountId.h"



#include "VivoxPresenceSubscription.generated.h"



class UVivoxLoginSession;

class UVivoxPresenceLocation;



UCLASS(BlueprintType)

class VIVOX_API UVivoxPresenceSubscription : public UObject

{

public:

    GENERATED_BODY()



    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAfterLocationAdded, const UVivoxPresenceLocation *, vivoxPresenceLocation);

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAfterLocationUpdated, const UVivoxPresenceLocation *, vivoxPresenceLocation);

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBeforeLocationRemoved, const UVivoxPresenceLocation *, vivoxPresenceLocation);



public:

    static UVivoxPresenceSubscription *CreateVivoxPresenceSubscription(IPresenceSubscription *VivoxPresenceSubscription);



    /**

    * This event is raised after a device has been added to the Locations() collection

    */

    UPROPERTY(BlueprintAssignable, Category="vivox")

    FAfterLocationAdded AfterLocationAdded;



    /**

    * This event is raised after a device has been added to the Locations() collection

    */

    UPROPERTY(BlueprintAssignable, Category="vivox")

    FAfterLocationUpdated AfterLocationUpdated;



    /**

    * This event is raised before a device will be removed from the Locations() collection

    */

    UPROPERTY(BlueprintAssignable, Category="vivox")

    FBeforeLocationRemoved BeforeLocationRemoved;



    FDelegateHandle AfterLocationAddedHandle;

    FDelegateHandle AfterLocationUpdatedHandle;

    FDelegateHandle BeforeLocationRemovedHandle;



	virtual void BeginDestroy() override;



    /**

     * The account that this subscription pertains to.

     */

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")

    FVivoxAccountId SubscribedAccount() const;



    /**

     * The login session owning this subscription

     */

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")

    UVivoxLoginSession * LoginSession();



    /**

     * If the account associated with this subscription is logged in, the Locations lists will have an entry for each location each login session for that user.

     */

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")

    TMap<FString, UVivoxPresenceLocation *> Locations() const;



    IPresenceSubscription *VivoxPresenceSubscription;

};

