/* Copyright (C) Siqi.Wu - All Rights Reserved
 * Written by Siqi.Wu <lion547016@gmail.com>, May 2019
 */

#include "VivoxAudioDevices.h"

#include "IAudioDevices.h"
#include "VivoxCore.h"

#include "LatentActions.h"

#include "VivoxGlobals.h"

UVivoxAudioDevices *UVivoxAudioDevices::CreateVivoxAudioDevices(IAudioDevices *VoiceAudioDevices) {
    UVivoxAudioDevices *vivoxAudioDevices = NewObject<UVivoxAudioDevices>();
    vivoxAudioDevices->VivoxVoiceAudioDevices = VoiceAudioDevices;

    {
        vivoxAudioDevices->AfterDeviceAvailableAddedHandle = vivoxAudioDevices->VivoxVoiceAudioDevices->EventAfterDeviceAvailableAdded.AddLambda([vivoxAudioDevices](const IAudioDevice & vivoxAudioDevice) {
            if (vivoxAudioDevices->AfterDeviceAvailableAdded.IsBound()) {
                vivoxAudioDevices->AfterDeviceAvailableAdded.Broadcast(UVivoxAudioDevice::CreateVivoxAudioDevice((IAudioDevice *)&vivoxAudioDevice));
            }
        });

        vivoxAudioDevices->BeforeAvailableDeviceRemovedHandle =  vivoxAudioDevices->VivoxVoiceAudioDevices->EventBeforeAvailableDeviceRemoved.AddLambda([vivoxAudioDevices](const IAudioDevice & vivoxAudioDevice) {
            if (vivoxAudioDevices->BeforeAvailableDeviceRemoved.IsBound()) {
                vivoxAudioDevices->BeforeAvailableDeviceRemoved.Broadcast(UVivoxAudioDevice::CreateVivoxAudioDevice((IAudioDevice *)&vivoxAudioDevice));
            }
        });

        vivoxAudioDevices->EffectiveDeviceChangedHandle = vivoxAudioDevices->VivoxVoiceAudioDevices->EventEffectiveDeviceChanged.AddLambda([vivoxAudioDevices](const IAudioDevice & vivoxAudioDevice) {
            if (vivoxAudioDevices->EffectiveDeviceChanged.IsBound()) {
                vivoxAudioDevices->EffectiveDeviceChanged.Broadcast(UVivoxAudioDevice::CreateVivoxAudioDevice((IAudioDevice *)&vivoxAudioDevice));
            }
        });
    }

    return vivoxAudioDevices;
}

void UVivoxAudioDevices::BeginDestroy() {
	this->AfterDeviceAvailableAdded.Clear();
	this->BeforeAvailableDeviceRemoved.Clear();
	this->EffectiveDeviceChanged.Clear();

	if (this->VivoxVoiceAudioDevices != nullptr) {
		this->VivoxVoiceAudioDevices->EventAfterDeviceAvailableAdded.Remove(this->AfterDeviceAvailableAddedHandle);
		this->VivoxVoiceAudioDevices->EventBeforeAvailableDeviceRemoved.Remove(this->BeforeAvailableDeviceRemovedHandle);
		this->VivoxVoiceAudioDevices->EventEffectiveDeviceChanged.Remove(this->EffectiveDeviceChangedHandle);
	}

	this->VivoxVoiceAudioDevices = nullptr;
	this->UObject::BeginDestroy();
}

UVivoxAudioDevice *UVivoxAudioDevices::SystemDevice() const {
    return UVivoxAudioDevice::CreateVivoxAudioDevice((IAudioDevice *)&(this->VivoxVoiceAudioDevices->SystemDevice()));
}

UVivoxAudioDevice *UVivoxAudioDevices::CommunicationDevice() const {
    return UVivoxAudioDevice::CreateVivoxAudioDevice((IAudioDevice *)&(this->VivoxVoiceAudioDevices->CommunicationDevice()));
}

TMap<FString, UVivoxAudioDevice *> UVivoxAudioDevices::AvailableDevices() const {
    TMap<FString, UVivoxAudioDevice *> availableDevices;
    for (const TPair<FString, IAudioDevice *> &vivoxAvailableDevice : VivoxVoiceAudioDevices->AvailableDevices()) {
        availableDevices.Add(vivoxAvailableDevice.Key,
                             UVivoxAudioDevice::CreateVivoxAudioDevice(vivoxAvailableDevice.Value));
    }
    return availableDevices;
}

