/* Copyright (C) Siqi.Wu - All Rights Reserved

 * Written by Siqi.Wu <lion547016@gmail.com>, July 2019

 */



#pragma once



#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"

#include "Engine/LatentActionManager.h"



#include "VivoxCore.h"



#include "VivoxPresence.generated.h"



/**

* \brief The online status of the user. Note that these must be equivalent to Vivox Native SDK vx_buddy_presence_state values.

*/

UENUM(BlueprintType)

enum class EVivoxPresenceStatus : uint8

{

    /**

    * \brief Not available (offline)

    */

            Unavailable	UMETA(DisplayName = "unavailable"),

    /**

    * \brief Generally available

    */

            Available	UMETA(DisplayName = "available"),

    /**

    * \brief Available to chat

    */

            Chat	UMETA(DisplayName = "chat"),

    /**

    * \brief Do Not Disturb

    */

            DoNotDisturb	UMETA(DisplayName = "do not disturb"),

    /**

    * \brief Away

    */

            Away	UMETA(DisplayName = "unavailable"),

    /**

    * \brief Away for an extended period of time.

    */

            ExtendedAway	UMETA(DisplayName = "extendedAway"),

};



UCLASS(BlueprintType)

class VIVOX_API UVivoxPresence : public UObject

{

public:

    GENERATED_BODY()



public:

    static UVivoxPresence *CreateVivoxPresence(Presence *VivoxPresence);



	virtual void BeginDestroy() override;



    /**

     * \brief The online status of the user

     */

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")

    EVivoxPresenceStatus CurrentStatus() const;



    /**

     * \brief An optional message published by that user

     */

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")

    FString Message() const;



FORCEINLINE bool operator ==(const Presence &RHS) const {

    return VivoxPresence == VivoxPresence;

}



FORCEINLINE bool operator !=(const Presence &RHS) const {

    return VivoxPresence != VivoxPresence;

}



    Presence *VivoxPresence;

};

