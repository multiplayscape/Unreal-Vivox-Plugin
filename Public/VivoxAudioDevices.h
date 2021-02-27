/* Copyright (C) Siqi.Wu - All Rights Reserved

 * Written by Siqi.Wu <lion547016@gmail.com>, July 2019

 */



#pragma once



#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"

#include "Engine/LatentActionManager.h"

#include "Delegates/IDelegateInstance.h"



#include "VivoxCore.h"



#include "VivoxStatus.h"

#include "VivoxAudioDevice.h"



#include "VivoxAudioDevices.generated.h"



UCLASS(BlueprintType)

class VIVOX_API UVivoxAudioDevices : public UObject

{

public:

    GENERATED_BODY()



    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAfterAvailableDeviceAdded, const UVivoxAudioDevice *, vivoxAudioDevice);

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBeforeAvailableDeviceRemoved, const UVivoxAudioDevice *, vivoxAudioDevice);

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEffectiveDeviceChanged, const UVivoxAudioDevice *, vivoxAudioDevice);



public:

    static UVivoxAudioDevices *CreateVivoxAudioDevices(IAudioDevices *VivoxVoiceAudioDevices);



    /**

     * \brief This event is raised after a device has been added to the AvailableDevices() collection.

     */

    UPROPERTY(BlueprintAssignable, Category="vivox")

    FAfterAvailableDeviceAdded AfterDeviceAvailableAdded;



    /**

     * \brief This event is raised before a device will be removed from the AvailableDevices() collection.

     */

    UPROPERTY(BlueprintAssignable, Category="vivox")

    FBeforeAvailableDeviceRemoved BeforeAvailableDeviceRemoved;



    /**

    * \brief When the effective device changed, this event is fired.

    * Use this to inform users that the device in use has changed.

    */

    UPROPERTY(BlueprintAssignable, Category="vivox")

    FEffectiveDeviceChanged EffectiveDeviceChanged;



    FDelegateHandle AfterDeviceAvailableAddedHandle;

    FDelegateHandle BeforeAvailableDeviceRemovedHandle;

    FDelegateHandle EffectiveDeviceChangedHandle;



	virtual void BeginDestroy() override;

    /**

     * \brief Call SetActiveDevice() with this device if you wish to use the operating system's default device.

     * \remarks Name and Id show the current values of the underlying device this virtual device is tracking.

     */

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")

    UVivoxAudioDevice * SystemDevice() const;



    /**

    * \brief Call SetActiveDevice() with this device if you wish to use the operating system's default communication device

    * \remarks Name and Id show the current values of the underlying device this virtual device is tracking.

    * This always points to the same physical device as SystemDevice except on Windows, UWP, and Xbox One, where it may differ.

    */

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")

    UVivoxAudioDevice * CommunicationDevice() const;



    /**

     * \brief The available devices on this system. Lists both "virtual" and physical devices.

     * \remarks Key equals the IAudioDevice Value's Id(). Virtual devices appear on the available device list

     * with their virtual Name and Id, e.g. "Default System Device" whereas the IAudioDevice reference

     * returned by SystemDevice() and similar methods show what the effective device would be upon setting

     * it active. You may use either IAudioDevice with SetActiveDevice() to set a virtual device active.

     */

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")

    TMap<FString, UVivoxAudioDevice *> AvailableDevices() const;



    /**

     * \brief Call this to set the active audio device. This will take effect immediately for all open sessions.

     * \param device The device to set active

     * \param theDelegate a delegate to call when this operation completes.

     * \return VxErrorInvalidArgument if device is empty

     */

    UFUNCTION(BlueprintCallable,

            meta = (Latent, LatentInfo = "LatentInfo", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"),

            Category = "vivox"

    )

    void SetActiveDevice(

            UObject *WorldContextObject,

            struct FLatentActionInfo LatentInfo,

            const UVivoxAudioDevice * device,

            EVivoxStatus &status,

            FString &delegateMessage

    );



    /**

     * \brief The active audio device

     */

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")

    UVivoxAudioDevice * ActiveDevice();



    /**

    * \brief The effective audio device. If the active device is set to SystemDevice or CommunicationDevice, then the effective device will reflect the actual device used.

    */

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")

    UVivoxAudioDevice * EffectiveDevice();



    /**

     * \brief AudioGain for the device.

     * This is a value between -50 and 50. Positive values make the audio louder, negative values make the audio softer.

     * 0 leaves the value unchanged (default).

     * This applies to all active audio sessions.

     */

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")

    int VolumeAdjustment();



    /**

    * \brief Set AudioGain for the device.

    * This is a value between -50 and 50. Positive values make the audio louder, negative values make the audio softer.

    * 0 leaves the value unchanged (default).

    * This applies to all active audio sessions.

    * \param value the desired audio gain

    * \return VxErrorInvalidArgument if value is outside of -50 -> 50

    */

    UFUNCTION(BlueprintCallable, Category = "vivox")

    EVivoxStatus SetVolumeAdjustment(int value);



    /**

     * \brief Get whether audio is muted for this device.

     */

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")

    bool Muted() const;



    /**

     * \brief Set whether audio is muted for this device.

     *ActiveDevice

     * Set to true in order to stop the audio device from capturing or rendering audio.

     * Default is false.

     */

    UFUNCTION(BlueprintCallable, Category = "vivox")

    void SetMuted(bool value);



    /**

    * \brief Refresh the list of available devices.

    *

    * \remarks Manual use of this method should be rare, as the device lists are refreshed automatically

    * when Vivox is initialized and when an auto device hotswap event is raised by the system.

    * Do note however that an automatic or manual call must complete before IAudioDevices methods for

    * ActiveDevice, EffectiveDevice, AvailableDevices etc, will return valid values. It may take up to

    * 200 milliseconds before the list of devices is refreshed.

    */

    UFUNCTION(BlueprintCallable, Category = "vivox")

    void Refresh();



    IAudioDevices *VivoxVoiceAudioDevices;

};