class FSetActiveDeviceAction : public FPendingLatentAction {
private:
    // trigger for finish
    bool completed = false;
    IAudioDevices *VivoxVoiceAudioDevices;
    FName ExecutionFunction;
    int32 OutputLink;
    FWeakObjectPtr CallbackTarget;

public:
    FSetActiveDeviceAction(
            IAudioDevices *vivoxVoiceAudioDevices,
            const UVivoxAudioDevice * device,
            EVivoxStatus &status,
            FString &delegateMessage,
            const FLatentActionInfo &LatentInfo
    )
            : VivoxVoiceAudioDevices(vivoxVoiceAudioDevices),
              ExecutionFunction(LatentInfo.ExecutionFunction),
              OutputLink(LatentInfo.Linkage), CallbackTarget(LatentInfo.CallbackTarget) {

        IAudioDevices::FOnSetActiveDeviceCompletedDelegate OnSetActiveDeviceCompletedCallback;
        OnSetActiveDeviceCompletedCallback.BindLambda(
                [this, &status, &delegateMessage, vivoxVoiceAudioDevices](VivoxCoreError Status,
                                                                          const FString &DelegateMessage) {
                    status = fromVivox(Status);
                    if (status != EVivoxStatus::Success) {
                        LOG_VIVOX_WARNING(FString::Printf(TEXT("Join failed: %s"),
                                                          ANSI_TO_TCHAR(FVivoxCoreModule::ErrorToString(Status))));
                    }
                    delegateMessage = DelegateMessage;
                    completed = true;
                }
        );
        status = fromVivox(vivoxVoiceAudioDevices->SetActiveDevice(*(device->VivoxVoiceAudioDevice),
                                                                   OnSetActiveDeviceCompletedCallback));
    }

    void UpdateOperation(FLatentResponse &Response) override {
        Response.FinishAndTriggerIf(completed, ExecutionFunction, OutputLink, CallbackTarget);
    }

#if WITH_EDITOR

    // Returns a human readable description of the latent operation's current state
    FString GetDescription() const override {
        return "Set Active Device Vivox";
    }

#endif
};

void UVivoxAudioDevices::SetActiveDevice(
        UObject *WorldContextObject,
        struct FLatentActionInfo LatentInfo,
        const UVivoxAudioDevice * device,
        EVivoxStatus &status,
        FString &delegateMessage
) {
    if (UWorld *World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert)) {
        FLatentActionManager &LatentActionManager = World->GetLatentActionManager();
        if (LatentActionManager.FindExistingAction<FSetActiveDeviceAction>(LatentInfo.CallbackTarget,
                                                                           LatentInfo.UUID) ==
            NULL) {
            LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
                                             new FSetActiveDeviceAction(this->VivoxVoiceAudioDevices,
                                                                        device,
                                                                        status,
                                                                        delegateMessage,
                                                                        LatentInfo));
        }
    }
}

UVivoxAudioDevice *UVivoxAudioDevices::ActiveDevice() {
    return UVivoxAudioDevice::CreateVivoxAudioDevice((IAudioDevice *)&(this->VivoxVoiceAudioDevices->ActiveDevice()));
}

UVivoxAudioDevice *UVivoxAudioDevices::EffectiveDevice() {
    return UVivoxAudioDevice::CreateVivoxAudioDevice((IAudioDevice *)&(this->VivoxVoiceAudioDevices->EffectiveDevice()));
}

int UVivoxAudioDevices::VolumeAdjustment() {
    return this->VivoxVoiceAudioDevices->VolumeAdjustment();
}

EVivoxStatus UVivoxAudioDevices::SetVolumeAdjustment(int value) {
    return fromVivox(VivoxVoiceAudioDevices->SetVolumeAdjustment(value));
}

bool UVivoxAudioDevices::Muted() const {
    return VivoxVoiceAudioDevices->Muted();
}

void UVivoxAudioDevices::SetMuted(bool value) {
    return VivoxVoiceAudioDevices->SetMuted(value);
}

void UVivoxAudioDevices::Refresh() {
    return VivoxVoiceAudioDevices->Refresh();
}